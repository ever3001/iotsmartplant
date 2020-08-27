#ifndef _SETUP_H_
#define _SETUP_H_

/*********** TAG DEFINITION */
typedef enum {
  TAG_MAIN = 0,
  TAG_ADC_SENSOR,
  TAG_DHT_22,
  TAG_PUMP,
  TAG_MQTT,
  TAG_MAX
} tag_t;
static const char* const allTags[TAG_MAX] = {
  "Main", "ADC_Sensors", "DHT_22", "PUMP", "MQTT"
};
/************************** */
#endif // !_SETUP_H_
