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
//                                       //INPUT VCC-->[5.1k]-|-[btn]--GND      ser               INPUT VCC-->[btn]-|-[5.1k]--GND
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



//A
// #include <ESP8266WiFi.h>

// #include <DNSServer.h> // not needy
// #include <ESP8266WebServer.h> // not needy

//B
#include <WiFiManager.h>  //         // https://github.com/tzapu/WiFiManager
#include "Voltmeter2.h"
#include "buttons.h"
#include "EEPROM32.h"

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
uint8_t maxDelay_Inv_Output220  = 5; //sec delay to pass power throw power relay from boiler
uint8_t delay_Inv_Output220;          //sec delay to pass power throw power relay from boiler
bool    doInv_Output220 = false;      //sec delay to pass power throw power relay from boiler 
/*
Relay Turn On Inverter
Turn on invite. by powering throw relay
  GPIO4 (D2) = Inv.On               ->Relay { Inv.On [r__/ __] }
*/
const static uint8_t Inv_On = D2;
uint8_t maxDelayAvoid_Inv_On = 24; //sec delay to turn to on boiler to fast, avoid toggle switching 
uint8_t delayAvoid_Inv_On = 0;    //sec delay to turn to on boiler to fast, avoid toggle switching 
bool    doAvoidInv_On = false;    //sec delay to turn to on boiler to fast, avoid toggle switching
/*
Turn on inv. prg by button
  GPIO0 (D3) = Prg.on.button
*/
const static byte Prg_on_button = D3;
bool buttonSET;
byte BUTTON_SET =  Prg_on_button;
// buttonTimeout btnPrg_on (buttonSET,BUTTON_SET , true ,3000);
bool sensorProg_on_button = false; // readDigital
bool doPrg_on_button = false; // 
byte delayPrg_on_button = 0; // counter where begins to give a true signal
byte maxDelayPrg_on_button = 8; // seconds
/*
Indicate status about program
  GPIO2 (D4) = LED_Indicator
*/
const static uint8_t LED_Indicator = D4;
/*
Read external signals from boiler where tells about successful working stage
  GPIO14(D5) = Inv.ReadSignal+      ->Contact Input
*/
const static uint8_t Inv_ReadSignal = D5;
bool sensorDoInv_ReadSignal = false;
/*
Read external signals from boiler where tells about successful working stage
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
VoltMeter voltAvrBattery  (Read_Battery_Volt, 0.0 ,   2000000.0 ,33000.0  );
byte minBatVlt = 15;
byte maxBatVlt = 16;
byte fixBatVlt = 100; // 1 equil 0.01 * 5100 = 510  // fix voltage in 255 range
bool doBatMaxVltReached = false; // enable when maximum voltage was reached and disable when minimum voltage is gained
byte maxBatVltSustained_sec = 20; //  if not interuption in 20-60 seconds have in reached by maximum voltage range then
byte maxBatVltSustained_cnt = 0; //  keep count of maxBatVltSustained_sec
bool doBatIsLow = false; // save condition about battery low voltage in the scope
bool doBatBeHigh = false; // save condition about battery high voltage in the scope
bool doReactInBatVlt = true; // react in battery automaticly turn on boiler by changend voltage;

int turnOffTimer = 0 ;// to avoid turning on and very fast turning off a boiler thant create junp in voltage at nearly fully charget batery 
int turnOffTimerMaxConst = 10; // not changing base value
int turnOffTimerMax = turnOffTimerMaxConst ;// seconds 
byte turnOffTimerUser = 1; // store addition timer value , additional for a turnOffTimerMax      

// Function declaration
void funInv_On_then_Output220 (String x = "off" , bool silence = false);
void oneSecTimer ();
void quarterSecondTimer ();
String fun_CmdRead (String);
void setBatVltRange (uint8_t , uint8_t );
bool avoidBatVltOutOfRangeThenMemCommit(); // avoid inputed voltage to be out of range;
void getmemBatVlt ();
void funTurnInvAutomaticallyByVoltage ();
void funTurnOffTimer(bool enableTimer = false);

// EEPROM memory address
int16_t memMinBatVlt = 1;
int16_t memMaxBatVlt = 2;
int16_t memfixVltR   = 3;
int16_t memReactInBatVlt   = 4;
int16_t memturnOffTimer   = 5;

//fast blink
bool    LED_IndicatorBlink     = false;
uint8_t LED_IndicatorBlinkFast = 0;
uint8_t LED_IndicatorBlinkFast_Common = 4;
uint8_t LED_IndicatorBlinkFast_Long = 8;
uint8_t LED_IndicatorBlinkFast_CommonShort = 2;

unsigned long clock_0_2sec = 0; // 0.2 seconds update
unsigned long clock_1secCounter = 0;// if x 5 then = 1 sec
unsigned long clock_1sec = 0 ; 

// Set web server port number to 80
WiFiServer server(80);


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Variable to store the HTTP request
String header;

/// input commands from user
String cmd_received;
String cmd_msgOut;  // write to user what condition have hit on the webserver


// Auxiliar variables to store the current output state
String output5StateInvOutput = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = Inv_Output220;
const int output4 = Inv_On;


  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  

void setup() {
  Serial.begin(115200);
  Serial.println ("Starting setup . . .");

  EEPROM32_INIT( 10);
 delay (10);
 getmemBatVlt ();

 if (readMemoryByte(memfixVltR)>=255) {writeMemory(memfixVltR,(byte)100); Serial.println ("Writing memfixVltR: 100");} else voltAvrBattery.FixVltR(readMemoryByte(memfixVltR)); // Works as potentiometer
 if (readMemoryByte(memReactInBatVlt)>=255) {writeMemory(memReactInBatVlt,true); Serial.println ("Writing memReactInBatVlt: true");} else doReactInBatVlt = (bool)(readMemoryByte(memReactInBatVlt)); // Works as auto on
 if (readMemoryByte(memturnOffTimer)>=255) {writeMemory(memturnOffTimer,(byte)1); Serial.println ("Writing memturnOffTimer: 1");} else turnOffTimerUser = (byte)(readMemoryByte(memturnOffTimer)); 

 if (doReactInBatVlt)        output4State = "on"; // change graphical user interface

 delay (10);
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

//A
 
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
  



  //B
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  //WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }


  //Serial.println (WiFiManager.localIP());
  // if you get here you have connected to the WiFi
  Serial.println("Connected________________________");
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


   // myClass.getVal();
void loop(){
  
  WiFiClient client = server.available();   // Listen for incoming clients

   

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis(); 
     // prgm
     // btnPrg_on.scaning();
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
            if (!doAvoidInv_On && header.indexOf("GET /5/on") >= 0) {//sec delay to turn to on boiler to fast, avoid toggle switching
              Serial.println("GPIO 5 on");
             
               // output5StateInvOutput = "on"; // alredy in funInv_On_then_Output220 function

               // second relay that pass power throw power 220v relay from boiler and also turn on boiler
              if (!doBatIsLow) // turn on boiler when no battery voltage is to low
               funInv_On_then_Output220 ("on") ; 
              else
               delayAvoid_Inv_On = maxDelayAvoid_Inv_On;  // activate protection against turning on to fast and multiple times


            } else if (header.indexOf("GET /5/off") >= 0) {
              
              Serial.println("GPIO 5 off");
              // output5StateInvOutput = "off"; // alredy in funInv_On_then_Output220 function

              
               funInv_On_then_Output220 ("off") ; 
               // delayAvoid_Inv_On = 5;
             

            } else if (header.indexOf("GET /4/on") >= 0) {

              Serial.println("GPIO 4 on");
              output4State = "on";
              //digitalWrite(output4, HIGH);
              writeMemory(memReactInBatVlt,true);
              doReactInBatVlt = true;

            } else if (header.indexOf("GET /4/off") >= 0) {

              Serial.println("GPIO 4 off");
              output4State = "off";
             // digitalWrite(output4, LOW);
              writeMemory(memReactInBatVlt,false);
              doReactInBatVlt = false;

            } else if (header.indexOf ("/configurations?finput") > 0 ){
              
              cmd_received = header.substring(header.indexOf("/configurations?finput") + 23, header.indexOf (" HTTP") ); // //test cmd_received output
               // cmd_received = header.substring ( cmd_received.indexOf ("finput="), 67 );
               // cmd_received = header.substring( 3, 55 ); // //test raw cmd_received output
               //
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_CommonShort;
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
            client.println("<body><h1>BoilerHeatingSolar</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            // client.println("<p>Inv_On - State " + output5StateInvOutput + " Avoid: " + String(delayAvoid_Inv_On) + "</p>");
            
            // If the output5StateInvOutput is off, it displays the ON button       
            if (output5StateInvOutput=="off") {
              // indicate about turning on an Inverter
             if (!doAvoidInv_On)
                  if(doBatBeHigh)
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Inv Off / High Battery</button></a></p>");
                  else
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Inv Off</button></a></p>");
              else// give numeric timeout visualatation
                 if(!doBatIsLow)
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">"+String(delayAvoid_Inv_On)+"!</button></a></p>");
                 
                  else
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Low Battery!</button></a></p>");
              //------------------------------------------------------

            } else {  //+(delay_Inv_Output220 > 0) ?+"P:"+String(delay_Inv_Output220): +"^"+
              
              if (delay_Inv_Output220 > 2)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">P:"+String(delay_Inv_Output220-2)+" Inv On</button></a></p>");
              else if (turnOffTimer > 0 && doBatIsLow)
                  client.println("<p><a href=\"/5/off\"><button class=\"button\">Off After "+String(turnOffTimer)+"</button></a></p>");

              else if (doInv_Output220)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">^Inv On</button></a></p>");
             
              else 
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">xInv On</button></a></p>");


            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            // client.println("<p>GPIO 4 - State " + output4State + " "+ String (delay_Inv_Output220) +" b:"+String (doInv_Output220)+"</p>" );
            // client.println("<p> "+ getStatusText () +"</p>" );
            
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Disabled Auto</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Activated "+String (voltAvrBattery.voltage)+"v</button></a></p>");
            }
            //Input configurations

        
              client.println ("<form action=\"/configurations\"><label for=\"finput\">cmd:</label><input type=\"text\" id=\"finput\" name=\"finput\"><br><br><input type=\"submit\" value=\"Submit\"></form>");
              
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


  // btnPrg_on.scaning();

  oneSecTimer ();

  
  
   // btnPrg_on.endScaning();


   
 }
}

void __main () {
 

  sensorDoInv_readAC      = digitalRead (Inv_readAC);          // need more then 170v dc-ac or short a test points
  sensorDoInv_ReadSignal  = digitalRead (Inv_ReadSignal);      // __/ __ need contact to activate
  sensorDoPrg_StopInv     = digitalRead (Prg_StopInv);         // need source and ground to activate opticouple
  sensorPrg_StopInvTemp   = digitalRead (Prg_StopInvTemp);
  sensorProg_on_button    = !digitalRead (Prg_on_button);
  // sensorProg_on_button         = btnPrg_on.getBtnTPressLongTime (); // hold 3 seconds to activate and return true

  //if (btnPrg_on.onlyReadPressedSignleTime()){
  //          sensorProg_on_button = true;
  //}

  
  // btnPrg_on.endScaning ();
  

}

void quarterSecondTimer () { //0.2 second

   __main();

  if (((long)clock_0_2sec + 200UL) < millis())

  {
      //turnOnByButtonEvent ();
      clock_0_2sec= millis();
      clock_1secCounter  = clock_1secCounter + 1;

      voltAvrBattery.VoltageMeterUpdate (true); // update with each passed clock
      

      if (output5StateInvOutput == "on")
          digitalWrite (Inv_On, HIGH);

       // blink led while turning on a 220v relay 
      
      if (LED_IndicatorBlinkFast > 0){
          LED_IndicatorBlinkFast--;
          LED_IndicatorBlink = !LED_IndicatorBlink;
        
      if (!LED_IndicatorBlink)//blick when turning on relay
        {
          digitalWrite (LED_Indicator, LOW);

        }else{
          digitalWrite (LED_Indicator, HIGH);
        }

      }


    


     //Sensor
      sensorRead_Battery_Volt = analogRead(Read_Battery_Volt);



     //sec delay to pass power throw power relay from boiler
     if (delay_Inv_Output220 < 3 &&  delay_Inv_Output220 >= 1) {

       doInv_Output220 = true; // to avoid cycling maxDelay 

       digitalWrite (Inv_Output220,HIGH);

    }
    //----------------------------------------------------------

    //sec delay to turn to on boiler to fast, avoid toggle switching
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

          




//---------------// React to turning on a boiler by voltage range ------------//660


      if (doReactInBatVlt/*<auto on condition from user*/ && !doBatMaxVltReached && (voltAvrBattery.voltage >= maxBatVlt) ) // turn on a boiler
         {
           Serial.println ("Condition: senscor " + String (voltAvrBattery.voltage) +"v >= batery then max " + String ( maxBatVlt) + "v " );
           funTurnOffTimer(true);

          if (maxBatVltSustained_cnt <= maxBatVltSustained_sec) // keep counting 
              maxBatVltSustained_cnt++;
              

            Serial.println ("sustained max voltage: " + String (maxBatVltSustained_cnt) );
            doBatBeHigh = true; // sub condition, telling about high voltage at the moment
          if (maxBatVltSustained_cnt >= maxBatVltSustained_sec) // when voltage was sustained more then 60 seconds when turn on a boiler 
             {
              doBatMaxVltReached = true;
              funInv_On_then_Output220 ("on",false);
              Serial.println ("turn on inv when voltage is in healthy level");
            }
         }
      else if (voltAvrBattery.voltage <= minBatVlt)
      {

           if (turnOffTimer > 0)
           {  
              Serial.print ("turnOffTimer: " + String (turnOffTimer) + " , ");
                 turnOffTimer--;
                
           } else {
                 doBatMaxVltReached = false;
                 maxBatVltSustained_cnt = 0;
                 funInv_On_then_Output220 ("off",false);
           }

         
          

          if (!doReactInBatVlt) Serial.print ("auto mode is disabled and / ");
          Serial.println ("inv is off when low battery");

          doBatIsLow = true;
          doBatBeHigh = false;
      }
      else // if no any voltage is awailable
      {
        maxBatVltSustained_cnt = 0;
        doBatIsLow = false;
        doBatBeHigh = false;

        if (!doReactInBatVlt) Serial.print ("auto mode is disabled and / ");
        Serial.println ("no battery condition");
        funTurnOffTimer(true);
      }





