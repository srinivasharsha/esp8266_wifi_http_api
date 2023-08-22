#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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

void sendDataToAPI(const char* data) {
  HTTPClient http;

  http.begin(apiEndpoint);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Error in HTTP POST request");
  }

  http.end();
}

void loop() {
  // Your code here

  // Example data to send
  const char* jsonData = "{\"value\": 123}";

  sendDataToAPI(jsonData);

  delay(5000); // Send data every 5 seconds
}
