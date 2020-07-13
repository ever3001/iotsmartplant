#ifndef _ACTUATORS_H_
#define _ACTUATORS_H_

#include "driver/gpio.h"
#include "esp_log.h"

#define ACTUATORS_TAG "Actuators"

#define GPIO_LED_PIN  GPIO_NUM_2 // Led
#define GPIO_PUMP_PIN GPIO_NUM_4 // Pump

#define GPIO_OUTPUT_PIN_SEL_MASK \
  ((1ULL << GPIO_LED_PIN) | (1ULL << GPIO_PUMP_PIN)) // Mask for the outputs

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

static void gpio_task_example(void* arg)
{
  uint8_t state = 0;
  for(;;) {
    state = !state;
    gpio_set_level(GPIO_LED_PIN, state);
    gpio_set_level(GPIO_PUMP_PIN, state);
#ifdef DEBUG
    ESP_LOGI(ADC_SENSOR_TAG, "[GPIO_TASK_EX] state = %d", state);
#endif
    // Delay 1 second
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}
#endif /* _ACTUATORS_H_ */
