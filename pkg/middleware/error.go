package middleware

import (
	"encoding/json"
	"net/http"
	"reflect"

	"github.com/go-playground/validator/v10"
	"github.com/gofiber/fiber/v2"
	"github.com/rs/zerolog/log"

	"github.com/nicklasfrahm/mykilio/pkg/util"
)

// Error returns a middleware to handler errors during requests.
func Error() func(*fiber.Ctx, error) error {
	return func(c *fiber.Ctx, err error) error {
		// Handle JSON parsing errors.
		if _, ok := err.(*json.SyntaxError); ok {
			return util.SemanticError(c, &util.ReasonJSONInvalid)
		}

		// Handle validation errors.
		if _, ok := err.(validator.ValidationErrors); ok {
			return util.SemanticError(c, &util.ReasonValidationFailed)
		}

		// Handle custom fiber errors.
		if ferr, ok := err.(*fiber.Error); ok {
			reason := util.NewReason(http.StatusInternalServerError, ferr.Message)
			log.Error().Msg(reason.Reason)
			return util.SemanticError(c, &reason)
		}

		// Display error and error type if the error is unexpected.
		log.Error().Err(err).Str("error_type", reflect.TypeOf(err).String()).Msg(util.ReasonUnexpectedError.Reason)
		return util.SemanticError(c, &util.ReasonUnexpectedError)
	}
}
