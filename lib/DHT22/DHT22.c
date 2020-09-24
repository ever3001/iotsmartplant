#include "DHT22.h"

/* Espressif Headers */
#include "esp_log.h"
#include "driver/gpio.h"
/* FreeRTOS Headers */
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
/* Library Header */
#include "dht.h"

#define GPIO_DHT22_PIN              (GPIO_NUM_32)
#define DHT22_CHECK_INTERVAL_IN_SEC (10)
#define TAG                         ("DHT22")

// Variable to save the hum and temp
DHT22_val_t DHT22Val;
// Type of the sensor used. See the library for more info
static const dht_sensor_type_t sensor_type = DHT_TYPE_AM2301;

/* Getters */
int16_t dht22_int16_get_hum()               { return DHT22Val.hum; }
int16_t dht22_int16_get_temp()              { return DHT22Val.temp; }
float dht22_float_get_hum()                 { return DHT22Val.hum/10; }
float dht22_float_get_temp()                { return DHT22Val.temp/10; }
DHT22_val_t dht22_int16_get_hum_temp()      { return DHT22Val; }

/* Setters */
static void dht22_set_hum_val(int16_t val)  { DHT22Val.hum = val; }
static void dht22_set_temp_val(int16_t val) { DHT22Val.temp = val; }
static void dht22_set_val(DHT22_val_t val)  { DHT22Val.hum = val.hum; DHT22Val.temp = val.temp; }

/* Task for reading the sensor value each x time */
void dht22_task(void* pvParameter)
{
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(DHT22_CHECK_INTERVAL_IN_SEC * 1000);

  ESP_LOGI(TAG, "Starting DHT22 Task\n\n");

  DHT22_val_t DHT22Buff = { 0, 0 };

  for(;;) {
    DHT22Buff.hum  = 0;
    DHT22Buff.temp = 0;

    if(dht_read_data(sensor_type, GPIO_DHT22_PIN, &DHT22Buff.hum, &DHT22Buff.temp) == ESP_OK) {
      dht22_set_val(DHT22Buff);
      ESP_LOGI(TAG, "Humidity: %.2f%% Temp: %.2fC", dht22_float_get_hum(), dht22_float_get_temp());
    } else {
      ESP_LOGE(TAG, "Could not read data from sensor");
    }
    
    // -- wait at least 2 sec before reading again ------------
    // The interval of whole process must be beyond 2 seconds !!
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}