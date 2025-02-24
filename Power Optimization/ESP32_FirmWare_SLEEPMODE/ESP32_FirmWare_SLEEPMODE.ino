#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "esp_sleep.h"

// Wifi Credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_Wifi_Password";

// MQTT Broker details
const char* mqttServer = "broker.hivemq.com";  // Example broker
const int mqttPort = 1883;
const char* mqtt_User = "your_mqtt_username";
const char* mqtt_password = "Your_password";

// Pins
#define DHTPIN 4
#define DHTTYPE DHT11
#define LED_PIN 2

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

// Sleep time (in Microseconds) - Wake up every 5 minutes (300 secs)
#define SLEEP_TIME 300e6

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Blink LED Twice on Startup
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }

  // Read sensor data
  dht.begin();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print("C, Humidity: ");
      Serial.print(humidity);
      Serial.print("%");

      // Connect to WiFi
      WiFi.begin(ssid, password);
      Serial.print("Connecting to Wi-Fi");
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
      }
      Serial.println("\nWiFi Connected");

      // Connect to MQTT Broker
      client.setServer(mqttServer, mqttPort);
      if (connectToMQTT()) {
        client.publish("sensor/temperature", String(temperature).c_str());
        client.publish("sensor/humidity", String(humidity).c_str());
      }
  }
  Serial.println("Entering Deep Sleep Mode...");
  delay(1000); // Short delay before sleeping

  // Enter deep sleep mode
  esp_sleep_enable_timer_wakeup(SLEEP_TIME);
  esp_deep_sleep_start();
}

void loop() {
  // The device will never reach this since it goes into deep sleep
}

bool connectToMQTT() {
  Serial.print("Connecting to MQTT...");
  if (client.connect("ESP32_Client", mqtt_User, mqtt_password)) {
    Serial.println("Connected to MQTT broker");
    return true;
  } else {
    Serial.print("Failed, rc=");
    Serial.print(client.state());
    Serial.println(" Retrying...");
    return false;
  }
}
