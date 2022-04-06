#include "MQTT_Moisture_Sensor.h"

#include "MQTT.h"
#include "Moisture_Sensor.h"
#include "cJSON.h"
#include "esp_log.h"

void mqtt_moisture_sensor_task(void* pvParameter)
{
  if(_moisture_sensor_data.xSemaphore != NULL && _mqtt.xSemaphore != NULL) {
    const TickType_t xDelay = pdMS_TO_TICKS(CONFIG_ADC_MOISTURE_SENSOR_MQTT_TASK_TIME_IN_SEC * 1000);
    int adc_reading_local   = 0;
    int msg_id              = 0;
    for(;;) {
      xSemaphoreTake(_moisture_sensor_data.xSemaphore, portMAX_DELAY);
      adc_reading_local = _moisture_sensor_data.value;
      xSemaphoreGive(_moisture_sensor_data.xSemaphore);

      cJSON* moistureSensorVal = NULL;
      char* JSONString         = NULL;

      cJSON* JSONToSend = cJSON_CreateObject();
      if(JSONToSend == NULL) { goto end; }

      moistureSensorVal = cJSON_CreateNumber(adc_reading_local);
      if(moistureSensorVal == NULL) { goto end; }
      cJSON_AddItemToObject(JSONToSend, "moisture_s", moistureSensorVal);

      JSONString = cJSON_Print(JSONToSend);
      xSemaphoreTake(_mqtt.xSemaphore, portMAX_DELAY);
      msg_id = esp_mqtt_client_publish(_mqtt.client, mqtt_moisture_sensor_subscribe_topic, JSONString, 0, 0, 0);
      xSemaphoreGive(_mqtt.xSemaphore);
      ESP_LOGI("[MQTT][MOISTURE_SENSOR]", "Message ID: %d", msg_id);
    end:
      cJSON_Delete(JSONToSend);

      // wait for the next cycle
      vTaskDelay(xDelay);
    }
  } else {
    ESP_LOGE("[MQTT][MOISTURE_SENSOR]", "Semaphores not initialized\r\n");
  }
  vTaskDelete(NULL);
}