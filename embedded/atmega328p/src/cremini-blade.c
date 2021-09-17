#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "context.h"
#include "cremini.h"
#include "cursor.h"
#include "fan.h"
#include "timer.h"
#include "twi.h"
#include "usart.h"

// Read a register byte from an array of registers.
#define REG_RB(offset, registers, cursor) \
  registers[cursor.address - offset].bytes[cursor.index]
// Write a register byte from an array of registers.
#define REG_WB(offset, registers, cursor, data) \
  registers[cursor.address - offset].bytes[cursor.index] = data
// Read a register value from an array of registers.
#define REG_RV(offset, registers, address) \
  registers[(address - offset) & 0xFF].value
// Write a register value from an array of registers.
#define REG_WV(offset, registers, address, data) \
  registers[(address - offset) & 0xFF].value = data

// Convert integer to boolean.
#define BOOL(b) b ? 1 : 0

// Convert boolean to text.
#define BOOL_TEXT(b) b ? "yes" : "no"

// An array that contains all uint8-typed status registers.
static volatile cremini_uint8_t status_regs[9];

// An array that contains all string-typed status registers.
static volatile cremini_string_t status_regs_string[3];

// An array that contains all uint8-typed specification registers.
static volatile cremini_uint8_t specification_regs[10];

// An array that contains all uint8-typed telemetry registers.
static volatile cremini_uint8_t telemetry_regs[2];

// An array that contains all float-typed telemetry registers.
static volatile cremini_float_t telemetry_regs_float[6];

// An array that contains all uint8-typed action registers.
static volatile cremini_uint8_t action_regs[3];

static void twi_receive(uint8_t data);
static void twi_send(void);
static void timer_every_second(void);
static void led_configure(void);
static void sbc_management_configure(void);
static uint8_t twi_address_configure(context_t *ctx);
static void led_apply(void);
static void sbc_power_apply(void);
static void sbc_status_apply(void);

static volatile cursor_t cursor = CURSOR_INITIALIZER;
static volatile uint8_t fan_hz = 0;
static volatile uint32_t uptime = 0;
static volatile uint8_t render = 1;
static volatile uint8_t sbccon = 0;
static volatile uint8_t sbcpon = 1;
static volatile uint8_t ledman = 1;
static double fan_rpm = 0;

// Configure application by creating a context.
context_t context = {
    .cpu_frequency_hz = 16e6,
    .twi_base_address = 0x10,
    .usart_baud = 1e6,
    .timer_interval_ms = 1000,
};

int main(void) {
  // USART must be configured as the first module to prevent random characters
  // appearing after a restart.
  usart_configure(&context);

  // TWI has to be configured as second module to allow the usage of the pins
  // for other functionality later.
  uint8_t twi_address = twi_address_configure(&context);
  twi_server_configure(twi_receive, twi_send);
  twi_server_start(twi_address);

  // Enable interrupts.
  sei();

  // Initialize modules.
  fan_tacho_configure();
  fan_control_configure();
  timer_configure(&context, timer_every_second);
  led_configure();
  sbc_management_configure();

  // Render status header.
  printf("TWI\tUPTIME\tFANDTY\tSBCCON\tSBCPON\tFANFDB\tFANRPM\n");

  while (1) {
    if (render) {
      led_apply();
      sbc_power_apply();
      sbc_status_apply();

      // Clear previous information.
      printf("\r");

      // Print TWI address and uptime.
      printf("0x%X\t%lu\t", twi_address, uptime);

      // Print fan duty.
      uint8_t duty = uptime % 255 * 5 % 255;
      fan_control_duty(duty);
      printf("%d%%  \t", (uint8_t)round((float)duty / (float)255 * 100.0));

      // Print SBC status information.
      printf("%s\t%s\t", BOOL_TEXT(sbccon), BOOL_TEXT(sbcpon));

      // Discard values greater than 10000 RPM.
      if (fan_hz <= 167) {
        // Calculate and print RPM based on frequency.
        fan_rpm *= 0.9;
        fan_rpm += 0.1 * fan_hz * 60.0;
      }
      printf("%d   \t%d   ", (uint8_t)round(fan_rpm / 60.0),
             (uint16_t)round(fan_rpm));

      // Mark rendering cycle as complete.
      render = 0;
    }
  }

  return 0;
}

