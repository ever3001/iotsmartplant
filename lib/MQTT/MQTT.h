#ifndef _MQTT_H_
#define _MQTT_H_

/** LWIP Headers */
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "mqtt_client.h"

typedef struct {
  SemaphoreHandle_t xSemaphore;
  esp_mqtt_client_handle_t client;
  bool connected;
} MQTT_t;

extern MQTT_t _mqtt;

esp_err_t setup_mqtt();

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);

#endif // _MQTT_H_