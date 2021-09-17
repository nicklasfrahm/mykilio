#ifndef FAN_H
#define FAN_H

#include <avr/io.h>
#include <stdint.h>

void fan_tacho_configure(void);

inline uint8_t __attribute__((always_inline)) fan_tacho_read(void) {
  // Return the number of pulses and reset the counter.
  uint8_t pulses = TCNT0;
  TCNT0 = 0;
  return pulses;
}

void fan_control_configure(void);

void fan_control_duty(uint8_t duty);

#endif
