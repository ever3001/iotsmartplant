#ifndef _MQTT_H_
#define _MQTT_H_

/** ESP32 Headers */
#include "esp_event_loop.h"
#include "esp_wifi.h"

/** FreeRTOS Headers */
#include "freertos/event_groups.h"

/** LWIP Headers */
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

/** Other Headers */
#include "esp_log.h"
#include "mqtt_client.h"

#define CONFIG_WIFI_SSID     "It's here mom"
#define CONFIG_WIFI_PASSWORD "asdf1234"
#define MQTT_TAG             "MQTT"
#define CONFIG_BROKER_URL    "mqtt://broker.hivemq.com:1883"

#define CONNECTED_BIT BIT0

static EventGroupHandle_t wifi_event_group;

static esp_err_t wifi_event_handler(void* ctx, system_event_t* event)
{
  switch(event->event_id) {
    case SYSTEM_EVENT_STA_START: esp_wifi_connect(); break;
    case SYSTEM_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);

      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      esp_wifi_connect();
      xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
      break;
    default: break;
  }
  return ESP_OK;
}

static void wifi_init(void)
{
  tcpip_adapter_init();
  wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_LOGI(MQTT_TAG, "start the WIFI SSID:[%s]", CONFIG_WIFI_SSID);
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_LOGI(MQTT_TAG, "Waiting for wifi");
  xEventGroupWaitBits(
      wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}


static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
  esp_mqtt_client_handle_t client = event->client;
  int msg_id;
  // your_context_t *context = event->context;
  switch(event->event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_CONNECTED");
      msg_id =
          esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
      ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
      ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
      ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
      ESP_LOGI(MQTT_TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
      break;
    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DISCONNECTED");
      break;

    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
      msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
      ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_PUBLISHED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_DATA:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DATA");
      printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      printf("DATA=%.*s\r\n", event->data_len, event->data);
      break;
    case MQTT_EVENT_ERROR: ESP_LOGI(MQTT_TAG, "MQTT_EVENT_ERROR"); break;
    default: ESP_LOGI(MQTT_TAG, "Other event id:%d", event->event_id); break;
  }
  return ESP_OK;
}


static void mqtt_app_start(void)
{
  esp_mqtt_client_config_t mqtt_cfg = {
    .uri = CONFIG_BROKER_URL,
    // .user_context = (void *)your_context
  };
  mqtt_cfg.event_handle = (mqtt_event_callback_t)mqtt_event_handler;

  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_start(client);
}

#endif // !_MQTT_H_
