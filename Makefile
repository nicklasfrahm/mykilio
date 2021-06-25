VERSION		?= dev
REMOTE		:= github.com
REGISTRY	:= ghcr.io
NAMESPACE	:= nicklasfrahm/mykilio
CONTROLLER	?= core.mykil.io

GOBUILD		:= go build
BIN_DIR		:= ./bin
CMD_DIR		:= ./cmd
CERT_DIR	:= ./certs
TARGETS		:= $(addprefix $(BIN_DIR)/,$(patsubst $(CMD_DIR)/%/,%,$(dir $(wildcard $(CMD_DIR)/*/))))
SRCS		:= $(shell find -iname *.go)

# Configure the desired OpenSSL curve. For compatibility reasons
# the curves outlined in RFC7518, section 3.1 are recommended:
# - P-256: secp256r1
# - P-384: secp384r1
# - P-521: secp521r1
# To obtain a full list of all curves run: openssl ecparam -list_curves
CURVE		?= secp521r1

.PHONY: all run clean container
.PRECIOUS: $(CERT_DIR)/%/curve.openssl $(CERT_DIR)/%/private.pem

all: $(TARGETS)

run: $(BIN_DIR)/$(CONTROLLER)
	$^

clean:
	-@rm -rvf $(BIN_DIR)/*
	-@rm -rvf $(CERT_DIR)/*
	-@rm -vf test

container: deployments/container/Dockerfile
	docker build \
		--build-arg "CONTROLLER=$(CONTROLLER)" \
		--build-arg "VERSION=$(VERSION)" \
		--tag $(REGISTRY)/$(NAMESPACE)-$(subst .mykil.io,,$(CONTROLLER)):$(VERSION) \
		--file $^ .

# Creates a cookie file that prevents running tests again if the
# sources didn't change.
test: $(SRCS)
	@touch test
	go test ./... -cover -v

# Compile the given controller and ensure that it has a valid keypair.
$(TARGETS): $(BIN_DIR)/%: $(SRCS) $(CERT_DIR)/%/curve.openssl $(CERT_DIR)/%/private.pem $(CERT_DIR)/%/public.pem
	CGO_ENABLED=0 $(GOBUILD) -o $@ -ldflags "-X main.apiGroup=$(@F) -X main.version=$(VERSION)" cmd/$(@F)/main.go

# Save curve type.
$(CERT_DIR)/%/curve.openssl:
	@mkdir -p $(@D)
	echo $(CURVE) > $(@D)/curve.openssl

# Generate a new private key to be used as part of the controller identity.
$(CERT_DIR)/%/private.pem: $(CERT_DIR)/%/curve.openssl
	openssl ecparam -genkey -name $(CURVE) -noout -out $@ 2> /dev/null

# Generate a new public key to be used as part of the controller identity.
$(CERT_DIR)/%/public.pem: $(CERT_DIR)/%/private.pem
	openssl ec -in $^ -pubout -out $@ 2> /dev/null
