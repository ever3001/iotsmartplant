#ifndef _MQTT_MOISTURE_SENSOR_H_
#define _MQTT_MOISTURE_SENSOR_H_

void mqtt_moisture_sensor_task(void* pvParameter);

extern const char* mqtt_moisture_sensor_subscribe_topic;

#endif // _MQTT_MOISTURE_SENSOR_H_