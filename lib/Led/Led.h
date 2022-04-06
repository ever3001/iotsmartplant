#ifndef _LED_H_
#define _LED_H_

#include "driver/gpio.h"

esp_err_t setup_led(gpio_config_t * io_conf);
void led_task(void* pvParameter);

esp_err_t set_led_high();
esp_err_t set_led_low();
esp_err_t set_led_toggle();

#endif // _LED_H_