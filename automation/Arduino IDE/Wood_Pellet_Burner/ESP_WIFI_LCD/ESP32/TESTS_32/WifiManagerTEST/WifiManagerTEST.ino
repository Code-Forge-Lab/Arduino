#include <WiFi.h>
#include <WiFiManager.h>
const int flashButtonPin = 0;
void setup() {
  Serial.begin(115200);
  pinMode(flashButtonPin, INPUT);
  WiFiManager wifiMn;
  if (!wifiMn.autoConnect("MyESP32AP")) {
    Serial.println("Failed to connect and hit timeout");
    esp_restart();
    delay(1000);
  }
  Serial.println("Connected to Wi-Fi!");
}
void loop() {
  if (digitalRead(flashButtonPin) == LOW) {
    WiFiManager wifiMn;
    wifiMn.resetSettings();
    esp_restart();
    Serial.println("WiFi settings reset");
    delay(1000);
  }
  // Your code here
}