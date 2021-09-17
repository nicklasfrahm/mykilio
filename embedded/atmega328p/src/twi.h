#ifndef TWI_H
#define TWI_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

void twi_server_configure(void (*on_receive)(uint8_t), void (*on_send)(void));
void twi_server_start(uint8_t address);
void twi_server_stop(void);

inline void __attribute__((always_inline)) twi_server_send(uint8_t data) {
  TWDR = data;
}

ISR(TWI_vect);

#endif
