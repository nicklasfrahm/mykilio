package controller

import (
	"io/ioutil"
	"net/url"
	"os"
	"strings"

	"github.com/rs/zerolog/log"
)

// Getenv attempts to get the value of an environment variable from
// a file path. If the value is not a file path, it will use the value.
func Getenv(variable string) string {
	// Get value of environment variable.
	value := os.Getenv(variable)

	// Attempt to parse value of environment variable as file.
	fileURL, err := url.Parse(value)
	if err != nil || fileURL.Scheme != "file" {
		return value
	}

	// Attempt to read value from file.
	bytes, err := ioutil.ReadFile(fileURL.Path)
	if err != nil {
		log.Fatal().Err(err).Msg("Reading environment file failed: " + value)
	}

	fileValue := strings.TrimSpace(string(bytes))
	if fileValue == "" {
		log.Fatal().Msg("Empty environment file: " + value)
	}

	return fileValue
}

// GetenvMandatory loads a mandatory environment variable and returns
// its value or terminates the process if the variable is unset.
func GetenvMandatory(variable string) string {
	value := Getenv(variable)
	if value == "" {
		log.Fatal().Msg("Empty environment variable: " + variable)
	}

	return value
}

// GetenvOptional loads an optional environment variable and returns
// its value or the provided default value if the varaible is not set.
func GetenvOptional(variable string, defaultValue string) string {
	value := Getenv(variable)
	if value == "" {
		value = defaultValue
	}

	return value
}
