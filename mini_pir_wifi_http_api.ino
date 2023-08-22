// Wiring:
// Connect the Grove Mini PIR Motion Sensor to the NodeMCU as follows:
//
// VCC (Voltage) pin of the sensor to the 3.3V pin on the NodeMCU.
// GND (Ground) pin of the sensor to the GND pin on the NodeMCU.
// REL (Signal) pin of the sensor to any available GPIO pin on the NodeMCU (e.g., D3).
// Here's a text representation of the wiring connections:
//
// Sensor VCC -> NodeMCU 3.3V
// Sensor GND -> NodeMCU GND
// Sensor REL -> NodeMCU D4 (or any other GPIO pin)
//

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define PIR_PIN D4  // Change this to the pin you've connected the PIR sensor to


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
  pinMode(PIR_PIN, INPUT);  // Set the PIR pin as an input
}

int getMotionStatus() {
  int motionStatus = digitalRead(PIR_PIN);
  Serial.print("Motion: ");
  Serial.println(motionStatus);

  return motionStatus;
}

void sendDataToAPI(int motionStatusVal) {
  WiFiClient client;
  HTTPClient http;

  // Construct the complete URL
  String url = apiEndpoint;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String jsonData = "{\"motion\": " + String(motionStatusVal) + "}";
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
  int motionStatus = getMotionStatus();

  sendDataToAPI(motionStatus);

  delay(5000);  // Send data every 5 seconds
}
