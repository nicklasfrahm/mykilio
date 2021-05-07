#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "cursor.h"
#include "mushroom.h"
#include "twi.h"

// Read a register byte from an array of registers.
#define REG_RB(offset, registers, cursor) \
  registers[cursor.address - offset].bytes[cursor.index]
// Write a register byte from an array of registers.
#define REG_WB(offset, registers, cursor, data) \
  registers[cursor.address - offset].bytes[cursor.index] = data

// A cursor to store the information about the current TWI register operation.
cursor_t cursor = CURSOR_INITIALIZER;

// An array that contains all uint8-typed status registers.
mushroom_uint8_t status_regs[9];

// An array that contains all string-typed status registers.
mushroom_string_t status_regs_string[3];

// An array that contains all uint8-typed specification registers.
mushroom_uint8_t specification_regs[6];

// An array that contains all uint8-typed telemetry registers.
mushroom_uint8_t telemetry_regs[2];

// An array that contains all float-typed telemetry registers.
mushroom_float_t telemetry_regs_float[6];

// An array that contains all uint8-typed action registers.
mushroom_uint8_t action_regs[3];

void twi_receive(uint8_t data);
void twi_send(void);

int16_t main(void) {
  // Set pin 5 of PORTB for output.
  DDRB |= _BV(DDB5);

  // Configure TWI server.
  twi_server_configure(twi_receive, twi_send);
  twi_server_start(0x40);

  while (1) {
    if (cursor.address != REG_ADRPTR) {
      // Set pin 5 high to turn led on.
      PORTB |= _BV(PORTB5);
    } else {
      // Set pin 5 low to turn led off.
      PORTB &= ~_BV(PORTB5);
    }
  }

  return 0;
}

void twi_receive(uint8_t data) {
  if (cursor.idle) {
    // This is the start of a new TWI transaction, so we initialize our cursor.
    cursor.idle = CURSOR_BUSY;
    cursor.address = data;
    cursor.length = mushroom_register_len(cursor.address);
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

void twi_send(void) {
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
