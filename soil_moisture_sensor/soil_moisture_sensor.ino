#include "WiFi.h"
#include "DHTesp.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"

// WiFi setup
char ssid[] = "YOUR-SSID";
char pwd[] = "YOUR-SSID-PASSWORD";

WiFiClient wifiConn;

// Temperature/humidity sensor setup
int DHT_PIN = 22;
DHTesp dht;

// Moisture sensor setup
int MOISTURE_PIN = 32;

// MQTT client
PubSubClient client(wifiConn);
char MQTT_HOST[] = "YOUR-MQTT-HOST";
int MQTT_PORT = YOUR-MQTT-PORT;

// Deep sleep
int DEEP_SLEEP_INTERVAL_IN_MINUTES = 15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Setup WiFi connection
  Serial.print("Connecting to: ");
  Serial.print(ssid);

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Read temperature/humidity sensor
  dht.setup(DHT_PIN, DHTesp::DHT11);
  delay(2000);

  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  float temperature_fahrenheit = dht.toFahrenheit(temperature);

  Serial.print("Temperature: ");
  Serial.println(temperature_fahrenheit);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Read soil moisture
  int soilMoisture = analogRead(MOISTURE_PIN);

  Serial.print("Soil moisture: ");
  Serial.println(soilMoisture);

  // Open MQTT connection
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.connect("YOUR-MQTT-TOPIC");

  Serial.println(client.state());

  // Build MQTT payload
  DynamicJsonDocument jsonPayload(200);
  jsonPayload["sensor"] = "YOUR-SENSOR-NAME";
  jsonPayload["reading"]["temperature"]["value"] = temperature_fahrenheit;
  jsonPayload["reading"]["humidity"]["value"] = humidity;
  jsonPayload["reading"]["moisture"]["value"] = soilMoisture;

  char strPayload[200];
  serializeJson(jsonPayload, strPayload);

  client.publish("YOUR-MQTT-TOPIC", strPayload);
  delay(1000);

  // Deep sleep to save battery
  Serial.println("Entering deep sleep...");
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_INTERVAL_IN_MINUTES * 60000000);
  esp_deep_sleep_start();
  Serial.println("Exiting deep sleep...");
}

void loop() {
  // put your main code here, to run repeatedly:

}
