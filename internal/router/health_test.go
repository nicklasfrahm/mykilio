package router

import (
	"log"
	"os"
	"testing"

	"github.com/gofiber/fiber/v2"

	"github.com/nicklasfrahm/mycelium/pkg/config"
	"github.com/nicklasfrahm/mycelium/pkg/middleware"
	"github.com/nicklasfrahm/mycelium/pkg/util"
)

var app *fiber.App

func TestMain(m *testing.M) {
	// Copy `.env` file.
	envFile := ".env"
	if err := util.Copy("../../"+envFile, envFile); err != nil {
		log.Fatal(err)
	}

	app = fiber.New(fiber.Config{
		ErrorHandler: middleware.Error(),
	})
	app.Use(middleware.ContentTypeJSON())
	app.Mount("/hardware-platforms", Health(config.Load()))

	rc := m.Run()

	if err := os.Remove(envFile); err != nil {
		log.Fatal(err)
	}

	os.Exit(rc)
}
