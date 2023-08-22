  // Wiring:
  // Connect the Grove Ultrasonic Distance Sensor to the NodeMCU as follows:
  //
  // VCC (Voltage) pin of the sensor to the 3.3V pin on the NodeMCU.
  // GND (Ground) pin of the sensor to the GND pin on the NodeMCU.
  // SIG (Signal) pin of the sensor to any available GPIO pin on the NodeMCU (e.g., D2).
  //
  // Here's a text representation of the wiring connections:
  // Sensor VCC -> NodeMCU 3.3V
  // Sensor GND -> NodeMCU GND
  // Sensor SIG -> NodeMCU D2 (or any other GPIO pin)
  //

  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <Ultrasonic.h>

  #define TRIGGER_PIN D2  // Change this to the pin you've connected the sensor's SIG pin to
  #define ECHO_PIN D2     // Same as the trigger pin

  Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
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

  long getDistance() {
    long distance = ultrasonic.read();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    return distance;
  }

  void sendDataToAPI(long val) {
    WiFiClient client;
    HTTPClient http;

    // Construct the complete URL
    String url = apiEndpoint;

    http.begin(client, url);  // Use the new version of begin() with WiFiClient and URL
    http.addHeader("Content-Type", "application/json");
    String jsonData = "{\"distance\": " + String(val) + "}";
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

    sendDataToAPI(getDistance());

    delay(5000);  // Send data every 5 seconds
  }