//---------------Program Button are pressed------------------------------//


           

           if (sensorProg_on_button)
           {
              Serial.println ("Pressing a button " + String (delayPrg_on_button) + "  %  " + String (delayPrg_on_button % 2 == 1));

              if (delayPrg_on_button < maxDelayPrg_on_button){
                  delayPrg_on_button ++;

                              // when pressing a button led inditcator should blink normaly when reached doPrg_on_button for a posetive result
                      if ( delayPrg_on_button % 2== 1  ) // then button was successfully pressed all the way down
                          digitalWrite (LED_Indicator, LOW);
                      else
                          digitalWrite (LED_Indicator , HIGH);

                }

                if (delayPrg_on_button == maxDelayPrg_on_button -1) {
                    doPrg_on_button = !doPrg_on_button; // here change bolean condition

                    if (doPrg_on_button) // turn on boiler and passing power relay
                        funInv_On_then_Output220 ("on");
                     else 
                        funInv_On_then_Output220 ("off");

                  }

              else
                  if (delayPrg_on_button == maxDelayPrg_on_button ){ // fast blink then reached a condtion 
                    LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common; 
                    Serial.print ("  it fast blinks +++  button: " + String (doPrg_on_button));
                   }

             


              




           } else {
             delayPrg_on_button = 0;
           }