static void twi_receive(uint8_t data) {
  if (cursor.idle) {
    // This is the start of a new TWI transaction, so we initialize our cursor.
    cursor.idle = CURSOR_BUSY;
    cursor.address = data;
    cursor.length = cremini_register_len(cursor.address);
    return;
  }

  if (cursor.address == REG_ADRPTR) {
    // Write the register address pointer for a read operation.
    cursor.address = data;
  } else if (IS_ACTION(cursor.address)) {
    REG_WB(REG_ACTION, action_regs, cursor, data);
  } else if (IS_SPECIFICATION(cursor.address)) {
    REG_WB(REG_SPECIFICATION, specification_regs, cursor, data);
  }

  cursor_update(&cursor);
}

static void twi_send(void) {
  // Update the cursor.
  if (cursor.idle) {
    cursor.idle = CURSOR_BUSY;
  }

  if (cursor.address == REG_ADRPTR) {
    // This is kind of pointless, because it would just return
    // its own address, but let's keep it for the sake of completeness.
    twi_server_send(cursor.address);
  } else if (IS_ACTION(cursor.address)) {
    twi_server_send(REG_RB(REG_ACTION, action_regs, cursor));
  } else if (IS_TELEMETRY_FLOAT(cursor.address)) {
    twi_server_send(REG_RB(REG_TELEMETRY_FLOAT, telemetry_regs_float, cursor));
  } else if (IS_TELEMETRY(cursor.address)) {
    twi_server_send(REG_RB(REG_TELEMETRY, telemetry_regs, cursor));
  } else if (IS_SPECIFICATION(cursor.address)) {
    twi_server_send(REG_RB(REG_SPECIFICATION, specification_regs, cursor));
  } else if (IS_STATUS_STRING(cursor.address)) {
    twi_server_send(REG_RB(REG_STATUS_STRING, status_regs_string, cursor));
  } else if (IS_STATUS(cursor.address)) {
    twi_server_send(REG_RB(REG_STATUS, status_regs, cursor));
  } else {
    twi_server_send(0x00);
  }

  cursor_update(&cursor);
}

static void led_apply(void) {
  if (ledman) {
    // Turn LED on.
    PORTD |= _BV(PORTD5);
  } else {
    // Turn LED off.
    PORTD &= ~_BV(PORTD5);
  }
}

static void sbc_management_configure(void) {
  // Configure SBC presence detection input.
  DDRD &= ~_BV(DDD7);
  // Configure SBC power-on and soft-on control outputs.
  DDRB |= _BV(DDB0) | _BV(DDB1);
}

static void sbc_power_apply(void) {
  if (sbcpon) {
    PORTB |= _BV(PORTB0);
  } else {
    PORTB &= ~_BV(PORTB0);
  }
}

static void sbc_status_apply(void) {
  // The SBCCON signal is low-active as the signal is pulled low when the
  // microcontroller is connected.
  sbccon = PIND & _BV(PIND7) ? 0 : 1;
}

static void led_configure(void) {
  // Configure user LED.
  DDRD |= _BV(DDD5);
}

static uint8_t twi_address_configure(context_t *ctx) {
  // Configure TWI address input.
  DDRB &= ~(_BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2));

  return ctx->twi_base_address | ((PINB >> 2) & 0x0F);
}

static void timer_every_second(void) {
  // Track uptime.
  uptime++;

  // Schedule new rendering.
  render = 1;

  // Read fan speed.
  fan_hz = fan_tacho_read();
}
