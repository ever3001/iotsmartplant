#ifndef _MOISTURE_SENSOR_H_
#define _MOISTURE_SENSOR_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/adc.h"

typedef struct {
  uint32_t value;
  SemaphoreHandle_t xSemaphore;
} moisture_sensor_data_t;

esp_err_t setup_moisture_sensor();
void moisture_sensor_task(void* pvParameter);

extern moisture_sensor_data_t _moisture_sensor_data;

#endif // _MOISTURE_SENSOR_H_