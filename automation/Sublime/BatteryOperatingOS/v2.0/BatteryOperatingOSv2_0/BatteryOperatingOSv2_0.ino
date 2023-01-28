/*

Some of the GPIO pins are used while booting, so Pulling this pin HIGH or LOW can prevent NODEMCU from booting

GPIO0: It oscillates and stabilizes HIGH after ~100ms. Boot Failure if pulled LOW
GPIO1: LOW for ~50ms, then HIGH, Boot Failure if Pulled LOW.
GPIO2: It oscillates and stabilize HIGH after ~100ms, Boot Failure if Pulled LOW.
GPIO3: LOW for ~50ms, then HIGH.
GPIO9: Pin is HIGH at Boot.
GPIO10: Pin is HIGH at Boot.
GPIO15: LOW, Boot failure if Pulled HIGH
GPIO16: HIGH during Boot and Falls to ~1Volt. 

GPIO16: pin is high at BOOT
GPIO0: boot failure if pulled LOW
GPIO2: pin is high on BOOT, boot failure if pulled LOW
GPIO15: boot failure if pulled HIGH
GPIO3: pin is high at BOOT
GPIO1: pin is high at BOOT, boot failure if pulled LOW
GPIO10: pin is high at BOOT
GPIO9: pin is high at BOOT


/*
NodeMCU IO index vs ESP8266 pin
IO index  ESP8266 pin
    0     [*] GPIO16
    1     GPIO5 
    2     GPIO4 
    3     GPIO0
    4     GPIO2
    5     GPIO14  
    6     GPIO12
    7     GPIO13
    8     GPIO15
    9     GPIO3
    10    GPIO1
    11    GPIO9
    12    GPIO10
[*] D0(GPIO16) can only be used as gpio read/write. 
No support for open-drain/interrupt/pwm/i2c/ow.
https://nodemcu.readthedocs.io/en/master/en/modules/gpio/

 
void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}


*/


/*
#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;


static const uint8_t LED_BUILTIN = 16;
static const uint8_t BUILTIN_LED = 16;
*/

/*

Label  GPIO            Input             Output                  Notes
D0     GPIO16          no interrupt      no PWM or I2C          support HIGH at boot , used to wake up from deep sleep
D1     GPIO5           OK                OK                     often used as SCL (I2C)
D2     GPIO4           OK                OK                     often used as SDA (I2C)
D3     GPIO0           pulled up         OK                     connected to FLASH button, boot fails if pulled LOW
D4     GPIO2           pulled up         OK                     HIGH at boot , connected to on-board LED, boot fails if pulled LOW
D5     GPIO14          OK                OK                     SPI (SCLK)
D6     GPIO12          OK                OK                     SPI (MISO)
D7     GPIO13          OK                OK                     SPI (MOSI)
D8     GPIO15          pulled to GND     OK                     SPI (CS), Boot fails if pulled HIGH
RX     GPIO3           OK?!              RX pin                 HIGH at boot
TX     GPIO1           TX pin            OK?!                   HIGH at boot , debug output at boot, boot fails if pulled LOW
A0     ADC0            Analog Input      NO

*/


//Reasons
// [1]  When pullup, wont let upload a program , better use pulldown 
// [2]  Always pullup by default internal resistor
// [3] Fail to boot up, wdt reset when pinMode (input)

//    Warning
// {!1} --Disable Serial.print , and become regular GPIO INPUT/OUTPUT that can work with I2C Displays
// {!2} -- If using button , must use pullup RESISTOR 
//                                       //INPUT VCC-->[5.1k]-|-[btn]--GND                     INPUT VCC-->[btn]-|-[5.1k]--GND
//INPUT or buttons                               PULLUP                                                PULLDOWN

static const uint8_t DTX  = 1;  //                    {!1}                                                {!1}
static const uint8_t DRX  = 3;   //                   {!1}                                                {!1}
static const uint8_t D0   = 16;  //               Yes {!2}                                                 Yes  but can't be use as interupts
static const uint8_t D1   = 5;   //I2C            Yes                                                      Yes
static const uint8_t D2   = 4;   //I2C            Yes                                                      Yes
static const uint8_t D3   = 0;   //               Yes                                                      NO, Reason [2]
static const uint8_t D4   = 2;   //               Yes                                                      Yes
static const uint8_t D5   = 14;  //               Yes                                                      Yes
static const uint8_t D6   = 12;  //               Yes                                                      Yes
static const uint8_t D7   = 13;  //               Yes                                                      Yes
static const uint8_t D8   = 15;  //               NO,   Reason[1]                                          Yes


