#include "Semaphores.h"

#include "DHT22.h"
#include "MQTT.h"
#include "Moisture_Sensor.h"
#include "Water_Sensor.h"
#include "esp_log.h"

esp_err_t setup_semaphores()
{
  esp_err_t err = ESP_FAIL;
  ESP_LOGI("[SEMAPHORES]", "Setting up semaphores...\r\n");
  _moisture_sensor_data.xSemaphore = xSemaphoreCreateMutex();
  if(_moisture_sensor_data.xSemaphore == NULL) { goto error; }
  _water_sensor_data.xSemaphore = xSemaphoreCreateMutex();
  if(_water_sensor_data.xSemaphore == NULL) { goto error; }
  _DHT22_data.xSemaphore = xSemaphoreCreateMutex();
  if(_DHT22_data.xSemaphore == NULL) { goto error; }
  _mqtt.xSemaphore = xSemaphoreCreateMutex();
  if(_mqtt.xSemaphore == NULL) { goto error; }
  ESP_LOGI("[SEMAPHORES]", "Semaphores created\r\n");
  err = ESP_OK;
error:
  return err;
}