#ifndef _MQTT_H_
#define _MQTT_H_

#include "MQTT_Setup.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
//#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

class MQTT_TCP
{
private:
    /* data */
public:
    MQTT_TCP(/* args */);
    ~MQTT_TCP();
};

MQTT_TCP::MQTT_TCP(/* args */)
{
}

MQTT_TCP::~MQTT_TCP()
{
}


#endif // !_MQTT_H_
