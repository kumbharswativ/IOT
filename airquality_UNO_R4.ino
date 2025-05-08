#include <WiFiS3.h>
#include <PubSubClient.h>

// WiFi Credentials
char ssid[] = "Never_Connect";         
char pass[] = "Masters@2024";

// MQTT Broker settings for Mosquitto
const char* mqtt_server = "10.0.0.80";  // Update with your broker's IP address
const int mqtt_port = 1884;             // Non-secure port

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const int sensorPin = A0;  // Analog pin for the sensor

void setup() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifi_attempts++;
    if (wifi_attempts > 30) {  // Give up after 30 attempts
      Serial.println();
      Serial.println("Failed to connect to WiFi after 30 attempts.");
      while (1) {
        delay(1000);
      }
    }
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  Serial.begin(9600);
  while (!Serial) {}  // Wait for Serial Monitor readiness
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  // Attempt connecting to WiFi until connection is established
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // Set MQTT broker details
  client.setServer(mqtt_server, mqtt_port);
}

void reconnectMQTT() {
  // Loop until we're connected to the MQTT broker
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a unique client ID
    String clientId = "ArduinoUnoR4Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect using the client ID
    if (client.connect(clientId.c_str())) {
      Serial.println(" connected!");
    } else {
      Serial.print("MQTT connect failed, state: ");
      Serial.println(client.state());
      Serial.println("Reattempting in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  // Ensure the MQTT connection is active
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  // Read sensor data
  int sensorValue = analogRead(sensorPin);
  char payload[10];
  dtostrf(sensorValue, 1, 2, payload);
  
  // Publish sensor reading to the MQTT topic
  if (client.publish("sensor/air_quality", payload)) {
    Serial.print("Published sensor value: ");
    Serial.println(payload);
  } else {
    Serial.println("Failed to publish sensor value");
  }
  
  // Wait for 5 seconds between publishes
  delay(5000);
}
