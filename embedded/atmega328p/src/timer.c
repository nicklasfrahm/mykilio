#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "timer.h"

#include <avr/io.h>

static void (*timer_on_event)(void);

void timer_configure(uint32_t cpu_freq, void (*on_event)(void)) {
  // Configure event handler.
  timer_on_event = on_event;

  // Implement high-precision timing by configuring timer 1 to clear itself
  // every time the configured value is reached. Use the CPU clock divided by
  // 1024 as the clock source of the timer.
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

  // Set the maximum value such that the timer overflow interrupt will be
  // triggered every second.
  uint16_t ticks = cpu_freq / 1024;
  OCR1A |= ticks;

  // Enable interrupts when timer reaches configured value.
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) { timer_on_event(); }
