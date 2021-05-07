#ifndef TWI_SERVER_H
#define TWI_SERVER_H

#include <avr/interrupt.h>
#include <stdint.h>

void twi_server_configure(void (*rcv)(uint8_t), void (*snd)(void));
void twi_server_start(uint8_t address);
void twi_server_stop(void);

inline void __attribute__((always_inline)) twi_server_send(uint8_t data) {
  TWDR = data;
}

ISR(TWI_vect);

#endif