///////////////////////////// // second relay that pass power throw power 220v relay
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

void funTurnOffTimer (bool emableTimer) {/// turn off timer calculation
      turnOffTimerMax = (int)turnOffTimerMaxConst * ((int)turnOffTimerUser);
   if (emableTimer) turnOffTimer = turnOffTimerMax;
}


// turn on both relays
void funInv_On_then_Output220 (String x  , bool silence) {

   if  (x == "on"){
     // digitalWrite (Inv_Output220, HIGH);
      // digitalWrite (Inv_On, HIGH);
      output5StateInvOutput = "on"; 
      funTurnOffTimer (true);

      if (delay_Inv_Output220 == 0 && !doInv_Output220 ){ // pass value only one time when was turned on at least one time
         delay_Inv_Output220 = maxDelay_Inv_Output220; 
         delayAvoid_Inv_On = maxDelayAvoid_Inv_On;  // activate protection against turning on to fast and multiple times
         doPrg_on_button = true; // only for in sinck with website button
         
        if (silence)  Serial.println ("funInv_On_then_Output220:on");
       }

      
      } 
   else{

      digitalWrite (Inv_Output220, LOW);
      digitalWrite (Inv_On, LOW);
      digitalWrite (LED_Indicator, LOW);
      output5StateInvOutput = "off"; // is opposite
       if (silence) Serial.println ("funInv_On_then_Output220:off");
      delay_Inv_Output220 = 0;
      doInv_Output220 = false;
      doPrg_on_button = false;
      doBatMaxVltReached = false;


   }
}

