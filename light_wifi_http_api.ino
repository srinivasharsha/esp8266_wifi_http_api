// Wiring:
// Connect the Grove Light Sensor to the NodeMCU as follows:
//
// VCC (Voltage) pin of the sensor to the 3.3V pin on the NodeMCU.
// GND (Ground) pin of the sensor to the GND pin on the NodeMCU.
// SIG (Signal) pin of the sensor to any available Analog pin on the NodeMCU (e.g., A0).
// Here's a text representation of the wiring connections:
//
// Sensor VCC -> NodeMCU 3.3V
// Sensor GND -> NodeMCU GND
// Sensor SIG -> NodeMCU AO (or any other Analog pin)
//

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LIGHT_PIN A0  // Connect the light sensor to NodeMCU A0

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
  analogReference(DEFAULT);  // Set the default analog reference
}

int getLightValue() {
  int lightValue = analogRead(LIGHT_PIN);
  Serial.print("Light Value: ");
  Serial.println(lightValue);

  return lightValue;
}

void sendDataToAPI(int lightValueVal) {
  WiFiClient client;
  HTTPClient http;

  // Construct the complete URL
  String url = apiEndpoint;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String jsonData = "{\"light\": " + String(lightValueVal) + "}";
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
  int lightValue = getLightValue();

  sendDataToAPI(lightValue);

  delay(5000);  // Send data every 5 seconds
}
