package middleware

import (
	"github.com/gofiber/fiber/v2"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mykilio/pkg/util"
)

// NotFound returns a middlware for endpoints that do not exist.
func NotFound() func(*fiber.Ctx) error {
	return func(c *fiber.Ctx) error {
		log.Warn().Msgf("404 %s", string(c.Request().RequestURI()))
		return util.SemanticError(c, &util.ReasonEndpointUnsupported)
	}
}
