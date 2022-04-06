#include "MQTT.h"

#include "esp_log.h"

esp_err_t setup_mqtt()
{
  esp_err_t esp_error = ESP_FAIL;
  ESP_LOGI("[MQTT]", "Setting up MQTT..\r\n");
  esp_mqtt_client_config_t mqtt_cfg = {
    .uri = CONFIG_MQTT_BROKER_URL,
    // .user_context = (void *)your_context
  };
  mqtt_cfg.event_handle           = (mqtt_event_callback_t)mqtt_event_handler;
  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  // esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
  esp_error = esp_mqtt_client_start(client);
  ESP_LOGI("[MQTT]", "MQTT setup complete\r\n");
  return esp_error;
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
  _mqtt.client = event->client;
  int msg_id;
  // your_context_t *context = event->context;
  switch(event->event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI("[MQTT]", "MQTT_EVENT_CONNECTED");
      _mqtt.connected = true;

      msg_id = esp_mqtt_client_subscribe(_mqtt.client, "/topic/qos0", 0);
      ESP_LOGI("[MQTT]", "sent subscribe successful, msg_id=%d", msg_id);

      msg_id = esp_mqtt_client_subscribe(_mqtt.client, "/topic/qos1", 1);
      ESP_LOGI("[MQTT]", "sent subscribe successful, msg_id=%d", msg_id);

    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI("[MQTT]", "MQTT_EVENT_DISCONNECTED");
      _mqtt.connected = false;
      break;

    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI("[MQTT]", "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
      // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0,
      // 0); ESP_LOGI("[MQTT]", "sent publish successful, msg_id=%d", msg_id);
      break;
    case MQTT_EVENT_UNSUBSCRIBED: ESP_LOGI("[MQTT]", "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id); break;
    case MQTT_EVENT_PUBLISHED: ESP_LOGI("[MQTT]", "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id); break;
    case MQTT_EVENT_DATA:
      ESP_LOGI("[MQTT][MQTT_EVENT_DATA]", "TOPIC=%.*s\r\n", event->topic_len, event->topic);
      ESP_LOGI("[MQTT][MQTT_EVENT_DATA]", "DATA=%.*s\r\n", event->data_len, event->data);
      break;
    case MQTT_EVENT_ERROR: ESP_LOGI("[MQTT]", "MQTT_EVENT_ERROR"); break;
    default: ESP_LOGI("[MQTT]", "Other event id:%d", event->event_id); break;
  }
  return ESP_OK;
}