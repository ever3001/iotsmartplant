# IoTSmartPlant


## Important Links:
- [ESP32 Pinout Reference: Which GPIO pins should you use?](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

## Diagram


![Diagram](./assets/draws/connection.jpg "Diagram")



## Materials
- [ESP32 DEVKIT V1]()
- [Mini submersible pump]()
- [5mm tubing]()
- [Transistor 2N2222]()
- [Resistor 1 kΩ]()
- [Resistor 4.7 kΩ]()
- [ 3mm LED]()
- [Capacitive moisture sensor](https://es.aliexpress.com/item/32952553582.html?spm=a2g0o.productlist.0.0.3f253235dWeDtl&s=p&ad_pvid=202007090935193961854443111450000669045_1&algo_pvid=e6fe35a7-0468-441f-b80c-8ae535fbdc61&algo_expid=e6fe35a7-0468-441f-b80c-8ae535fbdc61-0&btsid=0b0a050115943125198128359e06a7&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [Water level sensor]()

## 3D Model

TODO:


## FreeRTOS Notes

> Using vTaskDelay(x/ portTICK_PERIOD_MS) add more code then using vTaskDelay(pdMS_TO_TICKS(x)).
> Don't forget to calculate stack for a task when creating a task xTaskCreate(...)