// #include <WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
 #include <ESP8266WebServer.h>
#include "webpageCode.h";

WiFiServer server(80);
const int flashButtonPin = 0;

void webpage()
{
  server.send(200,"text/html", webpageCode);
}
WiFiManager wifiManager;
void setup() {
  Serial.begin(115200);
  pinMode(flashButtonPin, INPUT);

// Uncomment and run it once, if you want to erase all the stored information
  // wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  
  wifiManager.setConfigPortalTimeout(280);
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Inverter Manager");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();


  server.on("/", webpage);
  Serial.println("Connected to Wi-Fi!");
}
void loop() {
  // if (digitalRead(flashButtonPin) == LOW) {
  //   WiFiManager wifiMn;
  //   wifiMn.resetSettings();
  //   esp_restart();
  //   Serial.println("WiFi settings reset");
  //   delay(1000);
  // }
  // Your code here


WiFiClient client = server.available();   // Listen for incoming clients

server.handleClient();


}