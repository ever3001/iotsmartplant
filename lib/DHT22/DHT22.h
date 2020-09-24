#ifndef _DHT22_H_
#define _DHT22_H_
/* Standard Headers */
#include <stdint.h>
typedef struct {
  int16_t hum;
  int16_t temp;
} DHT22_val_t;

extern DHT22_val_t DHT22Val;


int16_t dht22_int16_get_hum();
int16_t dht22_int16_get_temp();
float dht22_float_get_hum();
float dht22_float_get_temp();
DHT22_val_t dht22_int16_get_hum_temp();

void dht22_task(void* pvParameter);

#endif /* _DHT22_H_ */