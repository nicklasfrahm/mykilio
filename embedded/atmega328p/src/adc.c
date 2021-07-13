#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "adc.h"

#include <avr/io.h>
#include <stdint.h>

#define CHANNELS 9

static volatile uint8_t selected = 0;
static volatile uint16_t enabled = 0;
static volatile uint16_t data[CHANNELS] = {0};

void adc_configure(void) {
  // Enable the ADC.
  ADCSRA |= _BV(ADEN);
  // To achieve the maximum resolution of 10bits, the ADC clock must be between
  // 50kHz and 200kHz. If the CPU frequency is 16MHz, like it typically is,
  // setting the prescaler to 128 will configure the ADC frequency to be 16MHz /
  // 128, so 125kHz.
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  // Enable the ADC interrupt.
  ADCSRA |= _BV(ADIE);
  // Use the internal 1.1V reference.
  ADMUX |= _BV(REFS1) | _BV(REFS0);
}

void adc_channel_enable(uint8_t channel) {
  // Set the bit of the ADC channel.
  enabled |= _BV(channel);
}

void adc_channel_disable(uint8_t channel) {
  // Reset the bit of the ADC channel.
  enabled &= ~_BV(channel);
}

uint16_t adc_channel_read(uint8_t channel) {
  // Read data from ADC buffer.
  return data[channel];
}

ISR(ADC_vect) {
  // Write current result to data register.
  data[selected] = ADC;

  // Find the next channel.
  uint8_t i = selected;
  do {
    // Select next channel.
    ++i;

    // Perform boundary check.
    if (i == CHANNELS) {
      i = 0;
    }

    // Check if the channel is enabled.
    if (enabled & _BV(i)) {
      selected = i;
      // Set channel for next sample.
      ADMUX |= (0x0F & selected);
    }
  } while (i != selected);

  // Start sampling process.
  adc_start();
}
