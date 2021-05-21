package model

import (
	"github.com/nicklasfrahm/mykilio/pkg/config"
)

// Health provides information about the application server health.
type Health struct {
	Hostname  string                 `json:"hostname"`
	Version   string                 `json:"version"`
	StartedAt string                 `json:"started_at"`
	Identity  config.ServiceIdentity `json:"identity"`
}

// HealthResponse is the server response with the health resource.
type HealthResponse struct {
	Data Health `json:"datas"`
}
