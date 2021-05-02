package middleware

import (
	"net/http"
	"strings"

	"github.com/gofiber/fiber/v2"
)

// RedirectSlashes slashes redirects routes with a trailing slash
// to the same route without the trailing slash.
func RedirectSlashes() func(*fiber.Ctx) error {
	return func(c *fiber.Ctx) error {
		path := c.Path()

		if len(path) > 1 && path[len(path)-1] == '/' {
			segments := strings.Split(c.OriginalURL(), "?")
			query := ""
			if len(segments) == 2 {
				query = "?" + segments[1]
			}
			return c.Redirect(path[:len(path)-1]+query, http.StatusMovedPermanently)
		}

		return c.Next()
	}
}
