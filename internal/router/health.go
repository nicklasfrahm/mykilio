package router

import (
	"net/http"

	"github.com/gofiber/fiber/v2"

	"github.com/nicklasfrahm/mykilio/pkg/config"
	"github.com/nicklasfrahm/mykilio/pkg/middleware"
	"github.com/nicklasfrahm/mykilio/pkg/model"
	"github.com/nicklasfrahm/mykilio/pkg/util"
)

var health *model.Health

// ListHealth displays application server information.
func ListHealth(c *fiber.Ctx) error {
	return c.Status(http.StatusOK).JSON(util.DataBody{
		Data: *health,
	})
}

// Health returns a router for the service.
func Health(cfg *config.Config) *fiber.App {
	router := fiber.New(fiber.Config{
		ErrorHandler: middleware.Error(),
	})

	// Cache application health.
	health = new(model.Health)
	*health = model.Health{
		Version:   cfg.Version,
		Hostname:  cfg.Hostname,
		StartedAt: cfg.StartedAt,
		Identity:  cfg.ServiceIdentity,
	}

	// Configure route handlers.
	router.Get("/", ListHealth)

	return router
}
