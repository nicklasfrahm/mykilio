#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "usart.h"

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define UART_BAUD 1000000
#define UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

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
    // Wait for byte to be received.s
  };
  byte = UDR0;

  // Loopback input data.
  usart_snd(byte, stream);

  // Return received data.
  return byte;
}

static FILE uart0_fd = FDEV_SETUP_STREAM(usart_snd, usart_rcv, _FDEV_SETUP_RW);

// Configure the UART transceiver.
void usart_configure(void) {
  // Configure baud rate.
  UBRR0H = UBRR_VALUE >> 8;
  UBRR0L = UBRR_VALUE;
  // Set frame format to 8 data bits, no parity, 1 stop bit.
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
  // Enable transmission and reception.
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
  // Assign the file stream to the standard input and the standard output.
  stdin = stdout = &uart0_fd;
}
