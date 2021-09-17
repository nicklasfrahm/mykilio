#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

// The context describes the application configuration and can be used to share
// basic parameters and configuration between different modules.
typedef struct context {
  // The CPU frequency in Hz, which is based on the installed crystal.
  uint32_t cpu_frequency_hz;
  // The lowest possible address that should be used by any server on the TWI
  // bus. By adding the offset to this address, the actual TWI server address
  // can be obtained.
  uint8_t twi_base_address;
  // The amount of characters that can be sent and received every second. This
  // is often referred to as BAUD and sometimes also known as character rate,
  // but not to be confused with the bitrate.
  uint32_t usart_baud;
  // The interval in milliseconds between timer interrupts for periodic tasks.
  uint32_t timer_interval_ms;
} context_t;

#endif
