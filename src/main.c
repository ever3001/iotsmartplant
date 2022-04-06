/** Freertos Headers **/
#include "esp_log.h" // ESP_LOGI
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h" // nvs_flash_init

/** Libraries Headers **/
#include "DHT22.h"
#include "Led.h"
#include "MQTT.h"
#include "Moisture_Sensor.h"
#include "Pump.h"
#include "Semaphores.h"
#include "Tasks.h"
#include "Water_Sensor.h"
#include "WiFi.h"
/*** External Libraries ***/
#include "cJSON.h"

/** Main globals variables **/
#include "Main_Globals.h"

esp_err_t setup();
esp_err_t setup_nvs();

void app_main(void)
{
  ESP_LOGI("[MAIN]", "Free memory: %d bytes\r\n", esp_get_free_heap_size());
  ESP_LOGI("[MAIN]", "IDF version: %s\r\n", esp_get_idf_version());
  ESP_ERROR_CHECK(setup());
}

esp_err_t setup()
{
  ESP_LOGI("[MAIN]", "Setting up...\r\n");
  // Initialize NVS
  ESP_ERROR_CHECK(setup_nvs());
  // Initialize LED
  ESP_ERROR_CHECK(setup_led(&_io_conf));
  // Initialize pump
  ESP_ERROR_CHECK(setup_pump(&_io_conf));
  // Initialize DHT22
  ESP_ERROR_CHECK(setup_dht22(&_io_conf));
  // Initialize Moisture Sensor
  ESP_ERROR_CHECK(setup_moisture_sensor());
  // Initialize Water Sensor
  ESP_ERROR_CHECK(setup_water_sensor());
  // Initialize Wifi
  ESP_ERROR_CHECK(setup_wifi());
  // Initialize MQTT
  ESP_ERROR_CHECK(setup_mqtt());
  // Initialize Semaphores
  ESP_ERROR_CHECK(setup_semaphores());
  // Initialize Tasks
  ESP_ERROR_CHECK(setup_tasks(_allTasks, sizeof(_allTasks) / sizeof(Task_t)));
  ESP_LOGI("[MAIN]", "Setup complete.\r\n");
  return ESP_OK;
}

esp_err_t setup_nvs()
{
  ESP_LOGI("[NVS]", "Setting up NVS...\r\n");
  esp_err_t err = nvs_flash_init();
  if(err == ESP_ERR_NVS_NO_FREE_PAGES) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_LOGI("[NVS]", "NVS setup complete\r\n");
  return err;
}
