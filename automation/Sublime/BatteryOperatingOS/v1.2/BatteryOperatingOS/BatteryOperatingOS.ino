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
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);

*/



/*
 Name:    lib_meniuInterface20x4_LiquidCrystal_I2C.ino
 Created: 2/8/2020 8:43:53 PM
 Author:  zick
*/

#include <NTPClient.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

// Replace with your network credentials
const char* ssid  = "MM";
const char* password = "GG";
signed int relayOn_TotalTime = 86460 ; // how mucth time shoud work a relay by seconds 60sec*45min = 2700
String headerName = "Battery Operating"; // give a name to a webserver


// Set web server port number to 80
WiFiServer server(80);



//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeekLT[7][20] = {"Sekmadienis", "Pirmadienis", "Antradienis", "Treciadienis", "Ketvirtadienis", "Penktadienis", "Sestadienis"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 7200; // 10800 // time zone offset
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);




String currentTimeHeader;
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output6State = "off";
String output7State = "off";
String output8State = "off";
String output9State = "off";


String collectionOfStateAsTrigggerTimes = "";


String pin_relayState = "off";

// Assign output variables to GPIO pins
//const int output5 = 5;
const byte pin_relayPower = D8; // on relay esp01 = 0, but on esp8266 = 2 
const byte pin_relay_inverter = D7;// turn on inverter
const byte pin_ReadInverterFault = D6 ; // read inverter where are in fault condition to react as swith off power relay fast as possible
const byte pin_button = 2; // GPIO2

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

bool previousRelayOn  = true; // always true to begin with
bool previousState = false;

int  CheckOneHourPassed ; // change state to auto-on in eacth hour by compared that is not the same
bool ActivateOneHourWhenPassed = false; // alow to pass a logick in each hour 


 signed int relayPowerOn_Sec = 0 ; // turn on relay by a seconds
 signed int relayInverterOn_Sec = 0; // seconds to turn on a inverter
 signed int relayInverterOn_Total = 16; // seconds when power relay to be on

bool       inverterFailed = false; // save condition about inverter process is working propietly
String      reportWhatTimeInverterFailed = ""; 

// represent for a user what time and day was turned on 
String     reportWhatTimeWasOn = ""; 
signed int reportWhatTimeOnResetCounter = 0; // when this variable reached zero, clear above string to emtpy value ""
signed int reportHowMuchTimeWasOn = 0;

unsigned long clock_1sec = 0;

unsigned long clock_0_2sec = 0; // 0.2 seconds update
unsigned long clock_1secCounter = 0;// if x 5 then = 1 sec 


byte          clock_1minCount = 0;





void funTimeClient () {
 
  timeClient.update();
/*
//Serial.print(daysOfTheWeekLT[timeClient.getDay()]);
//Serial.print(", ");
//Serial.print(timeClient.getHours());
//Serial.print(":");
//Serial.print(timeClient.getMinutes());
//Serial.print(":");
//Serial.println(timeClient.getSeconds());
*/

currentTimeHeader = String(daysOfTheWeekLT[timeClient.getDay()]) + "<br> " + timeClient.getFormattedTime() ; //String(daysOfTheWeekLT[timeClient.getDay()]) + ", " + String(timeClient.getHours()) + ": " + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());

//Serial.print (currentTimeHeader);
 //delay (1000);
}

void funTimeReportUser(String turnedByEvent = "") 
{

 // timeClient.update();
  reportWhatTimeOnResetCounter = 40200;// 86400s its aday,  43200 its a 12h
  reportWhatTimeWasOn =   turnedByEvent+", " + timeClient.getHours() +"h "+timeClient.getMinutes()+"min";
  reportHowMuchTimeWasOn = 0; // reset 
}


