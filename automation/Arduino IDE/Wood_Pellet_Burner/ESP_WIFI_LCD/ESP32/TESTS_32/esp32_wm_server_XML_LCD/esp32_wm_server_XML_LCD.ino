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

    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        lcdPrint ("Authenticate request", true);
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, NULL);
  });
  
    server.on("/adc_value", HTTP_GET, [](AsyncWebServerRequest * request) {
    nCounter ++;
    request->send(200, "text/html", String(nCounter));
  });

      server.on("/led_toggle", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("LED Toggled!!!");
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, __led_toggle);
    __led_toggle = !__led_toggle;
    request->send(200, "text/html", "");
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

  lcdPrint ("Temperature: "+ String(counter) + "C" , true , 0 , 0);
  counter = counter + 1;

  lcdPrint ("Fire: "+ String(counter1) + "LX" , false , 0 , 1);
  counter1 = counter1 + 2;

  lcdPrint ("nCounter: " + String(nCounter) + "c" , false , 0 , 2);

  lcdPrint ("toggleBtn: " + String(__led_toggle) + "!" , false , 0 , 3);


    

    }
    
}