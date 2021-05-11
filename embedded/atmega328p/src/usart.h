#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>

// This code is heavily inspired by this blogpost:
// https://embedds.com/using-standard-io-streams-in-avr-gcc/

// Initializes the USART transceiver.
void usart_configure(void);

#endif
