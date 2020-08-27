#ifndef _DHT22_H_
#define _DHT22_H_

#include "DHT.h"
#include "esp_log.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "Drivers_Map.h"

#define DHT22_CHECK_INTERVAL_IN_SEC (5)
#define DHT22_QUEUE_SEND_TIME_IN_MS (pdMS_TO_TICKS(2000))

typedef struct {
  float hum;
  float temp;
} DHT22_val_t;

static const char* DHT22_TAG = "DHT";

void DHT_task(void* pvParameter)
{
  if(pvParameter == NULL) {
    /* Queue was not created */
    ESP_LOGE(ADC_SENSOR_TAG, "[ERROR] xQueueHumTempSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(DHT22_CHECK_INTERVAL_IN_SEC * 1000);
  // Queue to send the information of DHT22
  QueueHandle_t xQueueHumTempSensor = (QueueHandle_t)pvParameter;

  setDHTgpio(GPIO_DHT22_PIN);
  ESP_LOGI(DHT22_TAG, "Starting DHT Task\n\n");

  DHT22_val_t DHT22 = { 0.0, 0.0 };

  for(;;) {
    int ret = readDHT();
    DHT22.hum  = 0.0;
    DHT22.temp = 0.0;

    errorHandler(ret);
#if 0
    if(ret == DHT_OK) {
#endif
      DHT22.hum  = getHumidity();
      DHT22.temp = getTemperature();
      ESP_LOGD(DHT22_TAG, "Hum: %.1f Tmp: %.1f\n", DHT22.hum, DHT22.temp);
      // Send sensor value to Queue
      if(xQueueSendToBack(xQueueHumTempSensor,
                          &DHT22,
                          DHT22_QUEUE_SEND_TIME_IN_MS) != pdTRUE) {
        ESP_LOGE(
            DHT22_TAG,
            "[DHT22][ERROR] sending humidity and temperature value in queue");
      }
#if 0
    }else{
      ESP_LOGE(DHT22_TAG, "[DHT_task][ERROR] by reading the values");
    }
#endif

    // -- wait at least 2 sec before reading again ------------
    // The interval of whole process must be beyond 2 seconds !!
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}

#endif /* _DHT22_H_ */