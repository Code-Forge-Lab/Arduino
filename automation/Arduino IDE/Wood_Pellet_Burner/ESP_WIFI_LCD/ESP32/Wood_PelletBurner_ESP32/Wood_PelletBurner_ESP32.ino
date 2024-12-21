/*
Program size: 326800 bytes
File system size: 1318001 bytes
File system used: 35642 bytes
Free space: 2585145 bytes
*/




#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "index.h"
#include "findLCD.h"


int counter = 0;
int counter1 = 0;
int counter2 = 0;


#define LED_BUILTIN 1
bool __led_toggle = false;

unsigned long clock_1sec = 0; 

const char* http_username = "admin";
const char* http_password = "admin";

AsyncWebServer server(80);    // Create an instance of the AsyncWebServer

int nCounter;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Variables to hold the state of the burner
float currentTemperature = 25.0; // Example starting temperature
bool burnerStatus = false;
int fireStrength = 2; // Levels: 1 (low), 2 (medium), 3 (high)
int fuelLeft = 75;    // Fuel percentage



// Function to simulate changing data
void updateSimulation() {
  if (burnerStatus) {
    currentTemperature += 0.1 * fireStrength; // Simulate heat increase
    fuelLeft = max(0, fuelLeft - 1);          // Simulate fuel consumption
  } else {
    currentTemperature -= 0.05; // Simulate cooling
  }
}

// Endpoint to send burner status as JSON
void handleStatus(AsyncWebServerRequest *request) {
  String json = "{";
  json += "\"temperature\":" + String(currentTemperature, 1) + ",";
  json += "\"burnerStatus\":" + String(burnerStatus ? "true" : "false") + ",";
  json += "\"fireStrength\":" + String(fireStrength) + ",";
  json += "\"fuelLeft\":" + String(fuelLeft);
  json += "}";
  request->send(200, "application/json", json);
}

// Endpoint to handle commands
void handleCommand(AsyncWebServerRequest *request) {
  if (request->hasParam("cmd")) {
    String cmd = request->getParam("cmd")->value();

    if (cmd == "toggleBurner") {
      burnerStatus = !burnerStatus;
    } else if (cmd == "boost") {
      fireStrength = 3; // Set to max fire strength
    } else if (cmd == "heatUp" && fireStrength < 3) {
      fireStrength++;
    } else if (cmd == "heatDown" && fireStrength > 1) {
      fireStrength--;
    } else {
      Serial.println("Received custom command: " + cmd);
      // Handle additional custom commands here
    }
    request->send(200, "text/plain", "OK");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}


void setup() {
    

     lcd.init();                       // Initialize the LCD
     lcd.backlight();                  // Turn on the backlight
     lcd.clear();
     lcdPrint ("Starting network" , true);
     lcdPrint ("configuration.", false , 0,1);

    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    //wm.resetSettings();

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
        lcdPrint ("Failed to connect", true);
    } 
    else {
        Serial.println("connected...yeey friend:)");
        lcdPrint ("Connected!!", true);
    }




// Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Define the /status endpoint
  server.on("/status", HTTP_GET, handleStatus);

  // Define the /command endpoint
  server.on("/command", HTTP_GET, handleCommand);

  
       // server.on("/adcFireValue", HTTP_GET, [](AsyncWebServerRequest * request) {
       //  request->send(200, "text/html", String(counter1));
       // });

       // server.on("/adcFuelValue", HTTP_GET, [](AsyncWebServerRequest * request) {
       //  request->send(200, "text/html", String(counter2));
       // });


     //    // Handle data endpoint
     // server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) {
     //     String json = "{\"adcFireValue\":" + String(counter1) + 
     //                   ",\"status\":\"" + burnerStatus + "\"}";
     //     request->send(200, "application/json", json);
     // });


    //     // Handle data endpoint
    // server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) {
    //     String json = "{\"adcFuelValue\":" + String(counter2) + 
    //                   ",\"status\":\"" + burnerStatus + "\"}";
    //     request->send(200, "application/json", json);
    // });

  //   server.on("/adc_value", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   nCounter ++;
  //   request->send(200, "text/html", String(nCounter));
  // });

  //     server.on("/led_toggle", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   Serial.println("LED Toggled!!!");
  //   // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //   digitalWrite(LED_BUILTIN, __led_toggle);
  //   __led_toggle = !__led_toggle;
  //   request->send(200, "text/html", "");
  // });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;

    Serial.println(inputMessage);
    request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, true);
}

void loop() {
  // nono bloking clock
if (((long)clock_1sec + 1000UL) < millis())
    {
      clock_1sec=millis();

      findLCD ();
      lcd.setCursor(0, 0);               // Set the cursor to the first column and first row

  lcdPrint ("Temperature: "+ String(counter1) + "C" , true , 0 , 0);

  lcdPrint ("Fire: "+ String(counter1) + "LX" , false , 0 , 1);
  counter1 = counter1 + 2;

  lcdPrint ("Fuel: " + String(counter2) + "gm" , false , 0 , 2);
  counter2 = counter2 + 3;

  lcdPrint ("toggleBtn: " + String(__led_toggle) + "!" , false , 0 , 3);


    
    }
}