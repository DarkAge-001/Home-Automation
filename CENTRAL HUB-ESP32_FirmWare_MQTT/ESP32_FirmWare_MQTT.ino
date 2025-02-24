#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

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

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");

  // Connect to MQTT
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();

  dht.begin();

  // LED ON to indicate full operation
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Read DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%");

  // Publish MQTT messages
  String tempStr = String(temperature);
  String humStr = String(humidity);

  client.publish("sensor/temperature", tempStr.c_str());
  client.publish("sensor/humidity", humStr.c_str());

  // Wait before sending the next reading
  delay(5000);
}

void connectToMQTT() {
  Serial.print("Connecting to MQTT...");
  while (!client.connected()) {
    if (client.connect("ESP32_Client", mqtt_User, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 Seconds...");
      delay(5000);
    }
  }
}
