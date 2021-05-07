/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

static const char* TAG = "bmc";

// The I2C controller does not need any buffers.
#define I2C_CONTROLLER_RX_BUF_LEN 0
#define I2C_CONTROLLER_TX_BUF_LEN 0

static esp_err_t i2c_controller_configure(void) {
  i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = CONFIG_I2C_CONTROLLER_SDA,
      .sda_pullup_en = GPIO_PULLDOWN_DISABLE,
      .scl_io_num = CONFIG_I2C_CONTROLLER_SCL,
      .scl_pullup_en = GPIO_PULLUP_DISABLE,
      .master.clk_speed = CONFIG_I2C_CONTROLLER_FREQUENCY,
  };

  esp_err_t err = i2c_param_config(CONFIG_I2C_CONTROLLER_PORT_NUM, &conf);
  if (err != ESP_OK) {
    return err;
  }

  return i2c_driver_install(CONFIG_I2C_CONTROLLER_PORT_NUM, conf.mode,
                            I2C_CONTROLLER_RX_BUF_LEN,
                            I2C_CONTROLLER_TX_BUF_LEN, 0);
}

static esp_err_t ping_peripheral(uint8_t* address) {
  // Send command to peripheral.
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (*address << 1) | I2C_MASTER_WRITE, 1);
  i2c_master_write_byte(cmd, 0x00, 1);
  i2c_master_write_byte(cmd, 0xFF, 1);
  i2c_master_stop(cmd);
  esp_err_t error =
      i2c_master_cmd_begin(CONFIG_I2C_CONTROLLER_PORT_NUM, cmd, 0);
  i2c_cmd_link_delete(cmd);

  return error;
}

void app_main(void) {
  // Configure the I2C controller.
  i2c_controller_configure();

  while (1) {
    for (uint8_t address = 0x01; address < 0xFF; address++) {
      esp_err_t error = ping_peripheral(&address);
      if (error) {
        ESP_LOGI(TAG, "Device found: %X", address);
      }
    }
  }
}
