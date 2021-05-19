VERSION		?= dev
REMOTE		:= github.com
NAMESPACE	:= nicklasfrahm/mykilio

GOBUILD		:= go build
BIN_DIR		:= ./bin
CMD_DIR		:= ./cmd
CERT_DIR	:= ./certs
TARGETS		:= $(addprefix $(BIN_DIR)/,$(patsubst $(CMD_DIR)/%/,%,$(dir $(wildcard $(CMD_DIR)/*/))))
SRCS		:= $(shell find -iname *.go)

.PHONY: all clean

all: $(TARGETS)

clean:
	-@rm -rvf $(BIN_DIR)/*
	-@rm -rvf $(CERT_DIR)/*

# Compile the given controller and ensure that it has a valid keypair.
$(TARGETS): $(BIN_DIR)/%: $(CERT_DIR)/%-public.pem $(CERT_DIR)/%-private.pem $(SRCS)
	CGO_ENABLED=0 $(GOBUILD) -o $@ -ldflags "-X main.apiGroup=$(@F)/$(VERSION)" cmd/$(@F)/main.go

# Generate a new private key to be used as part of the controller identity.
$(CERT_DIR)/%-private.pem:
	@mkdir -p $(@D)
	openssl ecparam -genkey -name prime256v1 -noout -out $@ 2> /dev/null

# Generate a new public key to be used as part of the controller identity.
$(CERT_DIR)/%-public.pem: $(CERT_DIR)/%-private.pem
	openssl ec -in $^ -pubout -out $@ 2> /dev/null
