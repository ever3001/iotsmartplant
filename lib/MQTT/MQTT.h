#ifndef _MQTT_H_
#define _MQTT_H_

/** ESP32 Headers */
#include "esp_event_loop.h"
#include "esp_wifi.h"

/** FreeRTOS Headers */
#include "freertos/event_groups.h"
#include "freertos/queue.h"

/** LWIP Headers */
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

/** Other Headers */
#include "Actuators.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include <cJSON.h>
#include <stdbool.h>
#include <string.h>

#define CONFIG_WIFI_SSID           "It's here mom"
#define CONFIG_WIFI_PASSWORD       "asdf1234"
#define MQTT_TAG                   "MQTT"
#define CONFIG_BROKER_URL          "mqtt://broker.hivemq.com:1883"
#define CONFIG_BROKER_PORT         (1883)
#define MQTT_QUEUE_SEND_TIME_IN_MS (pdMS_TO_TICKS(10000))


const char* const pubArray[] = { "/ping"
                                 "/sensor/water-sensor-01",
                                 "/sensor/moisture-sensor-01" };

const char* const subArray[] = { "/actuator/led-01", "/actuator/pump-01" };

#define DATA_BUFF_SIZE (128)

#define CONNECTED_BIT BIT0

static EventGroupHandle_t wifi_event_group;
esp_mqtt_client_handle_t client;

bool mqttConnected = false;

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
  client = event->client;
  int msg_id;
  // your_context_t *context = event->context;
  switch(event->event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_CONNECTED");
      mqttConnected = true;

      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
      ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
      ESP_LOGI(MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_DISCONNECTED");
      mqttConnected = false;
      // TODO: BLINK LED
      break;

    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI(MQTT_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
      // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0,
      // 0); ESP_LOGI(MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
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

      if(event->data_len < DATA_BUFF_SIZE) {
        // Creates a buffer to save data received
        char dataString[DATA_BUFF_SIZE];
        // Empty C-String
        memset(dataString, 0, strlen(dataString));
        // Copy event->data trought new string
        strncpy(dataString, event->data, event->data_len);
#ifdef DEBUG
        ESP_LOGI(MQTT_TAG, "[mqtt_event_handler] data = %s", dataString);
#endif
        cJSON* data_json = cJSON_Parse(dataString);
        if(data_json == NULL) {
          // const char* error_ptr = cJSON_GetErrorPtr();
          // if(error_ptr != NULL) {
          //   fprintf(stderr, "Error before: %s\n", error_ptr);
          // }
          // status = 0;
          goto end_cjson;
          ESP_LOGE(MQTT_TAG, "[mqtt_event_handler] data_json NULL");
        }
        const cJSON* pump = NULL;
        pump              = cJSON_GetObjectItemCaseSensitive(data_json, "pump");
        if((pump != NULL) && cJSON_IsNumber(pump)) {
          ESP_LOGI(MQTT_TAG, "[mqtt_event_handler] pump = %d", pump->valueint);
        }

        const cJSON* led = NULL;
        led              = cJSON_GetObjectItemCaseSensitive(data_json, "led");
        if((led != NULL) && cJSON_IsNumber(led)) {
          ESP_LOGI(MQTT_TAG, "[mqtt_event_handler] led = %d", led->valueint);
        }


      end_cjson:
        cJSON_Delete(data_json);
      }


      break;
    case MQTT_EVENT_ERROR: ESP_LOGI(MQTT_TAG, "MQTT_EVENT_ERROR"); break;
    default: ESP_LOGI(MQTT_TAG, "Other event id:%d", event->event_id); break;
  }
  return ESP_OK;
}


static void mqtt_app_start(void)
{
  ESP_LOGI(MQTT_TAG, "MQTT_APP_START");
  esp_mqtt_client_config_t mqtt_cfg = {
    .uri = CONFIG_BROKER_URL,
    // .user_context = (void *)your_context
  };
  mqtt_cfg.event_handle = (mqtt_event_callback_t)mqtt_event_handler;

  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  // esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID,
  // mqtt_event_handler, client);
  ESP_ERROR_CHECK(esp_mqtt_client_start(client));
}


static void mqtt_water_sensor_task(void* arg)
{
  if(arg == NULL) {
    /* Queue was not created */
    ESP_LOGE(MQTT_TAG, "[ERROR] xQueueWaterSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Queue to send the information of water sensor
  QueueHandle_t xQueueWaterSensor = (QueueHandle_t)arg;
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  int msg_id;
  for(;;) {
    if(xQueueReceive(xQueueWaterSensor,
                     &adc_reading,
                     (WATER_CHECK_INTERVAL_IN_SEC + 1) * 1000 /
                         portTICK_RATE_MS) == pdTRUE) {
#ifdef DEBUG
      ESP_LOGI(MQTT_TAG, "[WATER_SENSOR] water = %d", adc_reading);
#endif
      if(client != NULL) {
        cJSON* waterSensorVal = NULL;
        char* JSONString      = NULL;

        cJSON* JSONToSend = cJSON_CreateObject();
        if(JSONToSend == NULL) { goto end; }

        waterSensorVal = cJSON_CreateNumber(adc_reading);
        if(waterSensorVal == NULL) { goto end; }
        cJSON_AddItemToObject(JSONToSend, "water_s", waterSensorVal);

        JSONString = cJSON_Print(JSONToSend);

        msg_id = esp_mqtt_client_publish(
            client, "/sensor/water-sensor-01", JSONString, 0, 0, 0);
        ESP_LOGI(MQTT_TAG, "/sensor/water-sensor-01 successful, msg_id=%d", msg_id);
      end:
        cJSON_Delete(JSONToSend);
      }
    } else {
      ESP_LOGI(MQTT_TAG, "[WATER_SENSOR] no lecture");
    }
  }
  vTaskDelete(NULL);
}


static void mqtt_moisture_sensor_task(void* arg)
{
  if(arg == NULL) {
    /* Queue was not created */
    ESP_LOGE(MQTT_TAG, "[ERROR] xQueueMoistureSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Queue to send the information of water sensor
  QueueHandle_t xQueueMoistureSensor = (QueueHandle_t)arg;
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  int msg_id;
  for(;;) {
    if(xQueueReceive(xQueueMoistureSensor,
                     &adc_reading,
                     (MOISTURE_CHECK_INTERVAL_IN_SEC + 1) * 1000 /
                         portTICK_RATE_MS) == pdTRUE) {
#ifdef DEBUG
      ESP_LOGI(MQTT_TAG, "[MOISTURE_SENSOR] water = %d", adc_reading);
#endif
      if(client != NULL) {
        cJSON* moistureSensorVal = NULL;
        char* JSONString         = NULL;

        cJSON* JSONToSend = cJSON_CreateObject();
        if(JSONToSend == NULL) { goto end; }

        moistureSensorVal = cJSON_CreateNumber(adc_reading);
        if(moistureSensorVal == NULL) { goto end; }
        cJSON_AddItemToObject(JSONToSend, "moisture_s", moistureSensorVal);

        JSONString = cJSON_Print(JSONToSend);

        msg_id = esp_mqtt_client_publish(
            client, "/sensor/moisture-sensor-01", JSONString, 0, 0, 0);
        ESP_LOGI(MQTT_TAG, "/sensor/moisture-sensor-01 successful, msg_id=%d", msg_id);
      end:
        cJSON_Delete(JSONToSend);
      }
    } else {
      ESP_LOGI(MQTT_TAG, "[MOISTURE_SENSOR] no lecture");
    }
  }
  vTaskDelete(NULL);
}

#endif // !_MQTT_H_
