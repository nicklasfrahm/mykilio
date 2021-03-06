package main

import (
	"net/http"

	"github.com/gofiber/fiber/v2"

	"github.com/nicklasfrahm/mykilio/pkg/controller"
	"github.com/nicklasfrahm/mykilio/pkg/util"
)

type APIInfo struct {
	APIGroup string `json:"apiGroup"`
	Version  string `json:"version"`
}

func main() {
	// Create new controller for the specified apiGroup.
	ctrl := controller.New()

	// Mount controller-specific routes and handlers.
	ctrl.HTTPServer.Get("/", func(c *fiber.Ctx) error {
		return c.Status(http.StatusOK).JSON(util.DataBody{
			Data: APIInfo{
				APIGroup: ctrl.APIGroup,
				Version:  ctrl.Version,
			},
		})
	})

	// Start controller.
	ctrl.Start()
}
