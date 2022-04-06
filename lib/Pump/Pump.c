#include "Pump.h"

#include "Moisture_Sensor.h"
#include "Water_Sensor.h"
#include "esp_log.h"

esp_err_t setup_pump(gpio_config_t* io_conf)
{
  ESP_LOGI("[PUMP]", "Setting up Pump...\r\n");
  io_conf->pin_bit_mask |= (1ULL << CONFIG_PUMP_PIN);
  io_conf->mode         = GPIO_MODE_OUTPUT;
  io_conf->pull_up_en   = GPIO_PULLUP_DISABLE;
  io_conf->pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf->intr_type    = GPIO_INTR_DISABLE;
  set_pump_low();
  ESP_LOGI("[PUMP]", "Pump setup complete\r\n");
  return gpio_config(io_conf);
}


void pump_task(void* pvParameter)
{
  if(_water_sensor_data.xSemaphore != NULL && _moisture_sensor_data.xSemaphore != NULL) {
    const TickType_t xDelay              = pdMS_TO_TICKS(CONFIG_PUMP_TASK_TIME_IN_SEC * 1000);
    uint32_t water_sensor_value_local    = 0;
    uint32_t moisture_sensor_value_local = 0;
    for(;;) {
      // Copy the value of the water sensor to a local variable
      xSemaphoreTake(_water_sensor_data.xSemaphore, portMAX_DELAY);
      water_sensor_value_local = _water_sensor_data.value;
      xSemaphoreGive(_water_sensor_data.xSemaphore);
      // Copy the value of the moisture sensor to a local variable
      xSemaphoreTake(_moisture_sensor_data.xSemaphore, portMAX_DELAY);
      moisture_sensor_value_local = _moisture_sensor_data.value;
      xSemaphoreGive(_moisture_sensor_data.xSemaphore);
      // If the water sensor value is greater than the threshold, turn on the led
      if(water_sensor_value_local > CONFIG_PUMP_TASK_WATER_SENSOR_THRESHOLD &&
         moisture_sensor_value_local < CONFIG_PUMP_TASK_MOISTURE_SENSOR_THRESHOLD) {
        set_pump_high();
      } else {
        set_pump_low();
      }
      // wait for the next cycle
      vTaskDelay(xDelay);
    }
  } else {
    ESP_LOGE("[PUMP]", "Semaphore not initialized\r\n");
  }
  vTaskDelete(NULL);
}