void funTurnInvAutomaticallyByVoltage () {

  // if ( voltAvrBattery.voltage < minBatVlt){

  // }

  // if (voltAvrBattery.voltage > maxBatVlt)
}

String getText (String index, bool conditionNaming){
  if (conditionNaming)
    return ",  " + index + ": true";
  else
    return ",  " + index + ": false";

}


bool isValidNumber(String str){
    bool isNum=false;
    for(byte i=0;i<str.length();i++)
    {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if(!isNum) return false;
    }
    return isNum;
}

 

String getStatusText () {
 return   
            (". bits:" + String (sensorRead_Battery_Volt) +" = "
  + String  (voltAvrBattery.voltage) + "v.  "
  + getText ("DoInv_readAC D0 ~220v",sensorDoInv_readAC) 
  + getText ("  Inv_ReadSignal D5_/_",sensorDoInv_ReadSignal)
  + getText ("  Prg_StopInv D6",sensorDoPrg_StopInv) 
  + getText ("  StopInvTemp D7_/_", sensorPrg_StopInvTemp )
  + getText ("  btnPrg_on D3 _/_", doPrg_on_button ) 
  + getText ("  doBatMaxVltReached", doBatMaxVltReached ) 
  + getText ("  doReactInBatVlt", doReactInBatVlt ) 
  + "]]" );

            


}

