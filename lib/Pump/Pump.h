#ifndef _PUMP_H_
#define _PUMP_H_

#include "driver/gpio.h"

esp_err_t setup_pump(gpio_config_t* io_conf);
void pump_task(void* pvParameter);

static inline esp_err_t set_pump_high()
{
  return gpio_set_level(CONFIG_PUMP_PIN, 1);
}
static inline esp_err_t set_pump_low()
{
  return gpio_set_level(CONFIG_PUMP_PIN, 0);
}

#endif // _PUMP_H_