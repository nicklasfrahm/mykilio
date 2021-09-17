#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "usart.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

// DISCLAIMER: This code is heavily inspired by this blogpost:
// https://embedds.com/using-standard-io-streams-in-avr-gcc/

// Sends a single byte via USART.
static int usart_snd(char byte, __attribute__((unused)) FILE* stream) {
  while (!(UCSR0A & (1 << UDRE0))) {
    // Wait while previous byte is completed.
  };

  // Write data to transmit buffer.
  UDR0 = byte;
  return 0;
}

// Receives a single byte via USART.
static int usart_rcv(FILE* stream) {
  // Preallocate space.
  char byte;

  while (!(UCSR0A & (1 << RXC0))) {
    // Wait for byte to be received.
  };
  byte = UDR0;

  // Loopback input data.
  usart_snd(byte, stream);

  // Return received data.
  return byte;
}

static FILE usart_fd = FDEV_SETUP_STREAM(usart_snd, usart_rcv, _FDEV_SETUP_RW);

// Configure the UART transceiver.
void usart_configure(context_t* ctx) {
  // Calculate required register value for desired BAUD.
  uint32_t ubbr_reg = ((ctx->cpu_frequency_hz / (ctx->usart_baud * 16))) - 1;

  // Configure BAUD.
  UBRR0H = (uint8_t)(ubbr_reg >> 8);
  UBRR0L = (uint8_t)(ubbr_reg);

  // Set frame format to 8 data bits, no parity, 1 stop bit.
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

  // Enable transmission and reception.
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

  // Assign the file stream to the standard input and the standard output.
  stdin = stdout = &usart_fd;
}
