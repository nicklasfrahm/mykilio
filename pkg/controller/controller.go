package controller

import (
	"database/sql"
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
)

var (
	version    = "dev"
	apiGroup   = "test.mykil.io"
	controller *Controller
)

// Controller is the heart of the application that aggregates all functionality.
type Controller struct {
	APIGroup    string
	APIVersions []string `json:"apiVersions"`
	DB          *sqlx.DB
	Identity    *Identity
	HTTPServer  *fiber.App
	Version     string
}

type InstallHook func(c *Controller) string

// New creates a new controller for the specified API group and version.
func New() *Controller {
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

	// Create new controller instance.
	controller = &Controller{
		APIGroup:    apiGroup,
		APIVersions: make([]string, 0),
		HTTPServer:  NewHTTPServer(),
		Identity:    NewIdentity(),
		Version:     version,
	}

	// Log essential controller startup information.
	log.Info().Msgf("Controller: %s", controller.APIGroup)
	log.Info().Msgf("Version: %s", controller.Version)

	return controller
}

func (c *Controller) ConnectDB() {
	driverName := "postgres"

	db, err := sql.Open(driverName, os.Getenv("DATABASE_URI"))
	if err != nil {
		log.Fatal().Msgf("Failed to open database: %v", err)
	}
	driver, err := postgres.WithInstance(db, &postgres.Config{})
	if err != nil {
		log.Fatal().Msgf("Failed to initiate driver: %v", err)
	}

	// Get current working directory.
	workingDirectory, err := os.Getwd()
	if err != nil {
		log.Fatal().Msgf("Failed to get current working directory: %v", err)
	}

	// Get migration directory.
	migrationDir := os.Getenv("MIGRATION_DIR")
	if migrationDir == "" {
		migrationDir = "db"
	}

	// Initialize migration.
	migrationPath := "file://" + path.Join(workingDirectory, os.Getenv("MIGRATION_DIR"))
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
	log.Info().Msgf("Connected to database: %s", c.APIGroup)

	c.DB = sqlx.NewDb(db, driverName)
}

func (c *Controller) Start() {
	// Configure fallback route.
	c.HTTPServer.Use(middleware.NotFound())

	// Get local port behind proxy.
	localPort := os.Getenv("PORT")
	if localPort == "" {
		localPort = "8080"
	}

	// FIXME: What domain should we display? Especially when
	// this application will always run behind a proxy.
	// Log status of HTTP server.
	log.Info().Msgf("HTTP server: 0.0.0.0:%s", localPort)

	if err := c.HTTPServer.Listen(":" + localPort); err != nil {
		log.Fatal().Msg("Starting ")
	}
}

func (ctrl *Controller) Install(install InstallHook) {
	// Install the API and store the API version.
	ctrl.APIVersions = append(ctrl.APIVersions, install(ctrl))
}
