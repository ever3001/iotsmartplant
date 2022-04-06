#ifndef _DHT22_H_
#define _DHT22_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct {
  int16_t hum;
  int16_t temp;
} DHT22_t;

typedef struct {
  DHT22_t value;
  esp_err_t err;
  SemaphoreHandle_t xSemaphore;
} DHT22_data_t;

esp_err_t setup_dht22(gpio_config_t* io_conf);

extern DHT22_data_t _DHT22_data;

void dht22_task(void* pvParameter);

#endif // _DHT22_H_