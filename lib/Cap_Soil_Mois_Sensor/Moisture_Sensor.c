#include "Moisture_Sensor.h"
/* Espressif Headers */
#include "driver/adc.h"
#include "esp_log.h"

/* FreeRTOS Headers */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define MOISTURE_SENSOR_ADC_CHANNEL       (ADC1_CHANNEL_6)  // GPIO34
#define NO_OF_SAMPLES                     (64)              // Multisampling
#define TAG                               ("MOISTURE_SEN")
#define MOISTURE_CHECK_INTERVAL_IN_SEC    (10)

uint32_t moisture_val = 0;

uint32_t getMoistureVal(void)             { return moisture_val; }
static void setMoistureVal(uint32_t val)  { moisture_val = val; }

static esp_err_t init_sensor()
{
  ESP_LOGI(TAG, "Initializing Moisture Sensor...");
  setMoistureVal(0);
  esp_err_t esp_error = ESP_FAIL;
  /************** Configure ADC */
  // Initialize ADC1 with a 12 bit resolution
  esp_error = adc1_config_width(ADC_WIDTH_BIT_12);
  if(esp_error != ESP_OK) { goto error_01; }
  // Initialize the channel 6 full-scale between 150 to 2450 mV
  esp_error = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  if(esp_error != ESP_OK) { goto error_02; }
  ESP_LOGI(TAG, "Moisture Sensor Initialized");
  ESP_ERROR_CHECK(esp_error);
  return ESP_OK;
error_02:
  ESP_LOGI(TAG, "Deinit ADC1 channel 6 for moisture sensor");
  // TODO: Deinit ADC1 channel 6
error_01:
  ESP_LOGI(TAG, "Disconfigure ADC1 widht");
  // TODO: Deconfig ADC1 width
  ESP_LOGE(TAG, "Error in Moisture Sensor");
  return esp_error;
}

void moisture_sensor_task(void* pvParameter)
{
  // Save the value of the raw adc
  uint32_t adc_reading = 0;
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(MOISTURE_CHECK_INTERVAL_IN_SEC * 1000);
  for(;;) {
    adc_reading = 0;
    // Multisampling
    for(size_t i = 0; i < NO_OF_SAMPLES; ++i) {
      adc_reading += adc1_get_raw(MOISTURE_SENSOR_ADC_CHANNEL);
    }
    adc_reading /= NO_OF_SAMPLES;
    setMoistureVal(adc_reading);
    ESP_LOGD(TAG, "Moisture Sensor value = %d", getMoistureVal());
    // Block task for time calculated
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}