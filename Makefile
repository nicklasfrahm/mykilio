VERSION		?= dev
REMOTE		:= github.com
NAMESPACE	:= nicklasfrahm/mycelium

GOBUILD		:= go build
BIN_DIR		:= ./bin
CERT_DIR	:= ./certs

all: $(CERT_DIR)/service_id_public.pem $(BIN_DIR)/device-registry

# Compile binaries.
$(BIN_DIR)/%:
	@mkdir -p $(@D)
	$(GOBUILD) -o $@ -ldflags "-X $(REMOTE)/$(NAMESPACE)/pkg/config.version=$(VERSION) -X $(REMOTE)/$(NAMESPACE)/pkg/config.app=$(@F)" cmd/$(@F)/main.go

# Generate a new private key to be used as part of the service identity.
$(CERT_DIR)/service_id_private.pem:
	@mkdir -p $(@D)
	openssl ecparam -genkey -name prime256v1 -noout -out $@ 2> /dev/null

# Generate a new public key to be used as part of the service identity.
$(CERT_DIR)/service_id_public.pem: $(CERT_DIR)/service_id_private.pem
	openssl ec -in $^ -pubout -out $@ 2> /dev/null

.PHONY: all clean

# Clean up old build outputs.
clean:
	-@rm -rvf $(BIN_DIR)/*
	-@rm -rvf $(CERT_DIR)/*
