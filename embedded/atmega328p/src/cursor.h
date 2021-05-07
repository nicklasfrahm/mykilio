#ifndef CURSOR_H
#define CURSOR_H

#include <stddef.h>
#include <stdint.h>

// A cursor to manage the location of read and write operations via TWI.
typedef struct cursor {
  uint8_t address;
  size_t length;
  uint8_t index;
  uint8_t idle;
} cursor_t;

// The cursor is not currently in use and can be overwritten.
#define CURSOR_IDLE 1

// The cursor is currently in use.
#define CURSOR_BUSY 0

// A macro to initialize a cursor with the default values.
#define CURSOR_INITIALIZER \
  { 0x00, 0, 0, CURSOR_IDLE }

// Increment cursor index and release the cursor if all data was written.
void cursor_update(cursor_t* cursor);

// Write to an array of registers.
#define REG_WRITE(offset, registers, cursor, data) \
  registers[cursor.address - offset].bytes[cursor.index] = data
// Read from an array of registers.
#define REG_READ(offset, registers, cursor) \
  registers[cursor.address - offset].bytes[cursor.index]

#endif
