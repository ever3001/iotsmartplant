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
#include "Queues.h"
#include "Setup.h"
#include "Tasks.h"

#define MAIN_TAG "Main"


void app_main(void)
{
#ifdef DEBUG
  // TODO: It doesn't work debug print for some reason
  esp_log_level_set("*", ESP_LOG_DEBUG);
#else
  esp_log_level_set("*", ESP_LOG_INFO);
#endif
  ESP_LOGI(MAIN_TAG, "[APP] Startup..");
  ESP_LOGI(MAIN_TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(MAIN_TAG, "[APP] IDF version: %s", esp_get_idf_version());
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
  if(initQueues() != QUEUE_OK) {
    // TODO: Handle error
    while(1) {}
  }
  createTasks();
  wifi_init();
  mqtt_app_start();
}