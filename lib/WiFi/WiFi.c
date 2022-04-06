#include "WiFi.h"
#include "esp_log.h"

static EventGroupHandle_t wifi_event_group;

#define CONNECTED_BIT BIT0

static esp_err_t wifi_event_handler(void* ctx, system_event_t* event)
{
  switch(event->event_id) {
    case SYSTEM_EVENT_STA_START: esp_wifi_connect(); break;
    case SYSTEM_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      esp_wifi_connect();
      xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
      break;
    default: break;
  }
  return ESP_OK;
}


esp_err_t setup_wifi(){
  esp_err_t esp_error = ESP_FAIL;
  ESP_LOGI("[WIFI]", "Setting up WIFI..\r\n");
  tcpip_adapter_init();
  wifi_event_group = xEventGroupCreate();
  esp_error = esp_event_loop_init(wifi_event_handler, NULL);
  if(esp_error != ESP_OK) { goto error; }
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_error = esp_wifi_init(&cfg);
  if(esp_error != ESP_OK) { goto error; }
  esp_error = esp_wifi_set_storage(WIFI_STORAGE_RAM);
  if(esp_error != ESP_OK) { goto error; }
  wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASS,
        },
    };
  esp_error = esp_wifi_set_mode(WIFI_MODE_STA);
  if(esp_error != ESP_OK) { goto error; }
  esp_error = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  if(esp_error != ESP_OK) { goto error; }
  ESP_LOGI("[WIFI]", "Start the WIFI SSID:[%s]", CONFIG_WIFI_SSID);
  esp_error = esp_wifi_start();
  if(esp_error != ESP_OK) { goto error; }
  ESP_LOGI("[WIFI]", "Waiting for wifi\r\n");
  xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
  ESP_LOGI("[WIFI]", "WIFI setup complete\r\n");
error:
  return esp_error;
}