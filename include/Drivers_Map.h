#ifndef _DRIVER_MAP_H_
#define _DRIVER_MAP_H_

#include "driver/gpio.h"
#include "driver/adc.h"

/**** GPIO */
#define GPIO_LED_PIN                    (GPIO_NUM_4)     // Led
#define GPIO_PUMP_PIN                   (GPIO_NUM_16)    // Pump
#define GPIO_DHT22_PIN                  (GPIO_NUM_32)    // DHT22


/***** ADC */
#define MOISTURE_SENSOR_ADC_CHANNEL     (ADC1_CHANNEL_6) // GPIO34
#define WATER_SENSOR_ADC_CHANNEL        (ADC1_CHANNEL_7) // GPIO35

#endif // !_DRIVER_MAP_H_
