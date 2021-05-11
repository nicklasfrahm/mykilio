#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "usart.h"

#include <avr/io.h>

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
void usart_configure(uint32_t cpu_freq, uint32_t baud) {
  // Calculate required register value for desired BAUD.
  uint32_t ubbr_reg = ((cpu_freq / (baud * 16))) - 1;
  // Configure baud rate.
  UBRR0H = (uint8_t)(ubbr_reg >> 8);
  UBRR0L = (uint8_t)(ubbr_reg);
  // Set frame format to 8 data bits, no parity, 1 stop bit.
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
  // Enable transmission and reception.
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
  // Assign the file stream to the standard input and the standard output.
  stdin = stdout = &uart0_fd;
}
