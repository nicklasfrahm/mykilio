#ifndef ADC_H
#define ADC_H

#include <avr/interrupt.h>
#include <stdint.h>

void adc_configure(void);

void adc_channel_enable(uint8_t channel);

void adc_channel_disable(uint8_t channel);

uint16_t adc_channel_read(uint8_t channel);

inline void __attribute__((always_inline)) adc_start(void) {
  ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect);

#endif
