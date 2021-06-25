package controller

import (
	"database/sql"
	"net/http"
	"os"
	"path"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/golang-migrate/migrate/v4"
	"github.com/golang-migrate/migrate/v4/database/postgres"
	_ "github.com/golang-migrate/migrate/v4/source/file"
	"github.com/jmoiron/sqlx"
	"github.com/joho/godotenv"
	_ "github.com/lib/pq"
	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mykilio/pkg/middleware"
	"github.com/nicklasfrahm/mykilio/pkg/util"
)

var controller *Controller

// Metadata contains information about the controller.
type Metadata struct {
	Version  string `json:"version"`
	APIGroup string `json:"apiGroup"`
}

// Controller is the heart of the application that aggregates all functionality.
type Controller struct {
	APIVersions      []string `json:"apiVersions"`
	DB               *sqlx.DB
	Identity         *Identity
	HTTPServer       *fiber.App
	Metadata         Metadata
	WorkingDirectory string
}

type InstallHook func(c *Controller) string

// New creates a new controller for the specified API group and version.
func New(meta *Metadata) *Controller {
	// Return the controller if it was already initialized.
	if controller != nil {
		return controller
	}

	// Configure logger.
	log.Logger = log.Output(zerolog.ConsoleWriter{
		Out:        os.Stderr,
		TimeFormat: time.RFC3339,
	})

	// Load environment variables from `.env` file.
	godotenv.Load()

	// Get current working directory.
	workingDirectory, err := os.Getwd()
	if err != nil {
		log.Fatal().Msgf("Failed to get current working directory: %v", err)
	}

	// Get certificate directory.
	certDir := util.GetenvOptional("CERTIFICATE_DIR", path.Join(workingDirectory, "certs"))

	// Create new controller instance.
	controller = &Controller{
		APIVersions:      make([]string, 0),
		HTTPServer:       NewHTTPServer(),
		Identity:         NewIdentity(certDir),
		Metadata:         *meta,
		WorkingDirectory: workingDirectory,
	}

	// Log essential controller startup information.
	log.Info().Msgf("Controller: %s", controller.Metadata.APIGroup)
	log.Info().Msgf("Version: %s", controller.Metadata.Version)
	log.Info().Msgf("Certificate directory: %s", certDir)

	return controller
}

func (c *Controller) ConnectDB() *Controller {
	// Get migration directory.
	migrationDir := util.GetenvOptional("MIGRATION_DIR", path.Join(c.WorkingDirectory, "db", c.Metadata.APIGroup))
	log.Info().Msgf("Migration directory: %s", migrationDir)

	// Initialize database connection.
	driverName := "postgres"
	db, err := sql.Open(driverName, util.GetenvMandatory("DATABASE_URI"))
	if err != nil {
		log.Fatal().Msgf("Failed to open database: %v", err)
	}
	driver, err := postgres.WithInstance(db, &postgres.Config{})
	if err != nil {
		log.Fatal().Msgf("Failed to initiate driver: %v", err)
	}
	log.Info().Msgf("Database driver: %s", driverName)

	// Initialize migration.
	migrationPath := "file://" + migrationDir
	m, err := migrate.NewWithDatabaseInstance(migrationPath, driverName, driver)
	if err != nil {
		log.Fatal().Msgf("Failed to initiate migration: %v", err)
	}

	// Run migration.
	err = m.Up()
	if err != nil && err != migrate.ErrNoChange {
		log.Fatal().Msgf("Failed to run migration: %v", err)
	}

	// Check schema version.
	schemaVersion, dirty, err := m.Version()
	if err != nil {
		log.Fatal().Msgf("Failed to read schema version: %v", err)
	}
	dirtyState := "clean"
	if dirty {
		dirtyState = "dirty"
	}
	log.Info().Msgf("Schema version: %d-%s", schemaVersion, dirtyState)
	log.Info().Msgf("Connected to database: %s", c.Metadata.APIGroup)

	c.DB = sqlx.NewDb(db, driverName)

	return c
}

func (ctrl *Controller) Start() {
	// Mount controller-specific routes and handlers.
	// api

	ctrl.HTTPServer.Get("/", func(c *fiber.Ctx) error {
		return c.Status(http.StatusOK).JSON(util.DataBody{
			Data: struct {
				Metadata
			}{
				ctrl.Metadata,
			},
		})
	})

	// Configure fallback route.
	ctrl.HTTPServer.Use(middleware.NotFound())

	// Get local port behind proxy.
	localPort := os.Getenv("PORT")
	if localPort == "" {
		localPort = "8080"
	}

	// FIXME: What domain should we display? Especially when
	// this application will always run behind a proxy.
	// Log status of HTTP server.
	log.Info().Msgf("HTTP server: 0.0.0.0:%s", localPort)

	if err := ctrl.HTTPServer.Listen(":" + localPort); err != nil {
		log.Fatal().Msgf("Failed to listen for connections: %v", err)
	}
}

func (ctrl *Controller) Install(install InstallHook) *Controller {
	// Install the API and store the API version.
	ctrl.APIVersions = append(ctrl.APIVersions, install(ctrl))

	return ctrl
}
