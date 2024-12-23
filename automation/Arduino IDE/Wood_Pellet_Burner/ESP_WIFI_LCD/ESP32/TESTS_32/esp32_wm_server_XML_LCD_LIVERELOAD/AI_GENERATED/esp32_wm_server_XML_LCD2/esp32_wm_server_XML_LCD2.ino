#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"

const char* ssid = "network_glass";
const char* password = "migtukas";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Global variables for temperature and LED state
int requiredTemp = 25;  // Initial required temperature
int tempHot = 50;       // Initial hot temperature
int tempCold = 30;      // Initial cold temperature
bool ledState = false;  // Initial LED state

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure LED pin (optional: onboard LED GPIO2)
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);  // Turn LED off initially

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", index_html, NULL);// Send HTML from PROGMEM
  });

  // Endpoint to increase required temperature
  server.on("/__requiredTempPlus", HTTP_GET, [](AsyncWebServerRequest* request) {
    requiredTemp++;
    request->send(200, "text/plain", String(requiredTemp));
  });

  // Endpoint to decrease required temperature
  server.on("/__requiredTempMinus", HTTP_GET, [](AsyncWebServerRequest* request) {
    requiredTemp--;
    request->send(200, "text/plain", String(requiredTemp));
  });

  // Endpoint to get current required temperature
  server.on("/__adc_TEMP_REQUIRED", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", String(requiredTemp));
  });

  // Endpoint to get current hot temperature
  server.on("/__adc_TEMP_HOT", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", String(tempHot));
  });

  // Endpoint to get current cold temperature
  server.on("/__adc_TEMP_COLD", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", String(tempCold));
  });

  // Endpoint to toggle LED state
  server.on("/led_toggle", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("state")) {
      String state = request->getParam("state")->value();
      ledState = (state == "on");
      digitalWrite(2, ledState ? HIGH : LOW);
      request->send(200, "text/plain", "LED state: " + String(ledState ? "ON" : "OFF"));
    } else {
      request->send(400, "text/plain", "Missing 'state' parameter");
    }
  });

  // Start the server
  server.begin();
}

void loop() {
  // Nothing required in the loop for AsyncWebServer
}
