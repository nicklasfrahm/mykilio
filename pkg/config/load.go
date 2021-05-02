package config

import (
	"crypto"
	"crypto/ecdsa"
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
	"io/ioutil"
	"os"
	"path"
	"strings"
	"time"

	"github.com/joho/godotenv"
	"github.com/rs/zerolog/log"
)

var (
	app     = "mock"
	version = "dev"
)

var config *Config

// Load loads the configuration.
func Load() *Config {
	if config != nil {
		return config
	}

	err := godotenv.Load()
	if err != nil {
		log.Info().Msg("File not found: .env")
	}

	startedAt := time.Now().UTC().Format(time.RFC3339)
	serviceDomain := GetenvMandatory("SERVICE_DOMAIN")
	localPort := GetenvOptional("PORT", "8080")
	certDir := path.Clean(GetenvOptional("CERT_DIR", "./certs"))

	// Configure service URI based on application environment.
	serviceURI := "https://" + app + "." + serviceDomain
	if version == VersionDevelopment {
		serviceURI = "http://" + app + ".localtest.me:" + localPort
	}

	// Obtain the network hostname of the machine.
	hostname, err := os.Hostname()
	if err != nil {
		log.Fatal().Err(err).Msg("Failed to read hostname")
	}

	// Load the service identity.
	serviceIdentityPrivateKey, serviceIdentity := loadServiceIdentity(certDir)

	config = &Config{
		App:                       app,
		Hostname:                  hostname,
		LocalPort:                 localPort,
		ServiceDomain:             serviceDomain,
		ServiceIdentity:           serviceIdentity,
		ServiceIdentityPrivateKey: serviceIdentityPrivateKey,
		ServiceURI:                serviceURI,
		StartedAt:                 startedAt,
		Version:                   version,
	}

	return config
}

func loadServiceIdentity(certDir string) (crypto.PrivateKey, ServiceIdentity) {
	privateKeyFile := path.Join(certDir, "service_id_private.pem")
	publicKeyFile := path.Join(certDir, "service_id_public.pem")

	// Load PEM encoded private key file.
	privateKeyBytes, err := ioutil.ReadFile(privateKeyFile)
	if err != nil {
		log.Fatal().Err(err).Msg("Failed to load service identity private key: " + privateKeyFile)
	}
	privateKeyBlock, _ := pem.Decode(privateKeyBytes)

	// Load PEM encoded public key file.
	publicKeyBytes, err := ioutil.ReadFile(publicKeyFile)
	if err != nil {
		log.Fatal().Err(err).Msg("Failed to load service identity public key: " + publicKeyFile)
	}
	publicKeyBlock, _ := pem.Decode(publicKeyBytes)
	publicKey, err := x509.ParsePKIXPublicKey(publicKeyBlock.Bytes)
	if err != nil {
		log.Fatal().Err(err).Msg("Failed to parse service identity public key: " + publicKeyFile)
	}

	// Parse EC private key.
	if privateKeyBlock.Type == "EC PRIVATE KEY" {
		ecPrivateKey, err := x509.ParseECPrivateKey(privateKeyBlock.Bytes)
		if err != nil {
			log.Fatal().Err(err).Msg("Failed to parse service identity private key: " + privateKeyFile)
		}
		ecPrivateKey.PublicKey = *publicKey.(*ecdsa.PublicKey)
		return ecPrivateKey, ServiceIdentity{
			PublickKey: strings.TrimSpace(string(publicKeyBytes)),
			Format:     IdentityFormatECPrime256V1PEM,
		}
	}

	rsaPrivateKey, err := x509.ParsePKCS1PrivateKey(privateKeyBlock.Bytes)
	if err != nil {
		log.Fatal().Err(err).Msg("Failed to parse service identity private key: " + privateKeyFile)
	}
	rsaPrivateKey.PublicKey = *publicKey.(*rsa.PublicKey)
	return rsaPrivateKey, ServiceIdentity{
		PublickKey: strings.TrimSpace(string(publicKeyBytes)),
		Format:     IdentityFormatRSAPEM,
	}
}
