#ifndef _ADC_SENSORS_H_
#define _ADC_SENSORS_H_

#include "driver/adc.h"
#include "esp_log.h"

#define ADC_SENSOR_TAG "ADC_Sensors"

// Moisture Sensor
#define MOISTURE_SENSOR_ADC_CHANNEL    (ADC1_CHANNEL_6) // GPIO34
#define WATER_SENSOR_ADC_CHANNEL       (ADC1_CHANNEL_7) // GPIO35
#define ADC_ATTEN                      (ADC_ATTEN_DB_0) // Attenuation for the ADC
#define NO_OF_SAMPLES                  (64)             // Multisampling
#define MOISTURE_CHECK_INTERVAL_IN_SEC (3)
#define WATER_CHECK_INTERVAL_IN_SEC    (4)


static void moisture_sensor_task(void* arg)
{
  for(;;) {
    uint32_t adc_reading = 0;
    // Calculate the task delay
    const TickType_t xDelay = pdMS_TO_TICKS(WATER_CHECK_INTERVAL_IN_SEC * 1000);
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(MOISTURE_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
#ifdef DEBUG
    ESP_LOGI(ADC_SENSOR_TAG, "[MOISTURE_SENSOR] adc read = %d", adc_reading);
#endif
    // TODO: Send sensor value to Server
    // Block task for time calculated
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}

static void water_sensor_task(void* arg)
{
  for(;;) {
    uint32_t adc_reading = 0;
    // Calculate the task delay
    const TickType_t xDelay = pdMS_TO_TICKS(WATER_CHECK_INTERVAL_IN_SEC * 1000);
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
#ifdef DEBUG
    ESP_LOGI(ADC_SENSOR_TAG, "[WATER_SENSOR] adc read = %d", adc_reading);
#endif
    // TODO: Send sensor value to Server
    // Block task for time calculated
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}

static esp_err_t init_adc_sensor()
{
  ESP_LOGI(ADC_SENSOR_TAG, "Initializing ADC..");
  esp_err_t esp_error = ESP_OK;
  /************** Configure ADC */
  // Initialize ADC1 with a 12 bit resolution
  esp_error = adc1_config_width(ADC_WIDTH_BIT_12);
  if(esp_error != ESP_OK) { goto error_01; }
  // Initialize the channel 6 without attenuation
  esp_error = adc1_config_channel_atten(MOISTURE_SENSOR_ADC_CHANNEL, ADC_ATTEN);
  if(esp_error != ESP_OK) { goto error_02; }
  // Initialize the channel 7 without attenuation
  esp_error = adc1_config_channel_atten(WATER_SENSOR_ADC_CHANNEL, ADC_ATTEN);
  if(esp_error != ESP_OK) { goto error_03; }
error_01:
error_02:
error_03:
  ESP_LOGI(ADC_SENSOR_TAG, "Finish init ADC..");
  ESP_ERROR_CHECK(esp_error);
  return esp_error;
}
#endif /* _ADC_SENSORS_H_ */
