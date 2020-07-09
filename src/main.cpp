#ifdef __cplusplus

#include <iostream>

/** Freertos Headers */
#include "esp_system.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/**  ESP32 Headers */
#include "driver/gpio.h"
#include "driver/adc.h"

// Led
#define GPIO_LED_PIN GPIO_NUM_2
// Pump
#define GPIO_PUMP_PIN GPIO_NUM_12
// Mask for the outputs
#define GPIO_OUTPUT_PIN_SEL_MASK ((1ULL << GPIO_LED_PIN) | (1ULL << GPIO_PUMP_PIN))


// Moisture Sensor
#define MOISTURE_SENSOR_ADC_CHANNEL ADC1_CHANNEL_6
#define WATER_SENSOR_ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_ATTEN ADC_ATTEN_DB_0
#define NO_OF_SAMPLES 64          //Multisampling
#define MOISTURE_CHECK_INTERVAL_IN_SEC 3
#define WATER_CHECK_INTERVAL_IN_SEC 4

using namespace std;

int iot_plant_gpio_init(){
    /** Configure OUTPUTS */
    // Variable to configure the GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask   = GPIO_OUTPUT_PIN_SEL_MASK, // Bit mask of the pins that you want to set,e.g.GPIO18/19
        .mode           = GPIO_MODE_OUTPUT,         // Set as output
        .pull_up_en     = GPIO_PULLUP_DISABLE,      // Disable pull-up mode
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,    // Disable pull-down mode
        .intr_type      = GPIO_INTR_DISABLE,        // Disable interrupt
    };
    // Configure GPIO with the given settings
    gpio_config(&io_conf);

    /** Configure ADC */
    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(MOISTURE_SENSOR_ADC_CHANNEL, ADC_ATTEN);
    adc1_config_channel_atten(WATER_SENSOR_ADC_CHANNEL, ADC_ATTEN);

    return 0;
}



static void gpio_task_example(void* arg){
    uint8_t flag = 0;
    while(1)
    {
        flag = !flag;
        gpio_set_level(GPIO_LED_PIN, flag);
        gpio_set_level(GPIO_PUMP_PIN, flag);
        cout << "flag = " << (int)flag << endl;
        // Delay 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}

static void moisture_sensor_task(void* arg){
    while(1){
        uint32_t adc_reading = 0;
        //Multisampling
        for (size_t i = 0; i < NO_OF_SAMPLES; ++i)
        {
            adc_reading += adc1_get_raw(MOISTURE_SENSOR_ADC_CHANNEL);
        }
        adc_reading /= NO_OF_SAMPLES;
        cout << "Moisture sensor adc read = " << adc_reading << endl;
        vTaskDelay(pdMS_TO_TICKS(MOISTURE_CHECK_INTERVAL_IN_SEC * 1000));
    }
    vTaskDelete(NULL);
}


static void water_sensor_task(void* arg){
    while(1){
        uint32_t adc_reading = 0;
        //Multisampling
        for (size_t i = 0; i < NO_OF_SAMPLES; ++i)
        {
            adc_reading += adc1_get_raw(WATER_SENSOR_ADC_CHANNEL);
        }
        adc_reading /= NO_OF_SAMPLES;
        cout << "Water sensor adc read = " << adc_reading << endl;
        vTaskDelay(pdMS_TO_TICKS(WATER_CHECK_INTERVAL_IN_SEC * 1000));
    }
    vTaskDelete(NULL);
}

extern "C"
{
    void app_main(void)
    {
        // This API initialises the default NVS partition. The default NVS partition is the one that is labeled “nvs” in the partition table.
        nvs_flash_init();
        iot_plant_gpio_init();

        xTaskCreate(&gpio_task_example,     "GPIO Task Example",    1024, NULL, 5, NULL);
        xTaskCreate(&moisture_sensor_task,  "Moisture Sensor Task", 1024, NULL, 6, NULL);
        xTaskCreate(&water_sensor_task,     "Water Sensor Task",    1024, NULL, 7, NULL);
    }
}

#endif