/*
static const uint8_t D9   = 10;   // SDD3         Must Be disconected wires from internals
static const uint8_t D10   = 9;   //SDD2          Must Be disconected wires from internals
static const uint8_t D11  = 8; //SDD1             NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D12  = 11; //SDCMD           NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D13   = 7; //SDD0            NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D14 = 6; //SDCLK             NO,   Reason[3]                                            NO,   Reason[3] 
*/

/*

 pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

*/




#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include "Voltmeter2.h"
#include "buttons.h"


/*  About program
Inspect where inv. is on and output strong 220vac
  GPIO16 (D0) = Inv.ReadAC+         ->testPoint_Inv,ReadAC
*/
const static uint8_t Inv_readAC = D0;
bool sensorDoInv_readAC = false;
/*
Relay Turn On Relay to 220v Output
Can control 220v relay and also read inv live, is completly on with "Feed_Separate"
  GPIO5 (D1) = Inv.Output220        ->Realy { 220v Out [r__/ __] }
*/
const static uint8_t Inv_Output220 = D1;
uint8_t maxDelay_Inv_Output220  = 16; //sec delay to pass power throw power relay from inverter
uint8_t delay_Inv_Output220;          //sec delay to pass power throw power relay from inverter
bool    doInv_Output220 = false;      //sec delay to pass power throw power relay from inverter 
/*
Relay Turn On Inverter
Turn on invite. by powering throw relay
  GPIO4 (D2) = Inv.On               ->Relay { Inv.On [r__/ __] }
*/
const static uint8_t Inv_On = D2;
uint8_t maxDelayAvoid_Inv_On = 40; //sec delay to turn to on inverter to fast, avoid toggle switching 
uint8_t delayAvoid_Inv_On = 0;    //sec delay to turn to on inverter to fast, avoid toggle switching 
bool    doAvoidInv_On = false;    //sec delay to turn to on inverter to fast, avoid toggle switching
/*
Turn on inv. prg by button
  GPIO0 (D3) = Prg.on.button
*/
const static byte Prg_on_button = D3;
bool buttonSET;
byte BUTTON_SET =  Prg_on_button;
buttonTimeout btnPrg_on (buttonSET,BUTTON_SET , true ,3000);
byte doPrg_on_button = false; // 
/*
Indicate status about program
  GPIO2 (D4) = LED_Indicator
*/
const static uint8_t LED_Indicator = D4;
/*
Read external signals from inverter where tells about successful working stage
  GPIO14(D5) = Inv.ReadSignal+      ->Contact Input
*/
const static uint8_t Inv_ReadSignal = D5;
bool sensorDoInv_ReadSignal = false;
/*
Read external signals from inverter where tells about successful working stage
  GPIO12(D6) = Prg.StopInv+         ->48-70V Input
*/
const static uint8_t Prg_StopInv = D6;
bool sensorDoPrg_StopInv = false;

/*
Swtich temperature sensor that give a contact then reached critical level
Prg.StopInvTemp _/_
*/
const static uint8_t Prg_StopInvTemp = D7; 
bool sensorPrg_StopInvTemp = false;


/*
--Analog Read--
  A0 = Read.Batery.Volt
*/
const static uint8_t Read_Battery_Volt = A0;
int sensorRead_Battery_Volt = 0;  // value read from the pot
//(uint8_t pin_input = A0 ,              float broken_voltage = 0.0        ,  float R1 = 100000.0 , float R2 = 10000.0  )
VoltMeter voltAvrBattery  (Read_Battery_Volt, 0.0 ,   680000.0 ,3900.0  );

        

// Function declaration
void funInv_On_then_Output220 (String x);
void oneSecTimer ();
void quarterSecondTimer ();
String fun_CmdRead (String);



//fast blink

uint8_t LED_IndicatorBlinkFast = 0;
uint8_t LED_IndicatorBlinkFast_Common = 4;

unsigned long clock_0_2sec = 0; // 0.2 seconds update
unsigned long clock_1secCounter = 0;// if x 5 then = 1 sec
unsigned long clock_1sec = 0 ; 

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

