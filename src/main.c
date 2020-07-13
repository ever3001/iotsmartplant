/** Freertos Headers */
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

/** Project Headers */
#include "ADC_Sensors.h"
#include "Actuators.h"


#define TAG "Main"


void createTasks()
{
  BaseType_t xReturned;
  TaskHandle_t xHandle = NULL;

  xReturned = xTaskCreate(gpio_task_example,
                          "GPIO_TASK_EXAMPLE",
                          1024,
                          (void*)NULL,
                          tskIDLE_PRIORITY,
                          NULL);
  if(xReturned == pdPASS) { goto error_xTaskCreate; }

  xReturned = xTaskCreate(moisture_sensor_task,
                          "MOISTURE_SENSOR_TASK",
                          1024,
                          (void*)NULL,
                          tskIDLE_PRIORITY,
                          NULL);
  if(xReturned == pdPASS) { goto error_xTaskCreate; }
  xReturned = xTaskCreate(water_sensor_task,
                          "WATER_SENSOR_TASK",
                          1024,
                          (void*)NULL,
                          tskIDLE_PRIORITY,
                          NULL);
  if(xReturned == pdPASS) { goto error_xTaskCreate; }
error_xTaskCreate:
  if(xReturned == pdPASS) { vTaskDelete(xHandle); }
  /* The task was created.  Use the task's handle to delete the task. */
}


void app_main(void)
{
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
  // This API initialises the default NVS partition. The default NVS partition
  // is the one that is labeled “nvs” in the partition table.
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(actuators_gpio_init());
  ESP_ERROR_CHECK(init_adc_sensor());

  createTasks();

  // Start the real time scheduler.
  vTaskStartScheduler();

  // Will not get here unless there is insufficient RAM.
}