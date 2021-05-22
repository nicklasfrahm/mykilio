package hooks

import (
	"crypto/rand"
	"encoding/base64"

	"github.com/gofiber/fiber/v2"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mykilio/pkg/controller"
	"github.com/nicklasfrahm/mykilio/pkg/util"
)

// RandomString creates a random base64 encoded string.
func RandomString(length int) string {
	bytes := make([]byte, length*3/4)
	_, err := rand.Read(bytes)
	if err != nil {
		log.Fatal().Msgf("Failed to generate random bytes: %v", err)
	}

	return base64.StdEncoding.EncodeToString(bytes)
}

type ClientCredentials struct {
	ClientID string `json:"clientID"`
	Username string `json:"username"`
	Password string `json:"password"`
}

// V1Alpha1 returns the
func V1Alpha1(ctrl *controller.Controller) string {
	apiVersion := "v1alpha1"

	username := RandomString(32)
	password := RandomString(32)

	// Print credentials.
	log.Info().Msgf("Username: %s", username)
	log.Info().Msgf("Password: %s", password)

	ctrl.HTTPServer.Post("/"+apiVersion+"/authn", func(c *fiber.Ctx) error {
		creds := new(ClientCredentials)
		if err := c.BodyParser(creds); err != nil {
			return err
		}

		if creds.Username == "" || creds.Password == "" {
			return util.SemanticError(c, &util.ReasonCredentialsMissing)
		}

		if creds.Username != username || creds.Password != password {
			return util.SemanticError(c, &util.ReasonCredentialsInvalid)
		}

		return c.Status(200).Send([]byte("OK"))
	})

	return apiVersion
}
