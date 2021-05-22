package controller

import (
	mqtt "github.com/eclipse/paho.mqtt.golang"

	"github.com/gofiber/fiber/v2"
)

const (
	TransportMQTT = "MQTT"
	TransportHTTP = "HTTP"
)

// Install installs a service handler into the

type EventContext struct {
	Source string
	HTTP   *fiber.Ctx
	MQTT   mqtt.Message
}

type EventHandler func(*EventContext)

type Resource interface {
}

// func (src string, ctx )
