#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "timer.h"

#include <avr/io.h>
#include <math.h>
#include <stdint.h>

static uint16_t prescaler = 1024;
static void (*timer_on_event)(void);

void timer_configure(context_t *ctx, void (*on_event)(void)) {
  // Configure event handler.
  timer_on_event = on_event;

  // Calculate the timer frequency.
  float timer_freq = ctx->cpu_frequency_hz / prescaler;
  float timer_ticks = ctx->timer_interval_ms / 1000.0;

  // Set the maximum value such that the timer overflow interrupt will be
  // triggered according to the configured interval.
  OCR1A = (uint16_t)round(timer_freq * timer_ticks);

  // Implement high-precision timing by configuring timer 1 to clear itself
  // every time the configured value is reached. Use the CPU clock divided by
  // 1024 as the clock source of the timer.
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

  // Enable interrupts when timer reaches configured value.
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  // Run user-defined event handler.
  timer_on_event();
}
