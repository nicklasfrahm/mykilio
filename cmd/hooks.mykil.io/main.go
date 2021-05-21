package main

import (
	"net/http"

	"github.com/gofiber/fiber/v2"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mykilio/pkg/controller"
	"github.com/nicklasfrahm/mykilio/pkg/util"
)

var (
	apiVersion = "api.example.com/v1"
	version    = "dev"
)

type APIInfo struct {
	APIVersion string `json:"apiVersion"`
	Version    string `json:"version"`
}

func ListAPI(c *fiber.Ctx) error {
	return c.Status(http.StatusOK).JSON(util.DataBody{
		Data: APIInfo{
			APIVersion: apiVersion,
			Version:    version,
		},
	})
}

func main() {
	// Create new controller for the specified apiGroup.
	ctlr := controller.New(apiVersion)
	log.Info().Msgf("Controller: %s", apiVersion)

	// Mount controller specific routes and handlers.
	ctlr.HTTPServer.Get("/", ListAPI)

	// FIXME: Add /authn endpoint that will always return status
	// code 200 for a randomly generated and logged password.

	// Start controller.
	ctlr.Start()
}
