/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#include "../../lib/mushroom.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "net.h"
#include "sdkconfig.h"
#include "twi.h"

#define TAG_TWI "twi"

void app_main(void) {
  // Create the default event loop that is running in background.
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  // Configure and start the ethernet IP stack.
  ESP_ERROR_CHECK(net_eth_configure_and_start());
  // Configure the TWI client.
  ESP_ERROR_CHECK(twi_client_configure(0, GPIO_NUM_2, GPIO_NUM_4));

  uint8_t led_state = 0xFF;
  while (1) {
    // Discover all available TWI servers.
    twi_servers_t *servers = twi_client_discover_servers();

    // Log discovered servers.
    ESP_LOGI(TAG_TWI, "Detected %d servers:", servers->count);
    for (uint8_t i = 0; i < servers->count; ++i) {
      ESP_LOGI(TAG_TWI, "  %X", servers->addresses[i]);
      twi_client_write_byte(servers->addresses[i], REG_LEDMAN, &led_state);
    }

    // Toggle LED state.
    led_state = ~led_state;

    // TODO: Remove this. It is only useful for debugging.
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
