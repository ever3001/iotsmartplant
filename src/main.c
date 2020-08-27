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
#include "DHT22.h"
#include "MQTT.h"
#include "Setup.h"


#define TAG "Main"

#define QUEUE_SZ (5)

QueueHandle_t xQueueMoistureSensor, xQueueWaterSensor, xQueueHumTempSensor;

void createTasks()
{
#ifdef DEBUG
  ESP_LOGI(allTags[TAG_MAIN], "Initializing Tasks");
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

  xReturned = xTaskCreate(DHT_task,
                          "HUMIDITY_TEMP_SENSOR_TASK",
                          1024 * 4,
                          (void*)xQueueHumTempSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

  xReturned = xTaskCreate(mqtt_hum_temp_sensor_task,
                          "MQTT_HUMIDITY_TEMP_SENSOR_TASK",
                          1024 * 3,
                          (void*)xQueueHumTempSensor,
                          tskIDLE_PRIORITY,
                          &xHandle);
  if(xReturned != pdPASS) { goto error_xTaskCreate; }

error_xTaskCreate:
  if(xReturned != pdPASS) {
    // TODO: Error Handler
    ESP_LOGE(allTags[TAG_MAIN], "[ERROR] Creating a task");
  }
#ifdef DEBUG
  ESP_LOGI(allTags[TAG_MAIN], "Tasks Initialized");
#endif
}


void app_main(void)
{
  xQueueMoistureSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueMoistureSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(allTags[TAG_MAIN], "[ERROR] xQueueMoistureSensor init");
    // TODO: Handle Error
    while(1) {}
  }

  xQueueWaterSensor = xQueueCreate(QUEUE_SZ, sizeof(uint32_t));
  if(xQueueWaterSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(allTags[TAG_MAIN], "[ERROR] xQueueWaterSensor init");
    // TODO: Handle Error
    while(1) {}
  }

  xQueueHumTempSensor = xQueueCreate(QUEUE_SZ, sizeof(DHT22_val_t));
  if(xQueueHumTempSensor == NULL) {
    /* Queue was not created and must not be used. */
    ESP_LOGE(allTags[TAG_MAIN], "[ERROR] xQueueHumTempSensor init");
    // TODO: Handle Error
    while(1) {}
  }


  ESP_LOGI(allTags[TAG_MAIN], "[APP] Startup..");
  ESP_LOGI(allTags[TAG_MAIN],
           "[APP] Free memory: %d bytes",
           esp_get_free_heap_size());
  ESP_LOGI(allTags[TAG_MAIN], "[APP] IDF version: %s", esp_get_idf_version());
  // This API initialises the default NVS partition. The default NVS partition
  // is the one that is labeled “nvs” in the partition table.
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(actuators_gpio_init());
  ESP_ERROR_CHECK(init_adc_sensor());

  esp_log_level_set("*", ESP_LOG_DEBUG);

  createTasks();
  wifi_init();
  mqtt_app_start();
}