// int intDataTipe (int & changeOriginal , String  ) {

// }

String fun_CmdRead (String cmdRead /*input commands here*/) 
{
   String cmdGetSpecial; // special value simbol begins from '-'
   int    cmdGetSpecialInt; // value convert into integer 
   bool   cmdIsValidInt = false;
   String cmdNRead = cmdRead;
   bool sP = false;
    // Serial.println ("+++++++++++++++++++++++++++full link> " + cmdRead);

    if (cmdRead.length() > 0 ){// read comed arround
       
         // cmdRead = cmdRead.substring (0,cmdRead.length()-1); //to  remove incoming '\n'   
         // Serial.println ("------------found Text:" + cmdRead);
       
           cmd_msgOut = ""; // reset

        if (cmdRead.lastIndexOf("-") > 0) {
          cmdGetSpecial = cmdRead.substring (cmdRead.lastIndexOf("-") + 1 ,cmdRead.length()); //separate text after-
          cmdRead = cmdRead.substring (0,cmdRead.indexOf("-"));

           if (cmdGetSpecial.indexOf ("+"))//remove spaces
         {
          Serial.println ("found+++ Spaces +++: " + String (cmdGetSpecial.indexOf ("+")) + "~" + String (cmdGetSpecial.length ()) );
          cmdGetSpecial = cmdGetSpecial.substring  ( 0 , cmdGetSpecial.indexOf ("+")); // delete leave text before spaces '+' begins
          Serial.println ("rslt: " + cmdGetSpecial );
         }

          //to integer
          cmdGetSpecialInt = (cmdGetSpecial.toInt());

          // check in string is a valid integer without any characters
          cmdIsValidInt = isValidNumber (cmdGetSpecial);

          Serial.println ("comand: "  + cmdRead) ;
          Serial.println ("-value: " + cmdGetSpecial);
          Serial.println ("-valueInt: " + String (cmdGetSpecialInt));
          Serial.println (getText ("is integer data type: ", isValidNumber (cmdGetSpecial)  ));
          
         }


        


        if (cmdRead == "help")
         { 

           cmd_msgOut = "Awailable commands:,fixVltR-byte,clear,fixTurnOffTimer-byte,maxBatVlt-byte,minBatVlt-byte,status,restart,resetWifi-intpswrd, ";
           Serial.println (cmd_msgOut);
         }
         else if (cmdRead == "fixVltR" ){
           cmd_msgOut = "Registered fix voltage of R resistor 1 byte is equal -> 0.01: ";

            if (cmdIsValidInt) 
            {  
                LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                voltAvrBattery.FixVltR((byte)cmdGetSpecialInt); 
                cmd_msgOut+="["+  String ((byte)cmdGetSpecialInt) + "]" + " a value: " + String (voltAvrBattery.getFixVltR());
               
                writeMemory (memfixVltR, (byte)cmdGetSpecialInt);
             } else { cmd_msgOut += " [not a byte],stored: "+ String (readMemoryByte (memfixVltR)) + "b a value: " +  String (voltAvrBattery.getFixVltR()) + "R" ;};

           Serial.println (cmd_msgOut);
         }


          else if (cmdRead == "fixTurnOffTimer" ){
            funTurnOffTimer (); // refresh calculation values
         if (cmdIsValidInt) 
            {  
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
              turnOffTimerUser = (byte)cmdGetSpecialInt;
              funTurnOffTimer (); // refresh calculation valu es
              cmd_msgOut+=" fixTurnOffTimer  a value: " + String (turnOffTimerUser) + " as " + String (turnOffTimerMax) + "s timer ";
              writeMemory(memturnOffTimer,(byte)turnOffTimerUser);

            } 
            else {cmd_msgOut+="Failed register fixTurnOffTimer a mem value: " + String (turnOffTimerUser) + " as " + String (turnOffTimerMax) + "s timer max ";};
         }

         else if (cmdRead == "clear" ){
           cmd_msgOut = "";
            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
         }

         else if (cmdRead == "status"){

            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
            cmd_msgOut = "Received maxBatVlt:" + String (maxBatVlt) + " minBatVlt:" + String (minBatVlt) + getStatusText () ;
            Serial.println (cmd_msgOut);
            

         }
         else if (cmdRead == "resetWifi"){
            cmd_msgOut = "Received, reset wifi settings:";
             if (cmdIsValidInt)
             {
               
                if (cmdGetSpecialInt == 1357) {
                      LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                      Serial.println (cmd_msgOut);
                      wifiManager.resetSettings();
                      ESP.restart();
                }else
                  cmd_msgOut+= " [wrong password!]";
             }else
                  cmd_msgOut+= " [enter a numeric password!]";
            
         }
          else if (cmdRead == "restart"){
            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
            cmd_msgOut = "Received, start restart os:";
            Serial.println (cmd_msgOut);
            ESP.restart();
            
         }
         else if (cmdRead == "minBatVlt"){
           
             if (cmdIsValidInt){
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 minBatVlt = cmdGetSpecialInt;
                  
               if (avoidBatVltOutOfRangeThenMemCommit  () )
                     {cmd_msgOut = ("Registered minimum battery voltage: " + cmdGetSpecial  ); Serial.println (cmd_msgOut);}
               else
                     {cmd_msgOut = ("Failed to registered minimum battery voltage, out of range: " + cmdGetSpecial)  ; Serial.println (cmd_msgOut);}

                 Serial.println ("min out");

             }
            else 
              sP = true; // failed to proceed whithout a proper number

         }
         else if (cmdRead == "maxBatVlt"){
            
           
            if (cmdIsValidInt){
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 maxBatVlt = cmdGetSpecialInt;
                if ( avoidBatVltOutOfRangeThenMemCommit  ()  )
                  {cmd_msgOut = ("Registered maximum battery voltage:" + cmdGetSpecial) ; Serial.println (cmd_msgOut);}
                else
                  {cmd_msgOut = ("Failed to registered maximum battery voltage, out of range:") + cmdGetSpecial ; Serial.println (cmd_msgOut);}

                 Serial.println ("max out");
             }
            else
              sP = true; // failed to proceed whithout a proper number
         }

         else 
          {
           cmd_msgOut ="For more information type 'help' ( " + cmdRead + " - " + cmdGetSpecial +" ?)" ;
           Serial.println (cmd_msgOut);
          //Serial.println ("No command found DDDDDD:"  + cmdNRead + " ,^^ " + cmdRead.substring (cmdRead.indexOf ( " H") , cmdRead.length()) ) ;
          // Serial.println ("Received " +String (cmdRead.length()) + ">" + String (cmdRead) + String (cmdRead == "help") + " == help");
          }
       
           


              if (sP) 
              {
                      Serial.println ("Unable to proceed");
                      cmd_msgOut+= " [err:Unable to proceed , no integer]";
              }


    }

  return "   ";
}



