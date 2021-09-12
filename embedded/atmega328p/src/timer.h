#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>
#include <stdint.h>

void timer_configure(uint32_t cpu_freq, void (*on_event)(void));

ISR(TIMER1_COMPA_vect);

#endif
