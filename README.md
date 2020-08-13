# IoTSmartPlant


## Important Links:
- [ESP32 Pinout Reference: Which GPIO pins should you use?](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

## Diagram

![Diagram](./assets/draws/connection.jpg "Diagram")



## Materials
- [ESP32 DEVKIT V1](https://aliexpi.com/O208)
- [Mini submersible pump](https://aliexpi.com/nxGM)
- [5mm tubing](https://aliexpi.com/nxGM)
- [Transistor 2N2222](https://aliexpi.com/3Rwf)
- [Resistor 1 kΩ](https://aliexpi.com/fG14)
- [Resistor 4.7 kΩ](https://aliexpi.com/fG14)
- [3mm LED]()
- [Capacitive moisture sensor](https://aliexpi.com/Ybzc)
- [Water level sensor](https://aliexpi.com/yfMp)
- [Humidity and Temperature sensor](https://aliexpi.com/JSlJ)

## 3D Model

TODO:


## FreeRTOS Notes

> Using vTaskDelay(x/ portTICK_PERIOD_MS) add more code then using vTaskDelay(pdMS_TO_TICKS(x)).
> Don't forget to calculate stack for a task when creating a task xTaskCreate(...)