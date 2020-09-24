#ifndef _WATER_SENSOR_H_
#define _WATER_SENSOR_H_
/* Standard Headers */
#include <stdint.h>

uint32_t getWaterVal(void);
void water_sensor_task(void* pvParameter);

#endif /* _WATER_SENSOR_H_ */