void setBatVltRange (uint8_t minBatVlt_ , uint8_t maxBatVlt_) // not in use
{

  minBatVlt = minBatVlt_;
  maxBatVlt = maxBatVlt_;

  if (minBatVlt > maxBatVlt) // if minimum voltage is higher then maximum then make as same level to with
  {
    maxBatVlt = minBatVlt_ + 1;
    Serial.println ("maximum voltage was reseted");
  }

  // if (maxBatVlt > ) // voltage range
}






bool avoidBatVltOutOfRangeThenMemCommit (/*String name, byte value*/){
  bool itsDone = true;// chenk if all condition passed  successfully

   if (minBatVlt > maxBatVlt) // if minimum voltage is higher then maximum then make as same level to with
  {
    maxBatVlt = minBatVlt + 1;
    Serial.println ("minimum voltage was higher then expected, reseted");
    itsDone = false;
  }

 

      if (minBatVlt < 10 &&  maxBatVlt <= 16)
      {
        Serial.println ("12v mode");
      }
 else if (minBatVlt < 24 &&  maxBatVlt <= 32)
      {
        Serial.println ("24v mode");

      }
 else if (minBatVlt < 36 &&  maxBatVlt <= 48)
      {
        Serial.println ("36v mode");

      }
 else if (minBatVlt < 540 &&  maxBatVlt <= 640)
      {
        Serial.println ("48v mode");

      }
  else
  {
         itsDone = false;
    Serial.println ("doesnt found range in 'avoidBatVltOutOfRange' funcion");
  } 

  if (itsDone) // prepare to commit into EEPROM memory
  {
    Serial.println ("successfully updated battery variables values to EEPROM");

    writeMemory(memMinBatVlt,minBatVlt);
    writeMemory(memMaxBatVlt,maxBatVlt);
    return true;
  }else
  {
    Serial.println ("Unable to save battery variables values to EEPROM");
  }

 /* if (maxBatVlt >= 64)
 {
  maxBatVlt = 64;
 }

 if (minBatVlt >= 63)
 {
  minBatVlt = 63;
 }*/

    return false;
}







//==========
void getmemBatVlt () {
    Serial.println ("Received battery voltage range from memory");
  minBatVlt = readMemoryByte(memMinBatVlt);
  maxBatVlt = readMemoryByte(memMaxBatVlt);

  /*if (minBatVlt > 62 || maxBatVlt > 64)
  {
    minBatVlt = 42;
    maxBatVlt = 64;

    writeMemory(memMinBatVlt,minBatVlt);
    writeMemory(memMaxBatVlt,maxBatVlt);

    Serial.println ("Chaning bad boundaries having values" );
  }*/

}