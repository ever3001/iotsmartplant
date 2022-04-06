#include "MQTT_DHT22.h"
#include "DHT22.h"
#include "cJSON.h"
#include "MQTT.h"
#include "esp_log.h"
#include "float.h"

void mqtt_dht22_task(void* pvParameter){
  if( _DHT22_data.xSemaphore != NULL && _mqtt.xSemaphore != NULL ){
        const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_DHT22_MQTT_TASK_TIME_IN_SEC * 1000);
        esp_err_t local_err = ESP_FAIL;
        double local_hum = 0.0;
        double local_temp = 0.0;
        int msg_id = 0;
        for(;;) {
            xSemaphoreTake(_DHT22_data.xSemaphore, portMAX_DELAY);
            local_err = _DHT22_data.err;
            local_hum = _DHT22_data.value.hum;
            local_temp = _DHT22_data.value.temp;
            xSemaphoreGive(_DHT22_data.xSemaphore);

            if(local_err == ESP_OK){
              local_hum /= 10.0;
              local_temp /= 10.0;
            }else{
              local_hum = -DBL_MAX;
              local_temp = -DBL_MAX;
            }

            cJSON* dht22cJSON = NULL;
            char* JSONString  = NULL;

            cJSON* JSONToSend = cJSON_CreateObject();
            if(JSONToSend == NULL) { goto end; }

            dht22cJSON = cJSON_CreateNumber(local_hum);
            if(dht22cJSON == NULL) { goto end; }
            cJSON_AddItemToObject(JSONToSend, "hum_s", dht22cJSON);

            dht22cJSON = cJSON_CreateNumber(local_temp);
            if(dht22cJSON == NULL) { goto end; }
            cJSON_AddItemToObject(JSONToSend, "temp_s", dht22cJSON);

            JSONString = cJSON_Print(JSONToSend);
            xSemaphoreTake(_mqtt.xSemaphore, portMAX_DELAY);
            msg_id = esp_mqtt_client_publish(_mqtt.client, mqtt_dht22_subscribe_topic, JSONString, 0, 0, 0);
            xSemaphoreGive(_mqtt.xSemaphore);
            ESP_LOGI("[MQTT][DHT22]", "Message ID: %d", msg_id);
        end:
            cJSON_Delete(JSONToSend);
            // wait for the next cycle
            vTaskDelay(xDelay);
        }
  }else{
        ESP_LOGE("[MQTT][DHT22]", "Semaphores not initialized\r\n");
  }
  vTaskDelete(NULL);
}
