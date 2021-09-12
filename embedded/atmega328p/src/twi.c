#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "twi.h"

#include <util/twi.h>

static void (*twi_server_on_receive)(uint8_t);
static void (*twi_server_on_send)(void);

void twi_server_configure(void (*on_receive)(uint8_t), void (*on_send)(void)) {
  twi_server_on_receive = on_receive;
  twi_server_on_send = on_send;
}

// TODO: Remove all occurences of `cli()` and `sei()`.

void twi_server_start(uint8_t address) {
  // Clear acknowledge and enable bits by
  // disabling interrupts.
  cli();
  // Load address into the TWI address register.
  TWAR = address << 1;
  // Set the TWCR to enable address matching and
  // enable TWI, clear TWINT, enable TWI interrupt.
  TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
  // Enable interrupts.
  sei();
}

void twi_server_stop(void) {
  // Clear acknowledge and enable bits by
  // disabling interrupts.
  cli();
  // Reset TWI registers.
  TWCR = 0;
  TWAR = 0;
  // Enable interrupts.
  sei();
}

ISR(TWI_vect) {
  switch (TW_STATUS) {
    case TW_SR_DATA_ACK:
      // Client is writing to server, call the receive callback.
      twi_server_on_receive(TWDR);
      break;
    case TW_ST_SLA_ACK:
      // Client is reading data, call the send callback
      twi_server_on_send();
      break;
    case TW_ST_DATA_ACK:
      // Client is reading data, call the send callback.
      twi_server_on_send();
      break;
    case TW_BUS_ERROR:
      // Some sort of erroneous state, prepare TWI to be readdressed.
      TWCR = 0;
      break;
  }

  // Set the TWCR to enable address matching and
  // enable TWI, clear TWINT, enable TWI interrupt.
  TWCR = (1 << TWIE) | (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
}
