#include "Led.h"
#include "esp_log.h"
#include "Water_Sensor.h"

uint8_t led_status = 0;

esp_err_t set_led_high(){
    esp_err_t esp_error = ESP_FAIL;
    esp_error = gpio_set_level(CONFIG_LED_PIN, 1);
    if(esp_error == ESP_OK){
        led_status = 1;
    }
    return esp_error;
}

esp_err_t set_led_low(){
    esp_err_t esp_error = ESP_FAIL;
    esp_error = gpio_set_level(CONFIG_LED_PIN, 0);
    if(esp_error == ESP_OK){
        led_status = 0;
    }
    return esp_error;
}

esp_err_t set_led_toggle(){
    esp_err_t esp_error = ESP_FAIL;
    esp_error = gpio_set_level(CONFIG_LED_PIN, (led_status + 1) & 1);
    if(esp_error == ESP_OK){
        led_status = (led_status + 1) & 1;
    }
    return esp_error;
}

esp_err_t setup_led(gpio_config_t * io_conf){
    ESP_LOGI("[LED]", "Setting up Led...\r\n");
    io_conf->pin_bit_mask |= (1ULL << CONFIG_LED_PIN);
    io_conf->mode         = GPIO_MODE_OUTPUT;
    io_conf->pull_up_en   = GPIO_PULLUP_DISABLE;
    io_conf->pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf->intr_type    = GPIO_INTR_DISABLE;
    set_led_high();
    ESP_LOGI("[LED]", "Led setup complete\r\n");
    return gpio_config(io_conf);
}

void led_task(void* pvParameter){
  if( _water_sensor_data.xSemaphore != NULL ){
        const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_LED_TASK_TIME_IN_SEC * 1000);
        uint32_t water_sensor_value_local = 0;
        for(;;) {
            // Copy the value of the water sensor to a local variable
            xSemaphoreTake(_water_sensor_data.xSemaphore, portMAX_DELAY);
            water_sensor_value_local = _water_sensor_data.value;
            xSemaphoreGive(_water_sensor_data.xSemaphore);
            // If the water sensor value is greater than the threshold, turn on the led
            if( water_sensor_value_local < CONFIG_LED_TASK_WATER_SENSOR_THRESHOLD ){
                set_led_toggle();
            }else{
                set_led_high();
            }
            // wait for the next cycle
            vTaskDelay(xDelay);
        }
  }else{
        ESP_LOGE("[LED]", "Semaphore not initialized\r\n");
  }
  vTaskDelete(NULL);
}
