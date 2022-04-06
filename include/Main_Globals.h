#ifndef _MAIN_GLOBALS_H_
#define _MAIN_GLOBALS_H_

#include "DHT22.h"
#include "Led.h"
#include "MQTT.h"
#include "MQTT_DHT22.h"
#include "MQTT_Moisture_Sensor.h"
#include "MQTT_Water_Sensor.h"
#include "Moisture_Sensor.h"
#include "Pump.h"
#include "Tasks.h"
#include "Water_Sensor.h"

#define STACK_MIN_SIZE (1024)

gpio_config_t _io_conf                       = { 0 };
moisture_sensor_data_t _moisture_sensor_data = { 0 };
water_sensor_data_t _water_sensor_data       = { 0 };
DHT22_data_t _DHT22_data                     = { 0 };
MQTT_t _mqtt = { .xSemaphore = NULL, .client = NULL, .connected = false };

const char* mqtt_moisture_sensor_subscribe_topic = "/sensor/moisture-sensor-01";
const char* mqtt_water_sensor_subscribe_topic    = "/sensor/water-sensor-01";
const char* mqtt_dht22_subscribe_topic           = "/sensor/dht22-01";

const Task_t _allTasks[] = {
  /*
  { function_pointer,           name_of_the_task,                 stack_size,                 parameters,   priority         }
  */
  /*###### ACTUATORS TASKS */
  { led_task,                   "LED_TASK",                       STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  { pump_task,                  "PUMP_TASK",                      STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  /*###### SENSOR TASKS */
  { moisture_sensor_task,       "MOISTURE_SENSOR_TASK",           STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  { water_sensor_task,          "WATER_SENSOR_TASK",              STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  { dht22_task,                 "HUMIDITY_TEMP_SENSOR_TASK",      STACK_MIN_SIZE * 4,         NULL,         tskIDLE_PRIORITY },
  /*###### MQTT SENDING TASKS*/
  { mqtt_moisture_sensor_task,  "MQTT_MOISTURE_SENSOR_TASK",      STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY },
  { mqtt_water_sensor_task,     "MQTT_WATER_SENSOR_TASK",         STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY },
  { mqtt_dht22_task,            "MQTT_DHT22_TASK",                STACK_MIN_SIZE * 3,         NULL,         tskIDLE_PRIORITY }
};

#endif // _MAIN_GLOBALS_H_