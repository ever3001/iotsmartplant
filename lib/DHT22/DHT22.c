#include "DHT22.h"
#include "esp_log.h"
#include "dht.h"

esp_err_t setup_dht22(gpio_config_t * io_conf){
    ESP_LOGI("[DHT22]", "Setting up DHT22...\r\n");
    io_conf->pin_bit_mask |= (1ULL << CONFIG_DHT22_PIN);
    io_conf->mode         = GPIO_MODE_INPUT_OUTPUT;
    io_conf->pull_up_en   = GPIO_PULLUP_DISABLE;
    io_conf->pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf->intr_type    = GPIO_INTR_DISABLE;
    ESP_LOGI("[DHT22]", "DHT22 setup complete\r\n");
    return gpio_config(io_conf);
}

void dht22_task(void* pvParameter){
  if( _DHT22_data.xSemaphore != NULL ){
        const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_DHT22_TASK_TIME_IN_SEC * 1000);
        const dht_sensor_type_t sensor_type = DHT_TYPE_AM2301;
        _DHT22_data.value.hum = 0;
        _DHT22_data.value.temp = 0;
        _DHT22_data.err = ESP_FAIL;
        for(;;) {
            // save the value
            xSemaphoreTake(_DHT22_data.xSemaphore, portMAX_DELAY);
            _DHT22_data.err = dht_read_data(sensor_type, CONFIG_DHT22_PIN, &_DHT22_data.value.hum, &_DHT22_data.value.temp);
            if( _DHT22_data.err == ESP_OK ){
                float hum = _DHT22_data.value.hum / 10.0;
                float temp = _DHT22_data.value.temp / 10.0;
                ESP_LOGI("[DHT22]", "Humidity: %.1f%% Temp: %.1fC", hum, temp);
            }else{
                ESP_LOGE("[DHT22]", "Could not read data from sensor");
            }
            xSemaphoreGive(_DHT22_data.xSemaphore);
        
            //! NOTE: wait at least 2 sec before reading again
            vTaskDelay(xDelay);
        }
  }else{
        ESP_LOGE("[DHT22]", "Semaphore not initialized\r\n");
  }
  vTaskDelete(NULL);
}
