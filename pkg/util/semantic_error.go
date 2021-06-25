package util

import (
	"net/http"

	"github.com/gofiber/fiber/v2"
)

var (
	// ReasonJSONInvalid occurs when the JSON payload cannot be parsed.
	ReasonJSONInvalid = NewReason(400, "JSON Invalid")
	// ReasonValidationFailed occurs when the validation of a model fails.
	ReasonValidationFailed = NewReason(400, "Validation Failed")
	// ReasonCredentialsMissing occurs if the client does not provide credentials.
	ReasonCredentialsMissing = NewReason(401, "Credentials Missing")
	// ReasonCredentialsInvalid occurs if the client provides invalid credentials.
	ReasonCredentialsInvalid = NewReason(401, "Credentials Invalid")
	// ReasonEndpointUnsupported occurs when the endpoint is not part of the specification.
	ReasonEndpointUnsupported = NewReason(404, "Endpoint Unsupported")
	// ReasonEntityUnknown occurs when an entity does not exist.
	ReasonEntityUnknown = NewReason(404, "Entity Unknown")
	// ReasonEntityExists occurs when a conflicting entity already exists.
	ReasonEntityExists = NewReason(409, "Entity Exists")
	// ReasonUnexpectedError occurs when an unexpected error occurs.
	ReasonUnexpectedError = NewReason(500, "Unexpected Error")
)

// Reason describes why an error occurred.
type Reason struct {
	Title  string `json:"title"`
	Status int    `json:"status"`
	Reason string `json:"reason"`
}

// NewReason creates a new reason.
func NewReason(status int, reason string) Reason {
	return Reason{
		Title:  http.StatusText(status),
		Status: status,
		Reason: reason,
	}
}

// Pagination describes the pagination parameters used for a query.
type Link struct {
	Rel  string `json:"rel,omitempty"`
	Href string `json:"href"`
}

// DataBody describes the HTTP response body when data is sent to the client.
type DataBody struct {
	Data  interface{} `json:"data"`
	Links []Link      `json:"links,omitempty"`
}

// ErrorBody describes the HTTP response body when an error is sent to the client.
type ErrorBody struct {
	Error Reason `json:"error"`
}

// SemanticError sends a semantic HTTP error containing a reason
// for the error condition by using the fiber context.
func SemanticError(c *fiber.Ctx, reason *Reason) error {
	// Send reason.
	return c.Status(reason.Status).JSON(ErrorBody{
		Error: *reason,
	})
}
