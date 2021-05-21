package controller

import (
	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/compress"
	"github.com/gofiber/fiber/v2/middleware/cors"
	"github.com/gofiber/fiber/v2/middleware/recover"
	"github.com/gofiber/helmet/v2"
	"github.com/nicklasfrahm/mykilio/pkg/middleware"
)

func NewHTTPServer() *fiber.App {
	// Create new fiber app HTTP server.
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

	return app
}