/// input commands from user
String cmd_received;
String cmd_msgOut;  // write to user what condition have hit on the webserver


// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = Inv_Output220;
const int output4 = Inv_On;

void setup() {
  Serial.begin(115200);
  Serial.println ("Starting setup . . .");
  // Initialize the output variables as outputs
  pinMode (Inv_readAC,           INPUT);
  pinMode (Inv_Output220,        OUTPUT);// output
  pinMode (Inv_On,               OUTPUT);// output
  pinMode (Prg_on_button,        INPUT);
  pinMode (LED_Indicator,        OUTPUT);// output
  pinMode (Inv_ReadSignal,       INPUT);
  pinMode (Prg_StopInv,          INPUT);
  pinMode (Prg_StopInvTemp,      INPUT);
  pinMode (Read_Battery_Volt,    INPUT);
  // Set outputs to LOW
 // digitalWrite(output5, LOW);
 // digitalWrite(output4, LOW);



  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  
  wifiManager.setConfigPortalTimeout(180);
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("BatteryOperationOS");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  //Serial.println (WiFiManager.localIP());
  // if you get here you have connected to the WiFi
  Serial.println("Connected________________________");
    
  server.begin();
}


   // myClass.getVal();
void loop(){
  
  WiFiClient client = server.available();   // Listen for incoming clients

   

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) 
    {            // loop while the client's connected
     // prgm
     btnPrg_on.scaning();
      oneSecTimer ();

      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
           
            
            // turns the GPIOs on and off
            if (!doAvoidInv_On && header.indexOf("GET /5/on") >= 0) {//sec delay to turn to on inverter to fast, avoid toggle switching
              Serial.println("GPIO 5 on");
             
               output5State = "on";

               // second relay that pass power throw power 220v relay from inverter and also turn on inverter
               funInv_On_then_Output220 ("on") ; 

            } else if (header.indexOf("GET /5/off") >= 0) {
              
              Serial.println("GPIO 5 off");
              output5State = "off";

               funInv_On_then_Output220 ("off") ; 
               // delayAvoid_Inv_On = 5;
             

            } else if (header.indexOf("GET /4/on") >= 0) {

              Serial.println("GPIO 4 on");
              output4State = "on";
              //digitalWrite(output4, HIGH);

            } else if (header.indexOf("GET /4/off") >= 0) {

              Serial.println("GPIO 4 off");
              output4State = "off";
             // digitalWrite(output4, LOW);

            } else if (header.indexOf ("/configurations?finput") > 0 ){
              
              cmd_received = header.substring(header.indexOf("/configurations?finput") + 23, header.indexOf (" HTTP") ); // //test cmd_received output
               // cmd_received = header.substring ( cmd_received.indexOf ("finput="), 67 );
               // cmd_received = header.substring( 3, 55 ); // //test raw cmd_received output
               //
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #77878A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color:MediumSeaGreen ;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>BatteryOperationOS</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Inv_On - State " + output5State + " Avoid: " + String(delayAvoid_Inv_On) + "</p>");
            
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              // indicate about turning on an Inverter
              if (!doAvoidInv_On)
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Inv Off</button></a></p>");
              else// give numeric timeout visualatation
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">"+String(delayAvoid_Inv_On)+"!</button></a></p>");
              //------------------------------------------------------

            } else {  //+(delay_Inv_Output220 > 0) ?+"P:"+String(delay_Inv_Output220): +"^"+
              
              if (delay_Inv_Output220 > 2)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">P:"+String(delay_Inv_Output220-2)+" Inv On</button></a></p>");
              else if (doInv_Output220)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">^Inv On</button></a></p>");
              else 
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">xInv On</button></a></p>");


            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>GPIO 4 - State " + output4State + " "+ String (delay_Inv_Output220) +" b:"+String (doInv_Output220)+"</p>" );
            client.println("<p> "+ getStatusText () +"</p>" );
            
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">off "+String (voltAvrBattery.voltage)+"v</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">on "+String (sensorRead_Battery_Volt)+" vbits</button></a></p>");
            }
            //Input configurations

        
              client.println ("<form action=\"/configurations\"><label for=\"finput\">First name:</label><input type=\"text\" id=\"finput\" name=\"finput\"><br><br><input type=\"submit\" value=\"Submit\"></form>");
              
              //test cmd_received output
              //client.println("<p>NVD" + cmd_received + "  inderxf "+ String (cmd_received.indexOf ("=")) + "  http>" + String (cmd_received.indexOf ("HTTP"))  + "</p>");
              client.println("<p>Pout> " + /*cmd_received*/  cmd_msgOut + "</p>");

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }

     
      
    }
    // Clear the header variable
    // fun_CmdRead (header); // doesnt work cleanly
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

    

  }else {


  btnPrg_on.scaning();

  oneSecTimer ();

  
  
   btnPrg_on.endScaning();


   
 }
}

