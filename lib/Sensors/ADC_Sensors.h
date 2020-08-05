#ifndef _ADC_SENSORS_H_
#define _ADC_SENSORS_H_

#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/queue.h"

#define ADC_SENSOR_TAG "ADC_Sensors"

// Moisture Sensor
#define MOISTURE_SENSOR_ADC_CHANNEL      (ADC1_CHANNEL_6) // GPIO34
#define WATER_SENSOR_ADC_CHANNEL         (ADC1_CHANNEL_7) // GPIO35
#define ADC_ATTEN                        (ADC_ATTEN_DB_11) // Gives full-scale voltage 3.9 V
#define NO_OF_SAMPLES                    (64)              // Multisampling
#define MOISTURE_CHECK_INTERVAL_IN_SEC   (10)
#define WATER_CHECK_INTERVAL_IN_SEC      (10)
#define ADC_SENSOR_QUEUE_SEND_TIME_IN_MS (pdMS_TO_TICKS(2000))

#ifdef DEBUG
static void adc_sensor_test_task(void* arg)
{
  for(;;) {
    printf("ADC_SENSOR_TEST_TASK\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}
#endif

static void moisture_sensor_task(void* arg)
{
  if(arg == NULL) {
    /* Queue was not created */
    ESP_LOGE(ADC_SENSOR_TAG, "[ERROR] xQueueMoistureSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Queue to send the information of moisture sensor
  QueueHandle_t xQueueMoistureSensor = (QueueHandle_t)arg;
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  // Calculate the task delay
  const TickType_t xDelay =
      pdMS_TO_TICKS(MOISTURE_CHECK_INTERVAL_IN_SEC * 1000);
  for(;;) {
    adc_reading = 0;
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(MOISTURE_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
#ifdef DEBUG
    ESP_LOGI(ADC_SENSOR_TAG, "[MOISTURE_SENSOR] moisture = %d", adc_reading);
#endif
    // Send sensor value to Queue
    if(xQueueSendToBack(xQueueMoistureSensor,
                        &adc_reading,
                        ADC_SENSOR_QUEUE_SEND_TIME_IN_MS) != pdTRUE) {
      ESP_LOGE(
          ADC_SENSOR_TAG,
          "[MOISTURE_SENSOR][ERROR] sending moisture sensor value in queue");
    }
    // Block task for time calculated
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}

static void water_sensor_task(void* arg)
{
  if(arg == NULL) {
    /* Queue was not created */
    ESP_LOGE(ADC_SENSOR_TAG, "[ERROR] xQueueWaterSensor was not created");
    // TODO: Handle Error
    while(1) {}
  }
  // Queue to send the information of water sensor
  QueueHandle_t xQueueWaterSensor = (QueueHandle_t)arg;
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(WATER_CHECK_INTERVAL_IN_SEC * 1000);
  for(;;) {
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
#ifdef DEBUG
    ESP_LOGI(ADC_SENSOR_TAG, "[WATER_SENSOR] water = %d", adc_reading);
#endif
    // Send sensor value to Queue
    if(xQueueSendToBack(xQueueWaterSensor,
                        &adc_reading,
                        ADC_SENSOR_QUEUE_SEND_TIME_IN_MS) != pdTRUE) {
      ESP_LOGE(ADC_SENSOR_TAG,
               "[WATER_SENSOR][ERROR] sending water sensor value in queue");
    }
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
  // Initialize the channel 6 full-scale between 150 to 2450 mV
  esp_error = adc1_config_channel_atten(MOISTURE_SENSOR_ADC_CHANNEL, ADC_ATTEN);
  if(esp_error != ESP_OK) { goto error_02; }
  // Initialize the channel 7 full-scale between 150 to 2450 mV
  esp_error = adc1_config_channel_atten(WATER_SENSOR_ADC_CHANNEL, ADC_ATTEN);
  if(esp_error != ESP_OK) { goto error_03; }
error_01:
error_02:
error_03:
  // TODO: Error handling
  ESP_LOGI(ADC_SENSOR_TAG, "Finish init ADC..");
  ESP_ERROR_CHECK(esp_error);
  return esp_error;
}
#endif /* _ADC_SENSORS_H_ */
