#ifndef _MQTT_WATER_SENSOR_H_
#define _MQTT_WATER_SENSOR_H_

void mqtt_water_sensor_task(void* pvParameter);

extern const char* mqtt_water_sensor_subscribe_topic;

#endif // _MQTT_WATER_SENSOR_H_