void __main () {
 

  sensorDoInv_readAC      = digitalRead (Inv_readAC);          // need more then 170v dc-ac or short a test points
  sensorDoInv_ReadSignal  = digitalRead (Inv_ReadSignal);      // __/ __ need contact to activate
  sensorDoPrg_StopInv     = digitalRead (Prg_StopInv);         // need source and ground to activate opticouple
  sensorPrg_StopInvTemp   = digitalRead (Prg_StopInvTemp);
  // doPrg_on_button         = !digitalRead (Prg_on_button);
  doPrg_on_button         = btnPrg_on.getBtnTPressLongTime (); // hold 3 seconds to activate and return true

  //if (btnPrg_on.onlyReadPressedSignleTime()){
  //          doPrg_on_button = true;
  //}

  
  // btnPrg_on.endScaning ();
  voltAvrBattery.VoltageMeterUpdate ();

}

void quarterSecondTimer () { //0.2 second

   __main();

  if (((long)clock_0_2sec + 200UL) < millis())

  {
      //turnOnByButtonEvent ();
      clock_0_2sec= millis();
      clock_1secCounter  = clock_1secCounter + 1;


       // blink led while turning on a 220v relay 
      
      if (LED_IndicatorBlinkFast > 0){

          LED_IndicatorBlinkFast--;

      if (LED_IndicatorBlinkFast % 2 == 1  )//blick when turning on relay
          digitalWrite (LED_Indicator, HIGH);
      else
          digitalWrite (LED_Indicator, LOW);
        }


     //Sensor
      sensorRead_Battery_Volt = analogRead(Read_Battery_Volt);
     //sec delay to pass power throw power relay from inverter
     if (delay_Inv_Output220 < 3 &&  delay_Inv_Output220 >= 1) {

       doInv_Output220 = true; // to avoid cycling maxDelay 

       digitalWrite (Inv_Output220,HIGH);

    }
    //----------------------------------------------------------

    //sec delay to turn to on inverter to fast, avoid toggle switching
    if (delayAvoid_Inv_On > 0){

      doAvoidInv_On = true;
    }else{
      doAvoidInv_On = false;
    }
    //---------------------------------------------------------------

  }

}

void oneSecTimer () {

      

     quarterSecondTimer () ;
     
     

 if (clock_1secCounter >=5){
          clock_1secCounter = 0;

          
          if ( cmd_received.length() > 0)
          {
            fun_CmdRead (cmd_received);
             cmd_received = "";
          }


           if (delayAvoid_Inv_On > 0){
            delayAvoid_Inv_On--; // delay given to avoid turn on to fast
           }


          // second relay that pass power throw power 220v relay
          if (delay_Inv_Output220 > 0){
                
                delay_Inv_Output220--; // substrack when "inv_on" relay is activated
                

                // blink led while turning on a 220v relay 
                if (!doInv_Output220 && delay_Inv_Output220 % 2 == 1 || doInv_Output220 )//blick when turning on relay
                   digitalWrite (LED_Indicator, HIGH);
                 else
                   digitalWrite (LED_Indicator, LOW);

                 Serial.println ("delay_Inv_Output220: " + String (delay_Inv_Output220));
                 //--------------------
              }

          
          // Serial.println (getStatusText ());

          
    }

}

// turn on both relays
void funInv_On_then_Output220 (String x = "off") {

   if  (x == "on"){
     // digitalWrite (Inv_Output220, HIGH);
      digitalWrite (Inv_On, HIGH);

      if (delay_Inv_Output220 == 0 && !doInv_Output220 ){ // pass value only one time when was turned on at least one time
         delay_Inv_Output220 = maxDelay_Inv_Output220; 
         delayAvoid_Inv_On = maxDelayAvoid_Inv_On;  // activate protection against turning on to fast and multiple times
         Serial.println ("rejected");
       }

      
      }
   else{
      digitalWrite (Inv_Output220, LOW);
      digitalWrite (Inv_On, LOW);
      digitalWrite (LED_Indicator, LOW);

      delay_Inv_Output220 = 0;
      doInv_Output220 = false;
      // doPrg_on_button = false;


   }
}


