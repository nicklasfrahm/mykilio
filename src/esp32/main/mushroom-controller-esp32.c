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

static const char *TAG = "bmc";
static const uint8_t BLINK_PERIOD = 100;

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

static uint8_t led_reg = 0x01;
static uint8_t led_state = 0x00;
static uint8_t peripheral_address = 0x40;

void app_main(void) {
  // Configure the I2C controller.
  i2c_controller_configure();

  while (1) {
    // Send command to peripheral.
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (peripheral_address << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write_byte(cmd, led_reg, 1);
    i2c_master_write_byte(cmd, led_state, 1);
    i2c_master_stop(cmd);
    esp_err_t ret =
        i2c_master_cmd_begin(CONFIG_I2C_CONTROLLER_PORT_NUM, cmd, 0);
    i2c_cmd_link_delete(cmd);

    if (ret) {
      ESP_LOGE(TAG, "Failed to send I2C command");
    } else {
      ESP_LOGI(TAG, "Setting LED state: %s", led_state ? "ON" : "OFF");
    }

    // Invert LED state.
    led_state = ~led_state;

    vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD));
  }
}
