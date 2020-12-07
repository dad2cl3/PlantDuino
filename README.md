# PlantDuino

## Description
Simple Arduino script to monitor the moisture level of a potted plant. Sensor telemetry data is published using MQTT. MQTT data is consumed by Node-RED and published to Blynk. Additionally, AWS Simple Notification Service (SNS) is utilized to trigger SMS messages to subscribers when low moisture conditions are recorded by the sensor.

## Required hardware
ESP32 WiFi-enabled capacitive soil moisture and temperature and humidity sensor [link](https://smile.amazon.com/gp/product/B07VX1DWKK/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)

