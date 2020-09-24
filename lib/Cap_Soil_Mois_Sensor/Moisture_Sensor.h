#ifndef _MOISTURE_SENSOR_H_
#define _MOISTURE_SENSOR_H_
/* Standard Headers */
#include <stdint.h>

uint32_t getMoistureVal(void);
void moisture_sensor_task(void* pvParameter);

#endif /* _MOISTURE_SENSOR_H_ */