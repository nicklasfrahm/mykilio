package controller

import (
	"crypto"
	"crypto/x509"
	"encoding/pem"
	"errors"
	"io/ioutil"
	"os"
	"path"
	"strings"

	"github.com/rs/zerolog/log"
)

// SignatureScheme is a datatype roughly based on this section of RFC7518:
// https://datatracker.ietf.org/doc/html/rfc7518#section-3.1
type SignatureScheme struct {
	// Algorithm can be either one of EdDSA, ECDSA or RSA.
	Algorithm string `json:"algorithm"`
	// Curve is the OpenSSL curve identifier. You can get
	// a list by running: openssl ecparam -list_curves
	Curve string `json:"curve,omitempty"`
	// Hash can be either SHA-256, SHA-384 or SHA-512.
	Hash string `json:"hash"`
}

func (s SignatureScheme) String() string {
	if s.Curve != "" {
		return s.Algorithm + "/" + s.Curve + "/" + s.Hash
	}
	return s.Algorithm + "/" + s.Hash
}

var (
	// SchemeES256 is the equivalent of ES256 mentioned in RFC7518.
	SchemeES256 SignatureScheme = SignatureScheme{"ECDSA", "prime256v1", "SHA-256"}
	// SchemeES384 is the equivalent of ES384 mentioned in RFC7518.
	SchemeES384 SignatureScheme = SignatureScheme{"ECDSA", "secp384r1", "SHA-384"}
	// SchemeES512 is the equivalent of ES512 mentioned in RFC7518.
	SchemeES512 SignatureScheme = SignatureScheme{"ECDSA", "secp521r1", "SHA-512"}
)

func NewSignatureScheme(curve string) SignatureScheme {
	switch curve {
	case SchemeES256.Curve:
		return SchemeES256
	case SchemeES384.Curve:
		return SchemeES384
	case SchemeES512.Curve:
		return SchemeES512
	default:
		log.Fatal().Msgf("Failed to select signature scheme: %v", errors.New("unsupported curve or algorithm"))
	}

	return SignatureScheme{}
}

type Identity struct {
	PublicKeyPEM    string            `json:"publicKeyPEM"`
	PrivateKey      crypto.PrivateKey `json:"-"`
	SignatureScheme SignatureScheme   `json:"signatureScheme"`
}

// NewIdentity loads the keypair and the curve information.
func NewIdentity() *Identity {
	certDir := os.Getenv("CERT_DIR")
	curveNameFile := path.Join(certDir, "curve.openssl")
	privateKeyFile := path.Join(certDir, "private.pem")
	publicKeyFile := path.Join(certDir, "public.pem")

	// Read curve type information. Currently we do NOT support RSA.
	curveNameBytes, err := ioutil.ReadFile(curveNameFile)
	if err != nil {
		log.Fatal().Msgf("Failed to read curve name: %v", err)
	}

	// Read PEM encoded private key file.
	privateKeyBytes, err := ioutil.ReadFile(privateKeyFile)
	if err != nil {
		log.Fatal().Msgf("Failed to read private key: %v", err)
	}
	privateKeyBlock, _ := pem.Decode(privateKeyBytes)

	// Read PEM encoded public key file.
	publicKeyBytes, err := ioutil.ReadFile(publicKeyFile)
	if err != nil {
		log.Fatal().Msgf("Failed to read public key: %v", err)
	}
	publicKeyBlock, _ := pem.Decode(publicKeyBytes)

	// Parse public key to verify correctness.
	_, err = x509.ParsePKIXPublicKey(publicKeyBlock.Bytes)
	if err != nil {
		log.Fatal().Msgf("Failed to parse public key: %v", err)
	}

	// Parse the private key.
	ecPrivateKey, err := x509.ParseECPrivateKey(privateKeyBlock.Bytes)
	if err != nil {
		log.Fatal().Msgf("Failed to parse private key: %v")
	}

	// Create identity.
	return &Identity{
		PrivateKey:      ecPrivateKey,
		PublicKeyPEM:    strings.TrimSpace(string(publicKeyBytes)),
		SignatureScheme: NewSignatureScheme(strings.TrimSpace(string(curveNameBytes))),
	}
}
