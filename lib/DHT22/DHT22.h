#ifndef _DHT22_H_
#define _DHT22_H_

#include "Drivers_Map.h"
#include "Queues.h"
#include "dht.h"
#include "esp_log.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define DHT22_CHECK_INTERVAL_IN_SEC (5)
#define DHT22_QUEUE_SEND_TIME_IN_MS (pdMS_TO_TICKS(2000))


static const dht_sensor_type_t sensor_type = DHT_TYPE_AM2301;

typedef struct {
  int16_t hum;
  int16_t temp;
} DHT22_val_t;

#define DHT22_TAG ("DHT")

void DHT_task(void* pvParameter)
{
  if(xQueueHumTempSensor == NULL) {
    /* Queue was not created */
    ESP_LOGE(DHT22_TAG, "[ERROR] xQueueHumTempSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(DHT22_CHECK_INTERVAL_IN_SEC * 1000);

  //setDHTgpio(GPIO_DHT22_PIN);
  ESP_LOGI(DHT22_TAG, "Starting DHT Task\n\n");

  DHT22_val_t DHT22 = { 0, 0 };

  for(;;) {
    DHT22.hum  = 0;
    DHT22.temp = 0;

    if(dht_read_data(sensor_type, GPIO_DHT22_PIN, &DHT22.hum, &DHT22.temp) ==
       ESP_OK) {
      printf("Humidity: %d%% Temp: %dC\n", DHT22.hum / 10, DHT22.temp / 10);
    } else {
      printf("Could not read data from sensor\n");
    }

    ESP_LOGD(DHT22_TAG, "Hum: %.1d Tmp: %.1d\n", DHT22.hum, DHT22.temp);
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