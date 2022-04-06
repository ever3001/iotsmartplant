#ifndef _WATER_SENSOR_H_
#define _WATER_SENSOR_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/adc.h"

typedef struct {
  uint32_t value;
  SemaphoreHandle_t xSemaphore;
} water_sensor_data_t;

esp_err_t setup_water_sensor();
void water_sensor_task(void* pvParameter);

extern water_sensor_data_t _water_sensor_data;

#endif // _WATER_SENSOR_H_