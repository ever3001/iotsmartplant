#ifndef _ACTUATORS_H_
#define _ACTUATORS_H_

#include "driver/gpio.h"
#include "esp_log.h"
#include "Drivers_Map.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ACTUATORS_TAG "Actuators"

#define GPIO_OUTPUT_PIN_SEL_MASK \
  ((1ULL << GPIO_LED_PIN) | (1ULL << GPIO_PUMP_PIN)) // Mask for the outputs

static inline esp_err_t SET_LED_LEVEL(bool level){ return gpio_set_level(GPIO_LED_PIN, level); }
static inline esp_err_t SET_LED_HIGH(){ return SET_LED_LEVEL(true); }
static inline esp_err_t SET_LED_LOW(){ return SET_LED_LEVEL(false); }

static inline esp_err_t SET_PUMP_LEVEL(bool level){ return gpio_set_level(GPIO_PUMP_PIN, level); }
static inline esp_err_t SET_PUMP_HIGH(){ return SET_PUMP_LEVEL(true); }
static inline esp_err_t SET_PUMP_LOW(){ return SET_PUMP_LEVEL(false); }


esp_err_t actuators_gpio_init()
{
  /** Configure OUTPUTS */
  // Variable to configure the GPIO
  gpio_config_t io_conf = {
    .pin_bit_mask = GPIO_OUTPUT_PIN_SEL_MASK, // Mask with all OUT pins
    .mode         = GPIO_MODE_OUTPUT,         // Set as output
    .pull_up_en   = GPIO_PULLUP_DISABLE,      // Disable pull-up mode
    .pull_down_en = GPIO_PULLDOWN_DISABLE,    // Disable pull-down mode
    .intr_type    = GPIO_INTR_DISABLE,        // Disable interrupt
  };
  // Configure GPIO with the given settings
  return gpio_config(&io_conf);
}

void blink_act_task(void* pvParameter)
{
  // Calculate the task delay
  const TickType_t xDelay = pdMS_TO_TICKS(1 * 1000);
  char blink = 0;
  for(;;){
    SET_PUMP_LEVEL(blink);
    SET_LED_LEVEL(blink);
    blink = !blink;
    vTaskDelay(xDelay);
    ESP_LOGI("ACTUATOR", "[Blink Task] blink = %d", (int)blink);
  }
  vTaskDelete(NULL);
}

#endif /* _ACTUATORS_H_ */