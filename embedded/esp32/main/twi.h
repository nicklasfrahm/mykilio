#ifndef TWI_H
#define TWI_H

#include <stdint.h>

#include "driver/i2c.h"

// DISCLAIMER: I know that TWI is, at least for the most part, the same as I2C.
// I am mentioning this as I am conciously building an abstraction to:
//   (a) get rid of discriminating language,
//   (b) provide an easy interface to read and write remote registers.
// The idea of the abstraction is to provide a relatively neutral (register
// structure agnostic) interface for remote register access. The idea is similar
// to Modbus, but simpler. The goal is to have fewer and native C-like datatypes
// without assigning special meaning to their content like Modbus does.
// Currently this interface uses 7-bit TWI server addressing and 8-bit register
// addressing.

#define TWI_DISCOVERY_FIRST 1
#define TWI_DISCOVERY_LAST 127
// The maximum amout of devices that can possibly be discovered. The one in the
// macro is added as the address range includes the start and the end:
//   TWI_DISCOVERY_FIRST <= ADDRESS <= TWI_DISCOVERY_LAST
#define TWI_DISCOVERY_SIZE (TWI_DISCOVERY_LAST - TWI_DISCOVERY_FIRST + 1)

// Create an alias for the I2C type to share common naming.
typedef i2c_port_t twi_port_t;

// Stores the result of a server discovery, consisting of an array of addresses
// and an overall count of servers.
typedef struct twi_servers {
  uint8_t* addresses;
  uint8_t count;
} twi_servers_t;

// Configures the TWI client by setting the port and the pins for SCL and SDA.
// It also initializes data structures that are contained within this module and
// that are later used by other methods.
esp_err_t twi_client_configure(twi_port_t port, int32_t scl, int32_t sda);

// Discovers all servers on the TWI bus and returns an array of all found
// devices. It ignores the general call address as sending to this address will
// not provide any meaningfull acknowledgement specific to a single server.
twi_servers_t* twi_client_discover_servers(void);

// Writes a single byte to the TWI server.
esp_err_t twi_client_write_byte(uint8_t addr, uint8_t reg, uint8_t* byte);

#endif
