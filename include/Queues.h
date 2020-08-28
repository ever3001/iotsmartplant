#ifndef _QUEUES_H_
#define _QUEUES_H_

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define QUEUE_SZ (5)
#define QUEUE_TAG ("QUEUE")

#define QUEUE_OK                          (0)
#define QUEUE_ERROR_INIT_MOISTURE_SENSOR  (-1)
#define QUEUE_ERROR_INIT_WATER_SENSOR     (-2)
#define QUEUE_ERROR_INIT_HUM_TEMP_SENSOR  (-3)

typedef struct {
  int16_t hum;
  int16_t temp;
} DHT22_cpy_val_t;

QueueHandle_t xQueueMoistureSensor;
QueueHandle_t xQueueWaterSensor;
QueueHandle_t xQueueHumTempSensor;

int initQueues(void)
{
  xQueueMoistureSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueMoistureSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(QUEUE_TAG, "[ERROR] xQueueMoistureSensor init");
    return QUEUE_ERROR_INIT_MOISTURE_SENSOR;
  }

  xQueueWaterSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueWaterSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(QUEUE_TAG, "[ERROR] xQueueWaterSensor init");
    return QUEUE_ERROR_INIT_WATER_SENSOR;
  }

  xQueueHumTempSensor = xQueueCreate(QUEUE_SZ, sizeof(DHT22_cpy_val_t));
  if(xQueueHumTempSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(QUEUE_TAG, "[ERROR] xQueueHumTempSensor init");
    return QUEUE_ERROR_INIT_HUM_TEMP_SENSOR;
  }
  return QUEUE_OK;
}


#endif // !_QUEUES_H_