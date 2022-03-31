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


## Documentation

### Building locally

To work locally with this project, you'll have to follow the steps below:

1. Fork, clone or download this project
1. [Install][sphinx] Sphinx
1. Install sphinx-rtd-theme
1. Generate the documentation: `make`

The generated HTML will be located in the location specified by `conf.py`,
in this case `_build/html`.