#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "fan.h"

#include <avr/io.h>

void fan_tacho_configure(void) {
  // Configure PD4 as an input for tacho signal sensing.
  DDRD &= ~(1 << PIND4);

  // Configure a rising edge on PD4 (T0) as the clock source of the counter.
  TCCR0B |= (1 << CS02) | (1 << CS00);
}

uint8_t fan_tacho_read(void) { return TCNT0; }
void fan_tacho_reset(void) { TCNT0 = 0; }
