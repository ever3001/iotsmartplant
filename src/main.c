/** Freertos Headers */
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"

/** Project Headers */
#include "ADC_Sensors.h"
#include "Actuators.h"
#include "MQTT.h"


#define TAG "Main"

#define QUEUE_SZ (5)

QueueHandle_t xQueueMoistureSensor, xQueueWaterSensor;

void createTasks()
{
#ifdef DEBUG
  ESP_LOGI(TAG, "Initializing Tasks");
#endif
  BaseType_t xReturned;
  TaskHandle_t xHandle = NULL;

  xReturned = xTaskCreate(moisture_sensor_task,
                          "MOISTURE_SENSOR_TASK",
                          1024 * 4,
                          (void*)xQueueMoistureSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

  xReturned = xTaskCreate(water_sensor_task,
                          "WATER_SENSOR_TASK",
                          1024 * 4,
                          (void*)xQueueWaterSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

  xReturned = xTaskCreate(mqtt_moisture_sensor_task,
                          "MQTT_MOISTURE_SENSOR_TASK",
                          1024 * 3,
                          (void*)xQueueMoistureSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

  xReturned = xTaskCreate(mqtt_water_sensor_task,
                          "MQTT_WATER_SENSOR_TASK",
                          1024 * 3,
                          (void*)xQueueWaterSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

error_xTaskCreate:
  if(xReturned != pdPASS) {
    // TODO: Error Handler
    ESP_LOGE(TAG, "[ERROR] Creating a task");
  }
#ifdef DEBUG
  ESP_LOGI(TAG, "Tasks Initialized");
#endif
}


void app_main(void)
{
  xQueueMoistureSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueMoistureSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(TAG, "[ERROR] xQueueMoistureSensor init");
    // TODO: Handle Error
    while(1) {}
  }

  xQueueWaterSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueWaterSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(TAG, "[ERROR] xQueueWaterSensor init");
    // TODO: Handle Error
    while(1) {}
  }


  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
  // This API initialises the default NVS partition. The default NVS partition
  // is the one that is labeled “nvs” in the partition table.
  ESP_ERROR_CHECK(nvs_flash_init());
  // ESP_ERROR_CHECK(actuators_gpio_init());
  ESP_ERROR_CHECK(init_adc_sensor());
  createTasks();
  wifi_init();
  mqtt_app_start();
}