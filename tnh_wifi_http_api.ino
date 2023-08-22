// Wiring:
// Connect the Grove DHT11 Temperature & Humidity Sensor to the NodeMCU as follows:
//
// VCC (Voltage) pin of the sensor to the 3.3V pin on the NodeMCU.
// GND (Ground) pin of the sensor to the GND pin on the NodeMCU.
// SIG (Signal) pin of the sensor to any available GPIO pin on the NodeMCU (e.g., D3).
// Here's a text representation of the wiring connections:
//
// Sensor VCC -> NodeMCU 3.3V
// Sensor GND -> NodeMCU GND
// Sensor SIG -> NodeMCU D3 (or any other GPIO pin)
//

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>  // Include the DHT library

#define DHT_PIN D3  // Change this to the pin you've connected the DHT sensor's data pin to

  DHT dht(DHT_PIN, DHT11);  // Initialize the DHT sensor

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

void sendDataToAPI(float tempVal, float humidityVal) {
  WiFiClient client;
  HTTPClient http;

  // Construct the complete URL
  String url = apiEndpoint;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String jsonData = "{\"temperature\": " + String(tempVal) + ", \"humidity\": " + String(humidityVal) + "}";
  int httpResponseCode = http.POST(jsonData);  // Use jsonData instead of data

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Error in HTTP POST request");
  }

  http.end();
}

void loop() {
  float temperature = getTemperature();
  float humidity = getHumidity();

  sendDataToAPI(distance, temperature, humidity);

  delay(5000);  // Send data every 5 seconds
}
