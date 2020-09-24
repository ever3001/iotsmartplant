#include "Water_Sensor.h"
/* Espressif Headers */
#include "driver/adc.h"
#include "esp_log.h"

/* FreeRTOS Headers */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define WATER_SENSOR_ADC_CHANNEL       (ADC1_CHANNEL_7)  // GPIO34
#define NO_OF_SAMPLES                  (64)              // Multisampling
#define TAG                            ("WATER_SEN")
#define WATER_CHECK_INTERVAL_IN_SEC    (15)

uint32_t water_val = 0;

uint32_t getWaterVal(void)             { return water_val; }
static void setWaterVal(uint32_t val)  { water_val = val; }

static esp_err_t init_sensor()
{
  ESP_LOGI(TAG, "Initializing Water Sensor...");
  setWaterVal(0);
  esp_err_t esp_error = ESP_FAIL;
  /************** Configure ADC */
  // Initialize ADC1 with a 12 bit resolution
  esp_error = adc1_config_width(ADC_WIDTH_BIT_12);
  if(esp_error != ESP_OK) { goto error_01; }
  // Initialize the channel 7 full-scale between 150 to 2450 mV
  esp_error = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
  if(esp_error != ESP_OK) { goto error_02; }
  ESP_LOGI(TAG, "Water Sensor Initialized");
  ESP_ERROR_CHECK(esp_error);
  return ESP_OK;
error_02:
  ESP_LOGI(TAG, "Deinit ADC1 channel 7 for water sensor");
  // TODO: Deinit ADC1 channel 7
error_01:
  ESP_LOGI(TAG, "Disconfigure ADC1 widht");
  // TODO: Deconfig ADC1 width
  ESP_LOGE(TAG, "Error in Water Sensor");
  return esp_error;
}

void water_sensor_task(void* pvParameter)
{
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(WATER_CHECK_INTERVAL_IN_SEC * 1000);
  for(;;) {
    adc_reading = 0;
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
    setWaterVal(adc_reading);
    ESP_LOGI(TAG, "Water Sensor value = %d", getWaterVal());
    // Block task for time calculated
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}