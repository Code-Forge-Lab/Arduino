#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "index.h"
#include "findLCD.h"


int WaterTempRequired = 35;
int WaterTempOut = 0;
int WaterTempIn = 0;
int counter2 = 0;
byte cMenu = 0;

String burnerState = "OFF";

#define LED_BUILTIN 1
bool __led_toggle = false;

unsigned long clock_1sec = 0; 

const char* http_username = "admin";
const char* http_password = "admin";

// Variable to store the received command
String receivedCommand;

AsyncWebServer server(80);    // Create an instance of the AsyncWebServer

int nCounter;

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
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

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.localIP());
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
        lcdPrint ("Failed to connect", true);
    } 
    else {
        Serial.println("connected...yeey friend:)");
        lcdPrint ("Connected!!", true);
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        lcdPrint ("Authenticate request", true);
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, NULL);
  });
  



server.on("/__adc_TEMP_REQUIRED", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(200, "text/html", String(WaterTempRequired)+"C");
  });


  server.on("/__adc_TEMP_HOT", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(200, "text/html", String(WaterTempIn));
  });

  server.on("/__adc_TEMP_COLD", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(200, "text/html", String(WaterTempOut));
  });




      server.on("/led_toggle", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.print("LED Toggled!!!");
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, __led_toggle);
    __led_toggle = !__led_toggle;
    request->send(200, "text/html", "");
  });


/// Change Required Temperature
  server.on("/__requiredTempPlus", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.print("Temp Required Increased!!!");
    WaterTempRequired = WaterTempRequired + 1;
    request->send(200, "text/html", "");
  });

    server.on("/__requiredTempMinus", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.print("Temp Required Decreased");
    WaterTempRequired = WaterTempRequired - 1;

    request->send(200, "text/html", "");
  });



  

 // Handle command submission
  server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request){
    String receivedCommand;
    if (request->hasParam("command", true)) {
      cMenu = 10;
      receivedCommand = request->getParam("command", true)->value();
      Serial.println("Received Command: " + receivedCommand);

      // Simulate processing and create a response
      String response = "Processed Command: " + receivedCommand;
      request->send(200, "text/plain", response); // Send response back
    } else {
      request->send(400, "text/plain", "Invalid Command");
    }
  });


  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    String response = "{";
    response += "\"WaterTempRequired\":" + String(WaterTempRequired) + ",";
    response += "\"WaterTempIn\":" + String(WaterTempIn) + ",";
    response += "\"WaterTempOut\":" + String(WaterTempOut);
    response += "}";
    request->send(200, "application/json", response);
  });



   // Handle the burner state request
  server.on("/burner", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state")) {
      burnerState = request->getParam("state")->value();
      Serial.println("Burner State: " + burnerState);
      request->send(200, "text/plain", "Burner State Updated to " + burnerState);
    } else {
      request->send(400, "text/plain", "Missing state parameter");
    }
  });



  
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
   if(cMenu<=1){
        lcdPrint ("Temp Required: "+ String(WaterTempRequired) + "C" , true , 0 , 0);

        lcdPrint ("Temp In: "+ String(WaterTempIn) + "C" , false , 0 , 1);
        WaterTempIn = WaterTempIn + 2;
        
        lcdPrint ("Temp Out: "+ String(WaterTempOut) + "C" , false , 0 , 2);
        WaterTempOut = WaterTempOut + 1;

        // lcdPrint ("nCounter: " + String(nCounter) + "c" , false , 0 , 2);

        // lcdPrint ("toggleBtn: " + String(__led_toggle) + "!" , false , 0 , 3);
        lcdPrint ("IP: " + WiFi.localIP().toString() + "!" , false , 0 , 3);
        
   }else{
     if(cMenu>1)cMenu--;
     lcdPrint ( String(receivedCommand) , true , 0 , 0);
   }

    if (WaterTempRequired < (WaterTempOut + WaterTempIn)/2 ) // reset values at selected tem[erature limit
       {
        WaterTempIn = 0;
        WaterTempOut = 0;
        Serial.print("Reseted By Requared Max Value !");
       }

    }
    
}