/*
String transConditionalPrint (String state ,String indexHref, String trueStatePrint , String falsestatePrint) {
              if (state == "off")
                  return ("<p><a href=\"/"+indexHref+"/on\"><button class=\"button\">"+falsestatePrint+"</button></a></p>");
              else
                  return ("<p><a href=\"/"+indexHref+"/off\"><button class=\"button\">"+trueStatePrint+"</button></a></p>");
           }

           //  client.println(" <button class=\"button_footer\"  style=\"width:25%\">20h</button>");
//
*/

 String transConditionalPrintBtn_group (String state ,String indexHref, String trueStatePrint , String falsestatePrint ,String widthProcentage = "25%") {
              if (state == "off")
                  return (" <a href=\"/"+indexHref+"/on\"><button class=\"button_footer\"  style=\"width:"+widthProcentage+"\">"+trueStatePrint+"</button></a>");
              else
                   return (" <a href=\"/"+indexHref+"/off\"><button class=\"button_footer2\"  style=\"width:"+widthProcentage+"\">"+falsestatePrint+"</button></a>");
           }          

void turnOnRelaysTimeout (String turnedByEvent = "") { relayPowerOn_Sec = relayOn_TotalTime; pin_relayState = "on"; funTimeReportUser(turnedByEvent);/* inverterFailed = false;/* reset invertert failed condtition if possible*/ relayInverterOn_Sec = relayInverterOn_Total;}

