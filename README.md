# PlantDuino

## Description
Simple Arduino script to monitor the moisture level of a potted plant. Sensor telemetry data is published using MQTT. MQTT data is consumed by Node-RED and published to Blynk. Additionally, AWS Simple Notification Service (SNS) is utilized to trigger SMS messages to subscribers when low moisture conditions are recorded by the sensor.

## Required hardware
ESP32 WiFi-enabled capacitive soil moisture and temperature and humidity sensor [link](https://smile.amazon.com/gp/product/B07VX1DWKK/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)

## Measurement range
The lower limit of the sensor was measured in a container of dirt dried in an oven at 250°F for several days and was equal to ???. The upper limit of the sensor was measured in a glass of water and was equal to 1360. Based on the formula below, the upper limit of the sensor is 0.74. Because the measurement from the sensor is inversely proportional to the moisture level of the soil, the inverse of the moisture reading multiplied by 1000 is utilized. The formula is as follows:

<img src="http://latex.codecogs.com/svg.latex?1/_{M_r}*1000" title="http://latex.codecogs.com/svg.latex?1/_{M_r}*1000" />

## Arduino setup
Instructions for adding ESP32 boards to the Arduino IDE can be found [here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).

### Recommended board: ESP32 Dev Module
ESP32 board documentation can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html).

### Required libraries:
1. [DHTesp](https://github.com/beegee-tokyo/DHTesp) - Reads temperature and humidity from the built-in DHT11 sensor.
2. [PubSubClient](https://pubsubclient.knolleary.net) - Creates an MQTT client to publish sensor telemetry to an MQTT broker.
3. [ArduinoJSON](https://arduinojson.org/) - Adds JSON support to Arduino scripts.
4. [WiFi](https://www.arduino.cc/en/Reference/WiFi) - Arduino library that supports WiFi connectivity


### Arduino script
Because the sensor can be battery-powered, the script leverages the ESP32 deep sleep function. Therefore, all code is in the setup function instead of the loop function.

The script performs the following actions:
1. Connect to specified WiFi access point
2. Get readings from the sensors
3. Format telemetry data as JSON
4. Connect to specified MQTT broker
5. Publish formatted telemetry data to MQTT broker
6. Deep sleep for specified interval

### Node-RED and Blynk
Node-RED relies upon the following node collections:
1. [node-red-contrib-blynk-ws](https://github.com/gablau/node-red-contrib-blynk-ws) - Handles the websocket connection to Blynk and provides the write event node for updating Blynk

Node-RED performs the following actions:
1. Manages MQTT broker connection
2. Adds a timestamp to the flow
3. Parse the telemetry data
4. Invert the moisture reading
5. Update Blynk
6. Evaluate inverted moisture reading for low moisture condition
7. Publish message to AWS Simple Notification Service(SNS) topic for low moisture condition through local API

### API
There are a few Node-RED node collections available for working with Amazon Web Services(AWS), and the Simple Notification Service(SNS), in particular. None appear to be actively under development. As a result, a local API was built to handle the interaction with SNS when a low moisture condition occurs. The API is written in Python3 using [aiohttp](https://docs.aiohttp.org/en/stable/) and leverages the [AWS Python SDK](https://aws.amazon.com/sdk-for-python/). The API exposes a single endpoint, /plants/moisture/low_alert/, in order to publish a message to an SNS topic. Subscribers to the SNS topic will receive an SMS message on their mobile phone altering them to the low moisture condition.
