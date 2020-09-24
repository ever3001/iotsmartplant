#ifndef _TASKS_H_
#define _TASKS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
//#include "ADC_Sensors.h"
#include "Moisture_Sensor.h"
#include "Actuators.h"
#include "DHT22.h"
#include "MQTT.h"
#include "Setup.h"

typedef struct {
  TaskFunction_t pvTaskCode;
  const char* const pcName;
  const uint32_t usStackDepth;
  void* const pvParameters;
  UBaseType_t uxPriority;
} task_t;

#define STACK_MIN_SIZE (1024)

#define TASKS_TAG ("TASKS")

static const task_t allTask[] = {
  /*
  { function_pointer,           name_of_the_task,                 stack_size,                 parameters,                         priority         }
  */
  /*###### SENSOR TASKS */
  { moisture_sensor_task,       "MOISTURE_SENSOR_TASK",           STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  //{ water_sensor_task,          "WATER_SENSOR_TASK",              STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  { dht22_task,                 "HUMIDITY_TEMP_SENSOR_TASK",      STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  /*###### MQTT TASKS*/
  //{ mqtt_moisture_sensor_task,  "MQTT_MOISTURE_SENSOR_TASK",      STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY },
  //{ mqtt_water_sensor_task,     "MQTT_WATER_SENSOR_TASK",         STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY },
  //{ mqtt_hum_temp_sensor_task,  "MQTT_HUMIDITY_TEMP_SENSOR_TASK", STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY }

};

void createTasks( void )
{
    ESP_LOGI(TASKS_TAG, "Initializing Tasks");
    // TODO: Check if the array is empty if(....)
    BaseType_t xReturned = pdPASS;
    TaskHandle_t xHandle = NULL;
    size_t i;
    size_t arrayLength = sizeof(allTask) / sizeof(task_t);
    ESP_LOGD(TASKS_TAG," size = %d", arrayLength );
    for (i = 0; i < arrayLength; ++i)
    {
        ESP_LOGI(TASKS_TAG, "Initializing %s", allTask[i].pcName);
        xReturned = xTaskCreate(
                                allTask[i].pvTaskCode,
                                allTask[i].pcName,
                                allTask[i].usStackDepth,
                                (void *)allTask[i].pvParameters,
                                allTask[i].uxPriority,
                                &xHandle);
        if(xReturned != pdPASS) { break; }
        xHandle = NULL;
    }

    if(xReturned != pdPASS) {
        // TODO: Error Handler
        ESP_LOGE(TASKS_TAG, "[ERROR] Creating the task %s", allTask[i].pcName);
    }
    ESP_LOGI(TASKS_TAG, "Tasks Initialized");
} 


#endif // !_TASKS_H_