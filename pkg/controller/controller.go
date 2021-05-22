package controller

import (
	"os"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/joho/godotenv"
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
	Identity    *Identity
	HTTPServer  *fiber.App
	Version     string
}

// APIVersion stores information about an installed API version.
type APIVersion struct {
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
