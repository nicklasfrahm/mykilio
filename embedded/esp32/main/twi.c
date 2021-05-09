#include "twi.h"

#include <stdint.h>

#include "driver/i2c.h"

static twi_port_t twi_port;
static twi_servers_t* servers = NULL;

esp_err_t twi_client_configure(twi_port_t port, int32_t scl, int32_t sda) {
  // Store port in internal variable to be used by other methods.
  twi_port = port;

  // Create TWI client configuration.
  i2c_config_t config = {
      .mode = I2C_MODE_MASTER,
      .scl_io_num = scl,
      .sda_io_num = sda,
      .sda_pullup_en = GPIO_PULLUP_ONLY,
      .scl_pullup_en = GPIO_PULLUP_ONLY,
      // TODO: Test if speed of 400000 is working.
      .master.clk_speed = 100000,
  };

  // Configure the
  esp_err_t err = i2c_param_config(twi_port, &config);
  if (err != ESP_OK) {
    return err;
  }

  // Install the driver and set the server receive and send buffer to 0, because
  // the client does not need it.
  err = i2c_driver_install(twi_port, config.mode, 0, 0, 0);
  if (err != ESP_OK) {
    return err;
  }

  // Allocate space for storing information about the server discovery.
  if (servers == NULL) {
    servers = malloc(sizeof(twi_servers_t));
    servers->addresses = malloc(TWI_DISCOVERY_SIZE * sizeof(uint8_t));
  }

  return ESP_OK;
}

twi_servers_t* twi_client_discover_servers(void) {
  // Allocate memory for the whole address space.
  servers->addresses =
      realloc(servers->addresses, TWI_DISCOVERY_SIZE * sizeof(uint8_t));
  servers->count = 0;

  // Using this register essentially ensures that the operation is a no-op.
  uint8_t ping_reg = 0x00;
  uint8_t ping_byte = 0x00;
  for (uint8_t addr = TWI_DISCOVERY_FIRST; addr <= TWI_DISCOVERY_LAST; addr++) {
    // We are using a write transaction here as a read transaction is
    // essentially just a write operation to the same register followed by a
    // read operation. This is caused by the fact that for a read transaction,
    // the address pointer needs to be specified first.
    esp_err_t error = twi_client_write_byte(addr, ping_reg, &ping_byte);
    if (!error) {
      // Add the server address to our array of discovered address and increment
      // the counter for the amout of discovered servers.
      servers->addresses[servers->count++] = addr;
    }
  }

  // Reallocate to free up unused memory.
  servers->addresses =
      realloc(servers->addresses, servers->count * sizeof(uint8_t));

  return servers;
}

esp_err_t twi_client_write_byte(uint8_t addr, uint8_t reg, uint8_t* byte) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, 1);
  i2c_master_write_byte(cmd, reg, 1);
  i2c_master_write_byte(cmd, *byte, 1);
  i2c_master_stop(cmd);
  esp_err_t err = i2c_master_cmd_begin(twi_port, cmd, 0);
  i2c_cmd_link_delete(cmd);

  return err;
}