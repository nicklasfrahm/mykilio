package main

import (
	"os"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/compress"
	"github.com/gofiber/fiber/v2/middleware/cors"
	"github.com/gofiber/fiber/v2/middleware/recover"
	"github.com/gofiber/helmet/v2"
	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mycelium/internal/router"
	"github.com/nicklasfrahm/mycelium/pkg/config"
	"github.com/nicklasfrahm/mycelium/pkg/middleware"
)

func main() {
	log.Logger = log.Output(zerolog.ConsoleWriter{
		Out:        os.Stderr,
		TimeFormat: time.RFC3339,
	})

	cfg := config.Load()

	log.Info().Msg("Service: " + cfg.App)
	log.Info().Msg("Version: " + cfg.Version)

	// Create router.
	app := fiber.New(fiber.Config{
		ErrorHandler:          middleware.Error(),
		DisableStartupMessage: true,
	})

	// Load middlewares.
	app.Use(recover.New())
	app.Use(helmet.New())
	app.Use(cors.New(cors.Config{
		AllowHeaders:     "Accept,Authorization,Content-Type,X-CSRF-Token",
		AllowCredentials: true,
		MaxAge:           600,
	}))
	app.Use(compress.New(compress.Config{
		Level: compress.LevelBestCompression,
	}))
	app.Use(middleware.RedirectSlashes())
	app.Use(middleware.ContentTypeJSON())

	// Mount routers and handlers.
	app.Mount("/health", router.Health(cfg))

	// Configure fallback route.
	app.Use(middleware.NotFound())

	log.Info().Msg("Service online: " + cfg.ServiceURI + "/health")
	if err := app.Listen(":" + cfg.LocalPort); err != nil {
		log.Fatal().Err(err).Msg("Service failed")
	}
}
