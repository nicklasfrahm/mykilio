package middleware

import (
	"github.com/gofiber/fiber/v2"
)

// ContentTypeJSON forces the "content-type" header to use the JSON MIME type.
func ContentTypeJSON() func(*fiber.Ctx) error {
	return ContentType("application/json; charset=utf-8")
}

// ContentType is a middleware to force the "content-type" header
// for any outgoing response.
func ContentType(contentType string) func(*fiber.Ctx) error {
	return func(c *fiber.Ctx) error {
		c.Request().Header.Set("content-type", contentType)
		return c.Next()
	}
}
