package controller

import (
	// Standard library dependencies.
	"os"
	"time"

	// External dependencies.
	"github.com/gofiber/fiber/v2"
	"github.com/joho/godotenv"
	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"

	// Project-internal dependencies.

	"github.com/nicklasfrahm/mykilio/pkg/middleware"
)

// Controller is the heart of the application that aggregates all functionality.
type Controller struct {
	HTTPServer *fiber.App
	Identity   *Identity
}

// New creates a new controller for the specified API group and version.
func New(apiGroup string) *Controller {
	// Configure logger.
	log.Logger = log.Output(zerolog.ConsoleWriter{
		Out:        os.Stderr,
		TimeFormat: time.RFC3339,
	})

	// Load environment variables from `.env` file.
	godotenv.Load()

	// Create new controller instance.
	ctrl := &Controller{
		HTTPServer: NewHTTPServer(),
		Identity:   NewIdentity(),
	}

	return ctrl
}

func (c Controller) Start() {
	// Configure fallback route.
	c.HTTPServer.Use(middleware.NotFound())

	// FIXME: What domain should we display? Especially when
	// this application will always run behind a proxy.
	// Log status of HTTP server.
	log.Info().Msgf("HTTP server online: %s", "FIXME")

	// Get local port behind proxy.
	localPort := os.Getenv("PORT")
	if localPort == "" {
		localPort = "8080"
	}

	if err := c.HTTPServer.Listen(":" + localPort); err != nil {
		log.Fatal().Err(err).Msg("Starting ")
	}
}
