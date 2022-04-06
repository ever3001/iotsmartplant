#ifndef _MQTT_DHT22_H_
#define _MQTT_DHT22_H_

void mqtt_dht22_task(void* pvParameter);

extern const char* mqtt_dht22_subscribe_topic;

#endif // _MQTT_DHT22_H_