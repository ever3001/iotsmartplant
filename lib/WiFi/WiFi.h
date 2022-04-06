#ifndef _WIFI_H_
#define _WIFI_H_

#include "esp_event.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

esp_err_t setup_wifi();

#endif // _WIFI_H_