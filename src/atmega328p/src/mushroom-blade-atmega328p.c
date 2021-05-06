#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "mushroom.h"
#include "twi_server.h"

mushroom_cursor_t cursor = MUSHROOM_CURSOR_INITIALIZER;

void receive(uint8_t data) {
  // Configure the address to be written to.
  if (cursor.address == REG_NONE) {
    cursor.address = data;
    cursor.length = mushroom_register_len(cursor.address);
  }

  // TODO: Stream data in.
}

void send(void) {
  // Return zero data if no register is selected for reading.
  if (cursor.address == REG_NONE) {
    twi_server_send(0x00);
    return;
  }

  // TODO: Stream data out.

  if (cursor.index == cursor.length) {
    mushroom_cursor_init(&cursor);
  }
}

int16_t main(void) {
  // Set pin 5 of PORTB for output.
  DDRB |= _BV(DDB5);

  // Configure TWI server.
  twi_server_configure(receive, send);
  twi_server_start(0x40);

  while (1) {
    if (cursor.address != REG_NONE) {
      // Set pin 5 high to turn led on.
      PORTB |= _BV(PORTB5);
    } else {
      // Set pin 5 low to turn led off.
      PORTB &= ~_BV(PORTB5);
    }
  }

  return 0;
}
