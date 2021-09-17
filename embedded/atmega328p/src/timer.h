#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>

#include "context.h"

void timer_configure(context_t *ctx, void (*on_event)(void));

ISR(TIMER1_COMPA_vect);

#endif
