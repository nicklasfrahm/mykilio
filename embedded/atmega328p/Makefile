CC			:= -avr-gcc
CCFLAGS		:= -g -mmcu=atmega328p -Os -std=c11 -pedantic-errors -Wall -Wextra -Werror
LDFLAGS		:= -L/usr/lib/avr/include/ -Wl,-u,vfprintf -lprintf_min -lm
OC			:= -avr-objcopy
BUILD		:= ./build
OBJ_DIR		:= $(BUILD)/objects
BIN_DIR		:= $(BUILD)/bin
IMG_DIR		:= $(BUILD)/img
LIB_DIR		:= $(BUILD)/lib
VENDOR_DIR  := ./vendor
TARGET		:= mushroom-blade-atmega328p
BOOTLOADER  := optiboot_atmega328
PROGRAMMER  := arduino_isp
PORT		?= /dev/ttyUSB0
BAUD        ?= 115200
SRC			:= $(wildcard ../lib/*.c) $(wildcard src/*.c)
LIB			:= -I../lib/
OBJ			:= $(SRC:%.c=$(OBJ_DIR)/%.o)
DEP			:= $(OBJ:.o=.d)

all: $(IMG_DIR)/$(TARGET).hex

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(LIB) -c $< -MMD -o $@

$(BIN_DIR)/$(TARGET).bin: $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

$(IMG_DIR)/$(TARGET).hex: $(BIN_DIR)/$(TARGET).bin
	@mkdir -p $(@D)
	$(OC) -j .text -j .data -O ihex $^ $@

-include $(DEP)

.PHONY: all build clean flash burn isp infos

# Clean up old build outputs.
clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(BIN_DIR)/*
	-@rm -rvf $(IMG_DIR)/*
	-@rm -rvf $(LIB_DIR)/*

# Flash firmware via USB to serial to microcontroller. This requires a
# bootloader to be installed on the microcontroller, preferrably Optiboot.
flash: $(IMG_DIR)/$(TARGET).hex
	avrdude -P $(PORT) -b $(BAUD) -c arduino -p atmega328p -v -D -U $^

# Burn the Optiboot bootloader as described here with an Arduino as ISP.
# Reference: https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader/all
burn: $(VENDOR_DIR)/$(BOOTLOADER).hex
# Set fuse bits.
# Reference:
# - https://www.allaboutcircuits.com/projects/atmega328p-fuse-bits-and-an-external-crystal-oscillator/
# - https://www.engbedded.com/fusecalc/
	avrdude -P $(PORT) -b 19200 -c avrisp -p m328p -v -e -U lfuse:w:0xFF:m -U hfuse:w:0xD6:m -U efuse:w:0xFD:m
# Flash bootloader.
	avrdude -P $(PORT) -b 19200 -c avrisp -p m328p -v -e -U flash:w:$(VENDOR_DIR)/$(BOOTLOADER).hex -U lock:w:0x0F:m

# Create an in-system programmer (ISP) using a spare ATmega328P microcontroller.
isp: $(VENDOR_DIR)/$(PROGRAMMER).hex
	avrdude -P $(PORT) -b $(BAUD) -c arduino -p atmega328p -v -D -U $^

# Display information about the project strucuture.
info:
	@echo "[*] Image dir:       ${IMG_DIR}       "
	@echo "[*] Binary dir:      ${BIN_DIR}       "
	@echo "[*] Object dir:      ${OBJ_DIR}       "
	@echo "[*] Library dir:     ${LIB_DIR}       "
	@echo "[*] Vendor dir:      ${VENDOR_DIR}    "
	@echo "[*] Sources:         ${SRC}           "
	@echo "[*] Objects:         ${OBJ}       "
	@echo "[*] Dependencies:    ${DEP}  "
