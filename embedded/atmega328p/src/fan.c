#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "fan.h"

#include <avr/io.h>

void fan_tacho_configure(void) {
  // Use a falling edge on PD4 (T0) as the clock source of the counter.
  DDRD &= ~_BV(DDD4);
  TCCR0B |= _BV(CS02) | _BV(CS01);
}

static void fan_control_pwm_configure(void) {
  // Ensure that the 12V signal is always high.
  DDRD |= _BV(DDD6);
  PORTD |= _BV(PORTD6);

  // Configure counter 2 to generate a phase-correct PWM signal on PD3 (COM2B).
  DDRD |= _BV(DDD3);
  TCCR2A |= _BV(WGM20) | _BV(COM2B1);

  // Use the CPU clock directly as the input of the counter to generate a signal
  // with a frequency of 31.4 kHz.
  TCCR2B |= _BV(CS20);
}

void fan_control_configure(void) {
  // Configure the fan control.
  fan_control_pwm_configure();
}

void fan_control_duty(uint8_t duty) {
  // TODO: Set duty cycle based on fan control mode.

  // Set the duty cycle.
  OCR2B = duty;
}