void Relay_Auto_on (String turnedByEvent = "") {

    if (ActivateOneHourWhenPassed ) {
      
      

          ActivateOneHourWhenPassed = false; // but if any 1/4 condition is correct, then leave this disabler to prevent from multiple  on in same hour 
         if (output6State == "on" && timeClient.getHours() == 8) {Serial.println(" at 8h");    turnOnRelaysTimeout (turnedByEvent); }
    else if (output7State == "on" && timeClient.getHours() == 10) {Serial.println(" at 10h");  turnOnRelaysTimeout (turnedByEvent); }
    else if (output8State == "on" && timeClient.getHours() == 16) {Serial.println(" at 16h");  turnOnRelaysTimeout (turnedByEvent); }
    else if (output9State == "on" && timeClient.getHours() == 20) {Serial.println(" at 20h");  turnOnRelaysTimeout (turnedByEvent); }
    else { ActivateOneHourWhenPassed = true; }; // if failed to auto-on then reset to work for a next time

        

          //Serial.print ("Stage:ActivateOneHourWhenPassed auto-on ["+ String (ActivateOneHourWhenPassed));
         // Serial.println ("]     State20h " + String (output9State == "on") + ", getHours[" +String (timeClient.getHours()) + "]==20h:" + String (timeClient.getHours() == 20));
    }

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void one30SecTimer () 
{

if (clock_1minCount > 30 )
  {
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    clock_1minCount = 0;
    funTimeClient ();

    if (CheckOneHourPassed != timeClient.getHours() ) // check if hour is changed
    {
      CheckOneHourPassed = timeClient.getHours();
      ActivateOneHourWhenPassed = true ;// reset
    }

    Relay_Auto_on ("automatiskai");

  }
     else
       clock_1minCount = clock_1minCount + 1;


}

void turnOnByButtonEvent () { // when push button  is pressed

   if (!digitalRead (pin_button))
 {
      //Serial.println("Button is pressed");
      turnOnRelaysTimeout ("migtuku");

 }

}


void quarterSecondTimer () { //0.2 second

  if (((long)clock_0_2sec + 200UL) < millis())

  {
      turnOnByButtonEvent ();
      clock_0_2sec= millis();
      clock_1secCounter  = clock_1secCounter + 1;

  }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
void oneSecTimer () {
  
  quarterSecondTimer () ;

  if (clock_1secCounter >=5)
  {
    clock_1secCounter = 0;


     // Serial.println ("1 Sec");
    

    if (reportWhatTimeOnResetCounter > 0) {

        if (reportWhatTimeOnResetCounter == 1)
           {
            reportWhatTimeWasOn = ""; // in every 12 hours passed whidout any activation button or internet or auto-on, reset representive string to not show for a less confution in the future
            inverterFailed = false;/* reset invertert failed condtition if possible*/
           }

        reportWhatTimeOnResetCounter= reportWhatTimeOnResetCounter - 1;

    }

    if ( relayPowerOn_Sec > 0 ) {relayPowerOn_Sec = relayPowerOn_Sec -1; reportHowMuchTimeWasOn = reportHowMuchTimeWasOn + 1;} // countdown timeout
       {
         one30SecTimer ();
       }


      if (relayInverterOn_Sec > 0)
        {
         relayInverterOn_Sec-- ;
        }

      //  Serial.println ("relayInverterOn_Sec: "+ String (relayInverterOn_Sec) + " relayPowerOn_Sec: "+ String (relayPowerOn_Sec)  + " pin_ReadInverterFault: "+ String (digitalRead(pin_ReadInverterFault)) + " inverterFailed:" + String (inverterFailed) +
      //   " relayInverterOn_Sec:"+ String (relayInverterOn_Sec) + " << " + String (int(  relayInverterOn_Total / 2 ))   );
  }

}
  


// in closed wifi loop and wise wersa
void inFullLoop ()
{


}








//==============================================

byte connectionTimeout = 0; // if connection failed multiple times
String serialRead; // store a text from user where was inputed from a serial pipe line 
byte serialTimeout = 0;// for convience to user while tiping in sierial bus
char* ssid_;
char* password_;

char buffSSID[20];
char buffPSWD[20];

void setup() {
 Serial.begin(115200);
   Serial.println ("Starting ...SSD" + String (ssid) +" PWSD: "+ String (password));
  // Initialize the output variables as outputs
 // pinMode(output5, OUTPUT);
  pinMode (pin_relayPower, OUTPUT);
  pinMode (pin_relay_inverter, OUTPUT);
  pinMode (pin_button, INPUT_PULLUP);
  pinMode (pin_ReadInverterFault , INPUT_PULLUP);
  // Set outputs to LOW
  //digitalWrite(output5, LOW);
  digitalWrite(pin_relayPower, LOW );
  digitalWrite(pin_relay_inverter, LOW );

  EEPROM32_INIT();
//Serial.println("Load settings from EEPROM");
if (readMemoryBool (6))  output6State = "on";
if (readMemoryBool (7))  output7State = "on";
if (readMemoryBool (8))  output8State = "on";
if (readMemoryBool (9))  output9State = "on";


  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  Serial.print ("For more information type 'help'");
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   
     String serialGetSpecial; // special simbol begins from ':'
   
    if (serialTimeout > 0)
          serialTimeout--;
    else
        {
             Serial.print("_ ");

        }    
   
    connectionTimeout++;// to avoid printing all the time '_ ' character
    

    serialRead = serialEvent();// works perfectly with delay() function

    if (serialRead.length() > 0 ){// read comed arround
        serialRead = serialRead.substring (0,serialRead.length()-1); //to  remove incoming '\n'
       
        Serial.println (" ");// make space in new line after '_ _ _ _ _'
        serialTimeout = 200;

         if (serialRead.lastIndexOf(":") > 0) {
          serialGetSpecial = serialRead.substring (serialRead.lastIndexOf(":") + 1,serialRead.length()); //separate text after:
          serialRead = serialRead.substring (0,serialRead.indexOf(":"));// separate text before :
          Serial.println ("serialRead:"  + serialRead) ;
          Serial.println ("found ':' " + serialGetSpecial);
         }

        if (serialRead == "help")
           Serial.println ("Awailable commands to input a credential for login in local network.\nssid:wifiname\npswd:password\nstatus\nupdate");

         else if (serialRead == "pswd" ){
          // serialGetSpecial.toCharArray (password_ , serialGetSpecial.length() + 1 );
            

            
            serialGetSpecial.toCharArray(buffPSWD, 20);//convert from string to char
            password = buffPSWD;

          // password = "YEPTHATTaASTEISPURPLE";
           Serial.println ("Registered wifi password:" + serialGetSpecial  + ", wrote:" +  String (password));
         }

         else if (serialRead == "ssid"){

          // serialGetSpecial.toCharArray (ssid_ , serialGetSpecial.length() + 1 );
           // ssid = serialGetSpecial.c_str()
           serialGetSpecial.toCharArray(buffSSID, 20);//convert from string to char
           ssid = buffSSID;

           Serial.println ("Registered ssid as wifi name:" + serialGetSpecial + ", wrote:" +  String (ssid));
            
         }else if (serialRead == "status"){
          Serial.println ("===========================================");
          Serial.print ("ssid:"+ String (ssid) + "\npassword:" + String(password));
         
         }else if (serialRead == "update"){
            Serial.println ("ssd and password was updated!");
            WiFi.begin(ssid, password);
          }

         else {
          Serial.println ("For more information type 'help'");
         }

       // Serial.println ("Received " +String (serialRead.length()) + ":" + String (serialRead) + String (serialRead == "help") + " == help");
    }

   /* if (connectionTimeout > 55){
      Serial.print ("Failed to connect to a network");
      break;
    }*/
   
  }


  timeClient.begin();
  timeClient.setUpdateInterval(10000UL); // 10 seconds interval to update a time
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.  " + *ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  funTimeClient ();
}









//----------------------------------------------


void loop(){


  WiFiClient client = server.available();   // Listen for incoming clients
    
  if (client) {                             // If a new client connects,

    //Serial.println("New Client.");          // print a message out in the //serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the //serial monitor
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
            oneSecTimer (); // update rutine by eacht seconds

            /*
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
            //  //Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
            //  //Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if
            */




            if (header.indexOf("GET /reload") >= 0)
                previousRelayOn = true;

            if (header.indexOf("GET /4/on") >= 0) {                                       
              
              //Serial.println("GPIO 4 on");
              pin_relayState = "on";
              relayInverterOn_Sec = relayInverterOn_Total;
             // digitalWrite(pin_relayPower, HIGH);

              inverterFailed = false; // reset status about inverter when user pressed a to turn on

               if (previousRelayOn)
                    turnOnRelaysTimeout("internetu");
                  previousRelayOn = false;
              
            } else if (header.indexOf("GET /4/off") >= 0) {
             //Serial.println("GPIO 4 off");
              pin_relayState = "off";

                previousRelayOn = true;
                relayPowerOn_Sec = 0;
            }
            




            if (header.indexOf("GET /6/on") >= 0) {
              
            //Serial.println("GPIO 6 on");
              output6State = "on";
              writeMemory (6,true);

            } else if (header.indexOf("GET /6/off") >= 0) {
             //Serial.println("GPIO 6 off");
              output6State = "off";
               writeMemory (6,false);
            }


            if (header.indexOf("GET /7/on") >= 0) {
              
              //Serial.println("GPIO 7 on");
              output7State = "on";
              writeMemory (7,true);

              
            } else if (header.indexOf("GET /7/off") >= 0) {
             //Serial.println("GPIO 7 off");
              output7State = "off";
              writeMemory (7,false);

                
               
            }


            
             if (header.indexOf("GET /8/on") >= 0) {
              
              //Serial.println("GPIO 8 on");
              output8State = "on";
              writeMemory (8,true);

              
            } else if (header.indexOf("GET /8/off") >= 0) {
              //Serial.println("GPIO 8 off");
              output8State = "off";
              writeMemory (8,false);

               
            }


            if (header.indexOf("GET /9/on") >= 0) {
              
              //Serial.println("GPIO 9 on");
              output9State = "on";
              writeMemory (9,true);

              
            } else if (header.indexOf("GET /9/off") >= 0) {
              //Serial.println("GPIO 9 off");
              output9State = "off";
              writeMemory (9,false);

            }


           







            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;background-color:#E9E0EC;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77AE73;}");

            client.println(".container {width: auto;height: 70px}");
            client.println(".reload { background-color: #B68181; float: left;  border: none; color: white; padding: 16px 40px;text-decoration: none;  margin: 2px; cursor: pointer;}");
            client.println(".reload:hover {background-color: #B34949;transition-duration: 0.5s;}");
            client.println(".left-btn { float: left; }");
            client.println(".right-btn { float: right; }");



            client.println("h1,h2 {color: #191725;}");

            /* Add a background color on hover */ 
            client.println(" button:hover {background-color: #77878A;transition-duration: 1.2s;}");
            client.println(" .btn-group button:hover {background-color: #77878A;}"); // 3e8e41 = SHARP green, 77AE73 = blure green,  195B6A = blue , 77878A = brown
            
            client.println(".button_footer{background-color: #195B6A; height:50px; border: 1px solid #BFC4B9; /* Green border */color: white; /* White text */padding: 10px 24px; /* Some padding */cursor: pointer; /* Pointer/hand icon */float: left; /* Float the buttons side by side */}");
            client.println(".button_footer2{background-color: #77AE73; height:50px; border: 1px solid #BFC4B9; /* Green border */color: white; /* White text */padding: 10px 24px; /* Some padding */cursor: pointer; /* Pointer/hand icon */float: left; /* Float the buttons side by side */}");
             
            /* Clear floats (clearfix hack) */
            client.println(".btn-group:after { content: ""; clear: both;  display: table;}");

            client.println(".btn-group button:not(:last-child) { border-right: none; /* Prevent double borders */}");

            //footer table menu 
            client.println("#footer {position:absolute;bottom:0; width:100%;   /* Height of the footer */background:#A9A9A9;}");
//            client.println("");
            
            client.println("</style></head>");


            
            // Web Page Heading
            client.println("<body> <h1> " + headerName + " </h1> ");


            if (relayPowerOn_Sec > 0 ){
              
              String d = (getDays(relayPowerOn_Sec) != 0) ? String(getDays(relayPowerOn_Sec)) +"d " : " " ;
              String h = (getHours(relayPowerOn_Sec) > 0) ? String(getHours(relayPowerOn_Sec)) +"h " : " " ;
              String m = (getMinutes(relayPowerOn_Sec) > 0) ? String(getMinutes(relayPowerOn_Sec)) +"m " : " " ;
              String s = (getSeconds(relayPowerOn_Sec) > 0) ? String(getSeconds(relayPowerOn_Sec)) +"s " : " " ;

              client.println("<div class=\"container\"><a href=\"/reload\" class=\"reload\" style=\"text-align:left;\">Working ["+ d + h + m + s + "] </a>");
             
            //     digitalWrite(pin_relayPower, HIGH);
            //    digitalWrite(pin_relay_inverter, HIGH );

              
            }
            else{
              client.println("<div class=\"container\"><a href=\"/reload\" class=\"reload\" style=\"text-align:left;\">    [Reload]   </a>");
              digitalWrite(pin_relayPower, LOW);
              digitalWrite(pin_relay_inverter, LOW );
              
               
            }
          

          client.println(" <h4 style=\"text-align:right;\">" + currentTimeHeader + "</h4>");
           
          client.println("</div>");


            
            // Display current state, and ON/OFF buttons for GPIO 4  
             
            if (pin_relayState=="off") {
              client.println("<a href=\"/4/on\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/4/off\"><button class=\"button button2\">OFF</button></a>");
            }


             
             
            


             client.println("<div class=\"btn-group\" id = \"footer\">");

            // client.println(" <button class=\"button_footer\"  style=\"width:25%\">8h</button>");
             //client.println(" <button class=\"button_footer\"  style=\"width:25%\">12h</button>");
            // client.println(" <button class=\"button_footer\"  style=\"width:25%\">16h</button>");
           //  client.println(" <button class=\"button_footer\"  style=\"width:25%\">20h</button>");
             client.println(transConditionalPrintBtn_group(output6State ,"6", "8h  on" , "8h off"));
             client.println(transConditionalPrintBtn_group(output7State ,"7", "10h on" , "10h off"));
             client.println(transConditionalPrintBtn_group(output8State ,"8", "16h on" , "16h off"));
             client.println(transConditionalPrintBtn_group(output9State ,"9", "20h on" , "20h off"));
             

              client.println("<p style=\"margin: 0px;\">Auto-on:");
             if (output6State =="on")
               client.println("8h,");

             if (output7State =="on")
               client.println("10h,");

             if (output8State =="on")
               client.println("16h,");
                
             if (output9State =="on")
               client.println("20h");

             if (reportWhatTimeWasOn.length() > 0)
                client.println ("<br>Buvo ijungta:" + reportWhatTimeWasOn);

              if (inverterFailed)
                 client.println ("<br>Inverteris sutriko: " + reportWhatTimeInverterFailed+ " !");

             client.println("</p>");



            client.println ("</div>");



             



            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            inFullLoop ();
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
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
   oneSecTimer (); // update rutine by eacht seconds
   inFullLoop ();
   


       if (!digitalRead(pin_ReadInverterFault) && (relayInverterOn_Sec  < int( relayInverterOn_Total / 2) )/*ignore inverter while booting*/  ) // if inverter failed at some point, switch down a power relay to avoid blackouts
             {
                 relayPowerOn_Sec = 0; // reset
                 inverterFailed = true; // save a failed condtition
                 pin_relayState = "off"; // back to off state
                 reportWhatTimeInverterFailed =  " " + timeClient.getFormattedTime(); // save time when was failed inverter to boot or was to low at a power 

             }



     if (relayPowerOn_Sec > 0  && !inverterFailed){ // if timeout still on then turn on 
              
             

               if (previousState && relayPowerOn_Sec== 1)
                {
                  //Serial.print("RESET timer:pin_relayState from :"+pin_relayState + " " + String (previousState));
                  pin_relayState = "off"; // back to off state
                  
                  //Serial.println(" new :" +pin_relayState);

                   previousState = false;
                }

             


               if (relayInverterOn_Sec  > 0) // firts turn on a inverter
                  {
                      digitalWrite (pin_relay_inverter , HIGH); 
                      reportWhatTimeInverterFailed = ""; //reset when user pressed to turn on button
                  }
              else if (relayInverterOn_Sec == 0)  // then turn on a power relay and also inverter 
                  {
                      digitalWrite (pin_relay_inverter , HIGH);
                      digitalWrite(pin_relayPower, HIGH);  
                  }


            }
            else{
              
                     digitalWrite(pin_relayPower, LOW);
                     digitalWrite (pin_relay_inverter , LOW);

               previousState = true;
              
            }
   



           
}







void EEPROM32_INIT() {
  
  EEPROM.begin(512);
  
};


// write to memory ones 
bool writeMemory(int16_t address, bool value) {

  if (EEPROM.read(address) != value) {
    EEPROM.write(address, value);
      //Serial.println("writeMemory as bool adr["+String(address) + "] Wrote:" + String (value));
    //delay(200);

    if (EEPROM.commit()) {
      //Serial.println("writeMemory:EEPROM successfully committed");
      return true;
      } else {
      //Serial.println("writeMemory:ERROR! EEPROM commit failed");
    }
  }

  return false;
}


// write to memory ones 
bool readMemoryBool(int16_t address) {

  Serial.println("Read bool:" + String (EEPROM.read(address)) );    
  return EEPROM.read(address);
};



// write to memory ones 
byte readMemoryByte(int16_t address) {
       Serial.println("Read byte:" + String (EEPROM.read(address)) );
  return EEPROM.read(address);
};


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
String serialEvent() 
{  
  String inputString = "";// each time create new set of string to store each available chare for later to return in function
   byte chCnt = 0;
   
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    //chCnt++;
       //Serial.print ( String (chCnt) + ":" +inputString + " " );
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
   
      //if (inChar == '\n') {
      //stringComplete = true;
      //Serial.print (" char:" + String (chCnt) );
      //}

    delay (10);// criticly inportant, requared to sink in with arriving input data
  }
  return inputString;
}


int getDays(int d )  {

  return (( d % 2592000L) / 86400);
}

int getHours(int h )  {
  return (( h % 86400L) / 3600);
}
int getMinutes(int m)  {
  return ((m % 3600) / 60);
}
int getSeconds(int s)  {
 //Serial.println ("Day..........................................................................................: " + String ( (( s % 2592000L) / 86400)) );
  return (s % 60);
}
