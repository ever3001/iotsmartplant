#include "MQTT_Water_Sensor.h"
#include "Water_Sensor.h"
#include "cJSON.h"
#include "MQTT.h"
#include "esp_log.h"

void mqtt_water_sensor_task(void* pvParameter){
  if( _water_sensor_data.xSemaphore != NULL && _mqtt.xSemaphore != NULL ){
        const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_ADC_WATER_SENSOR_MQTT_TASK_TIME_IN_SEC * 1000);
        int adc_reading_local = 0;
        int msg_id = 0;
        for(;;) {
            xSemaphoreTake(_water_sensor_data.xSemaphore, portMAX_DELAY);
            adc_reading_local = _water_sensor_data.value;
            xSemaphoreGive(_water_sensor_data.xSemaphore);

            cJSON* waterSensorVal = NULL;
            char* JSONString         = NULL;

            cJSON* JSONToSend = cJSON_CreateObject();
            if(JSONToSend == NULL) { goto end; }

            waterSensorVal = cJSON_CreateNumber(adc_reading_local);
            if(waterSensorVal == NULL) { goto end; }
            cJSON_AddItemToObject(JSONToSend, "water_s", waterSensorVal);

            JSONString = cJSON_Print(JSONToSend);
            xSemaphoreTake(_mqtt.xSemaphore, portMAX_DELAY);
            msg_id = esp_mqtt_client_publish(_mqtt.client, mqtt_water_sensor_subscribe_topic, JSONString, 0, 0, 0);
            xSemaphoreGive(_mqtt.xSemaphore);
            ESP_LOGI("[MQTT][WATER_SENSOR]", "Message ID: %d", msg_id);
        end:
            cJSON_Delete(JSONToSend);

            // wait for the next cycle
            vTaskDelay(xDelay);
        }
  }else{
        ESP_LOGE("[MQTT][WATER_SENSOR]", "Semaphores not initialized\r\n");
  }
  vTaskDelete(NULL);
}