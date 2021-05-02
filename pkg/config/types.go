package config

import (
	"crypto"
)

const (
	// IdentityFormatECPrime256V1PEM is the identity format for a PEM encoded EC prime-256v1 curve keypair.
	IdentityFormatECPrime256V1PEM = "EC_PRIME256V1_PEM"
	// IdentityFormatRSAPEM is the identity format for a PEM encoded RSA keypair.
	IdentityFormatRSAPEM = "RSA_PEM"
	// VersionDevelopment is the tag of the development version.
	VersionDevelopment = "dev"
)

// Config is the application configuration.
type Config struct {
	App                       string
	Hostname                  string
	LocalPort                 string
	ServiceDomain             string
	ServiceIdentity           ServiceIdentity
	ServiceIdentityPrivateKey crypto.PrivateKey
	ServiceURI                string
	StartedAt                 string
	Version                   string
}

// ServiceIdentity is the identity of a service.
type ServiceIdentity struct {
	Format     string `json:"format"`
	PublickKey string `json:"public_key"`
}
