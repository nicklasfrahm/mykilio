#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "twi/twi_server.h"

uint8_t led_state = 0;
uint8_t twi_register = 0;

void receive(uint8_t data) {
  if (twi_register != 0) {
    led_state = data;
    twi_register = 0;
  }

  if (twi_register == 0) {
    twi_register = data;
  }
}

void send(void) {
  if (twi_register != 0) {
    twi_server_send(led_state);
  }
}

int16_t main(void) {
  // Set pin 5 of PORTB for output.
  DDRB |= _BV(DDB5);

  // Configure TWI server.
  twi_server_configure(receive, send);
  twi_server_start(0x40);

  while (1) {
    if (led_state) {
      // Set pin 5 high to turn led on.
      PORTB |= _BV(PORTB5);
    } else {
      // Set pin 5 low to turn led off.
      PORTB &= ~_BV(PORTB5);
    }
  }

  return 0;
}
