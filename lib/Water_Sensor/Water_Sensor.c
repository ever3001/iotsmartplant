#include "Water_Sensor.h"

#include "esp_log.h"

esp_err_t setup_water_sensor()
{
  esp_err_t esp_error = ESP_FAIL;
  ESP_LOGI("[Water_sensor]", "Setting up Water_sensor...\r\n");
  // Initialize ADC1 with a 12 bit resolution
  esp_error = adc1_config_width(ADC_WIDTH_BIT_12);
  if(esp_error != ESP_OK) { goto error; }
  // Initialize the channel 7 full-scale between 150 to 2450 mV
  esp_error = adc1_config_channel_atten(CONFIG_ADC_WATER_SENSOR_CH, ADC_ATTEN_DB_11);
  if(esp_error != ESP_OK) { goto error; }
  ESP_LOGI("[Water_sensor]", "Water_sensor setup complete\r\n");
error:
  return esp_error;
}

void water_sensor_task(void* pvParameter)
{
  if(_water_sensor_data.xSemaphore != NULL) {
    const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_ADC_WATER_SENSOR_TASK_TIME_IN_SEC * 1000);
    int adc_reading;
    for(;;) {
      adc_reading = 0;
#ifdef CONFIG_ADC_WATER_SENSOR_MULTISAMPLE
      // Multisampling
      for(size_t i = 0; i < CONFIG_ADC_WATER_SENSOR_MULTISAMPLE_NO_SAMPLES; ++i) {
        adc_reading += adc1_get_raw(CONFIG_ADC_WATER_SENSOR_CH);
      }
      adc_reading /= CONFIG_ADC_WATER_SENSOR_MULTISAMPLE_NO_SAMPLES;
#else
      // Single sampling
      adc_reading = adc1_get_raw(CONFIG_ADC_WATER_SENSOR_CH);
#endif
      // save the value
      xSemaphoreTake(_water_sensor_data.xSemaphore, portMAX_DELAY);
      _water_sensor_data.value = adc_reading;
      // print the value
      ESP_LOGI("[Water_sensor]", "Water_sensor value: %d", _water_sensor_data.value);
      xSemaphoreGive(_water_sensor_data.xSemaphore);
      // wait for the next cycle
      vTaskDelay(xDelay);
    }
  } else {
    ESP_LOGE("[Water_sensor]", "Semaphore not initialized\r\n");
  }
  vTaskDelete(NULL);
}
