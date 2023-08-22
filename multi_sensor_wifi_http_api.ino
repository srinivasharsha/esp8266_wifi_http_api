// Wiring:
// Grove Ultrasonic Distance Sensor:
// VCC: Connect to NodeMCU 3.3V
// GND: Connect to NodeMCU GND
// SIG: Connect to NodeMCU D2

// Grove DHT11 Temperature & Humidity Sensor:
// VCC: Connect to NodeMCU 3.3V
// GND: Connect to NodeMCU GND
// SIG: Connect to NodeMCU D3

// Grove Mini PIR Motion Sensor:
// VCC: Connect to NodeMCU 3.3V
// GND: Connect to NodeMCU GND
// REL (Signal): Connect to NodeMCU D4

// Grove Light Sensor:
// VCC: Connect to NodeMCU 3.3V
// GND: Connect to NodeMCU GND
// Connect to NodeMCU A0

// Here's a text representation of the wiring connections:

// Ultrasonic Sensor VCC -> NodeMCU 3.3V
// Ultrasonic Sensor GND -> NodeMCU GND
// Ultrasonic Sensor SIG -> NodeMCU D2
// DHT11 Sensor VCC -> NodeMCU 3.3V
// DHT11 Sensor GND -> NodeMCU GND
// DHT11 Sensor SIG -> NodeMCU D3
// PIR Motion Sensor VCC -> NodeMCU 3.3V
// PIR Motion Sensor GND -> NodeMCU GND
// PIR Motion Sensor REL (Signal) -> NodeMCU D4
// Light Sensor VCC -> NodeMCU 3.3V
// Light Sensor GND -> NodeMCU GND
// Light Sensor -> NodeMCU A0
//

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ultrasonic.h>
#include <DHT.h>

#define TRIGGER_PIN D2
#define ECHO_PIN D2
#define DHT_PIN D3
#define PIR_PIN D4
#define LIGHT_PIN A0

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
DHT dht(DHT_PIN, DHT11);

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* apiEndpoint = "http://your-api-endpoint.com";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  pinMode(PIR_PIN, INPUT);
  analogReference(DEFAULT);  // Set the default analog reference
}

long getDistance() {
  long distance = ultrasonic.read();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

float getTemperature() {
  float temperature = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  return temperature;
}

float getHumidity() {
  float humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  return humidity;
}

int getMotionStatus() {
  int motionStatus = digitalRead(PIR_PIN);
  Serial.print("Motion: ");
  Serial.println(motionStatus);

  return motionStatus;
}

int getLightValue() {
  int lightValue = analogRead(LIGHT_PIN);
  Serial.print("Light Value: ");
  Serial.println(lightValue);

  return lightValue;
}

void sendDataToAPI(long distanceVal, float tempVal, float humidityVal, int motionStatusVal, int lightValueVal) {
  WiFiClient client;
  HTTPClient http;

  String url = apiEndpoint;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String jsonData = "{\"distance\": " + String(distanceVal) + ", \"temperature\": " + String(tempVal) + ", \"humidity\": " + String(humidityVal) + ", \"motion\": " + String(motionStatusVal) + ", \"light\": " + String(lightValueVal) + "}";
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Error in HTTP POST request");
  }

  http.end();
}

void loop() {
  long distance = getDistance();
  float temperature = getTemperature();
  float humidity = getHumidity();
  int motionStatus = getMotionStatus();
  int lightValue = getLightValue();

  sendDataToAPI(distance, temperature, humidity, motionStatus, lightValue);

  delay(5000);
}
