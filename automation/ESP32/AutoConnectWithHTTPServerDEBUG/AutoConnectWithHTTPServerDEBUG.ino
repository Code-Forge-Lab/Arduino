// #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// //needed for library
// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include "WiFiManager.h"         //https://github.com/tzapu/WiFiManager
//std::unique_ptr<ESP8266WebServer> server;

/////////////////////////////
#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Update.h>


AsyncWebServer server(80);    // Create an instance of the AsyncWebServer


void handleRoot() {
  request->send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->uri();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";
  for (uint8_t i = 0; i < request->args(); i++) {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }
  request->send(404, "text/plain", message);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  wifiManager.resetSettings();

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  //wifiManager.autoConnect("AutoConnectAP");
  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect();


  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  
  request.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  request->on("/", handleRoot);

  request->on("/inline", []() {
    request->send(200, "text/plain", "this works as well");
  });

  request->onNotFound(handleNotFound);

  request->begin();
  Serial.println("HTTP request started");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  request->handleClient();
}