String getText (String index, bool conditionNaming){
  if (conditionNaming)
    return ",  " + index + ": true";
  else
    return ",  " + index + ": false";

}
String getStatusText () {
 return   
            (". bits:" + String (sensorRead_Battery_Volt) +" = "
  + String  (voltAvrBattery.voltage) + "v.  "
  + getText ("DoInv_readAC D0 ~220v",sensorDoInv_readAC) 
  + getText ("  DoInv_ReadSignal D5_/_",sensorDoInv_ReadSignal)
  + getText ("  DoPrg_StopInv D6 +48v",sensorDoPrg_StopInv) 
  + getText ("  btnPrg_on D3 _/_", doPrg_on_button ) 
  + getText ("  StopInvTemp D7_/_", sensorPrg_StopInvTemp ) );


}

// int intDataTipe (int & changeOriginal , String  ) {

// }

String fun_CmdRead (String cmdRead /*input commands here*/) 
{
   String cmdGetSpecial; // special value simbol begins from '-'
   int    cmdGetSpecialInt; // value convert into integer 
   String cmdNRead = cmdRead;
    // Serial.println ("+++++++++++++++++++++++++++full link> " + cmdRead);

    if (cmdRead.length() > 0 ){// read comed arround
       
         // cmdRead = cmdRead.substring (0,cmdRead.length()-1); //to  remove incoming '\n'   
         // Serial.println ("------------found Text:" + cmdRead);
       
          // cmd_msgOut = ""; // reset

        if (cmdRead.lastIndexOf("-") > 0) {
          cmdGetSpecial = cmdRead.substring (cmdRead.lastIndexOf("-") + 1 ,cmdRead.length()); //separate text after-
          cmdRead = cmdRead.substring (0,cmdRead.indexOf("-"));

           if (cmdGetSpecial.indexOf ("+"))// spaces
         {
          Serial.println ("found+++ Spaces +++: " + String (cmdGetSpecial.indexOf ("+")) + "~" + String (cmdGetSpecial.length ()) );
          cmdGetSpecial = cmdGetSpecial.substring  ( 0 , cmdGetSpecial.indexOf ("+")); // delete leave text before spaces '+' begins
          Serial.println ("rslt: " + cmdGetSpecial );
         }

          //to integer
          cmdGetSpecialInt = (cmdGetSpecial.toInt());
          Serial.println ("comand: "  + cmdRead) ;
          Serial.println ("-value: " + cmdGetSpecial);
          Serial.println ("-valueInt: " + String (cmdGetSpecialInt));
          
         }


        


        if (cmdRead == "help")
         { 

           cmd_msgOut = "Awailable commands to input a credential for login in local network.\nssd-wifiname\npswd-password\nfastblink-int ";
           Serial.println (cmd_msgOut);
         }
         else if (cmdRead == "pswd" ){
           cmd_msgOut = "Registered wifi password:" + cmdGetSpecial;
           Serial.println (cmd_msgOut);

         }

          else if (cmdRead == "fastblink" ){
           cmd_msgOut = "fast blink value:" + cmdGetSpecial;
           Serial.println (cmd_msgOut);
           LED_IndicatorBlinkFast_Common = cmdGetSpecialInt;

         }

         else if (cmdRead == "ssd"){
            cmd_msgOut = "Registered ssd as wifi name:" + cmdGetSpecial ;
           Serial.println (cmd_msgOut);

         }

         else 
          {
           cmd_msgOut ="For more information type 'help' ( " + cmdRead + " - " + cmdGetSpecial +" ?)" ;
           Serial.println (cmd_msgOut);
          //Serial.println ("No command found DDDDDD:"  + cmdNRead + " ,^^ " + cmdRead.substring (cmdRead.indexOf ( " H") , cmdRead.length()) ) ;
          // Serial.println ("Received " +String (cmdRead.length()) + ">" + String (cmdRead) + String (cmdRead == "help") + " == help");
          }
       
           

      
    }

  return "";
}