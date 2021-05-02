package middleware

import (
	"github.com/gofiber/fiber/v2"

	"github.com/nicklasfrahm/mycelium/pkg/util"
)

// NotFound returns a middlware for endpoints that do not exist.
func NotFound() func(*fiber.Ctx) error {
	return func(c *fiber.Ctx) error {
		return util.SemanticError(c, &util.ReasonEndpointUnsupported)
	}
}
