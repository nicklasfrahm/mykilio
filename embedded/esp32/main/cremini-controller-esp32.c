/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>

#include "../../lib/mushroom.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_tls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "net.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "twi.h"

#define TAG_TWI "twi"
#define TAG "MQTT"

extern const uint8_t mqtt_eclipseprojects_io_pem_start[] asm(
    "_binary_letsencrypt_pem_start");
extern const uint8_t mqtt_eclipseprojects_io_pem_end[] asm(
    "_binary_letsencrypt_pem_end");

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base,
           event_id);
  esp_mqtt_event_handle_t event = event_data;
  esp_mqtt_client_handle_t client = event->client;
  int msg_id;
  switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
      ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
      ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
      ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
      break;
    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
      break;

    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
      msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
      ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_PUBLISHED:
      ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_DATA:
      ESP_LOGI(TAG, "MQTT_EVENT_DATA");
      printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      printf("DATA=%.*s\r\n", event->data_len, event->data);
      if (strncmp(event->data, "send binary please", event->data_len) == 0) {
        ESP_LOGI(TAG, "Sending the binary");
      }
      break;
    case MQTT_EVENT_ERROR:
      ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
      if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
        ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x",
                 event->error_handle->esp_tls_last_esp_err);
        ESP_LOGI(TAG, "Last tls stack error number: 0x%x",
                 event->error_handle->esp_tls_stack_err);
        ESP_LOGI(TAG, "Last captured errno : %d (%s)",
                 event->error_handle->esp_transport_sock_errno,
                 strerror(event->error_handle->esp_transport_sock_errno));
      } else if (event->error_handle->error_type ==
                 MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
        ESP_LOGI(TAG, "Connection refused error: 0x%x",
                 event->error_handle->connect_return_code);
      } else {
        ESP_LOGW(TAG, "Unknown error type: 0x%x",
                 event->error_handle->error_type);
      }
      break;
    default:
      ESP_LOGI(TAG, "Other event id:%d", event->event_id);
      break;
  }
}

static void mqtt_app_start(void) {
  const esp_mqtt_client_config_t mqtt_cfg = {
      .uri = CONFIG_MQTT_URI,
      .cert_pem = (const char *)mqtt_eclipseprojects_io_pem_start,
      .username = CONFIG_MQTT_USERNAME,
      .password = CONFIG_MQTT_PASSWORD};

  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  /* The last argument may be used to pass data to the event handler, in this
   * example mqtt_event_handler */
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler,
                                 NULL);
  esp_mqtt_client_start(client);
}

void app_main(void) {
  // Create the default event loop that is running in background.
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  // Configure and start the ethernet IP stack.
  ESP_ERROR_CHECK(net_eth_configure_and_start());
  // Configure the TWI client.
  ESP_ERROR_CHECK(twi_client_configure(0, GPIO_NUM_2, GPIO_NUM_4));

  mqtt_app_start();

  uint32_t previous_count = 0;
  uint32_t previous_hash = 0;
  uint8_t led_state = 0xFF;
  while (1) {
    // Discover all available TWI servers.
    twi_servers_t *servers = twi_client_discover_servers();

    // Log discovered servers only if new servers were detected.
    uint32_t hash = 0;
    for (uint8_t i = 0; i < servers->count; ++i) {
      // Weak hash does not cover edge-cases. One example is where X servers
      // with sum(addr) Y are replaced by X servers with sum(addr) Y.
      hash += servers->addresses[i];
    }
    if (servers->count != previous_count || hash != previous_hash) {
      ESP_LOGI(TAG_TWI, "Detected %d servers: ", servers->count);
      for (uint8_t i = 0; i < servers->count; ++i) {
        ESP_LOGI(TAG_TWI, "  %X", servers->addresses[i]);
        twi_client_write_byte(servers->addresses[i], REG_LEDMAN, &led_state);
      }
      previous_count = servers->count;
      previous_hash = hash;
    }

    // Toggle LED state.
    led_state = ~led_state;

    // TODO: Remove this. It is only useful for debugging.
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
