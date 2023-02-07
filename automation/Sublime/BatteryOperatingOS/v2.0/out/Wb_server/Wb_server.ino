/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <NTPClient.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

// Replace with your network credentials
const char* ssid     = "a";
const char* password = "b";

// Set web server port number to 80
WiFiServer server(80);


const long utcOffsetInSeconds = 10800; // 10800
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeekLT[7][20] = {"Sekmadienis", "Pirmadienis", "Antradienis", "Treciadienis", "Ketvirtadienis", "Penktadienis", "Sestadienis"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);



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


String pin_Inv_OnState = "off";

// Assign output variables to GPIO pins
//const int output5 = 5;
const byte pin_Inv_On = D2; // on relay esp01 = 0, but on esp8266 = 2 
const byte Inv_Output220 = D1; //5
const byte pin_button = D3; 
const byte pin_ledIndicator = D4;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

bool previousInv_On__On  = true; // always true to begin with
bool previousState = false;
bool doBtnWasPress; //save condition about pressed button state
int  asBtnWasPress_Sec; // hold a count how much time button is pressed
bool doWebBtnWasPress = false;

int  CheckOneHourPassed ; // change state to auto-on in eacth hour by compared that is not the same
bool ActivateOneHourWhenPassed = false; // alow to pass a logick in each hour 

signed int Inv_OnOn_TotalTime = 23000 ; // how mucth time shoud work a Inv_On by seconds 60sec*45min = 2700
signed int Inv_OnOn_Sec = 0 ; // turn on Inv_On by a seconds
signed int powrelay_Sec = 0;
unsigned long clock_1sec = 0;

unsigned long clock_0_2sec = 0; // 0.2 seconds update
unsigned long clock_1secCounter = 0;// if x 5 then = 1 sec 

byte          clock_1minCount = 0;





void funTimeClient () {
 
  timeClient.update();

Serial.print(daysOfTheWeekLT[timeClient.getDay()]);
Serial.print(", ");
Serial.print(timeClient.getHours());
Serial.print(":");
Serial.print(timeClient.getMinutes());
Serial.print(":");
Serial.println(timeClient.getSeconds());


currentTimeHeader = String(daysOfTheWeekLT[timeClient.getDay()]) + "<br> " + timeClient.getFormattedTime() ; //String(daysOfTheWeekLT[timeClient.getDay()]) + ", " + String(timeClient.getHours()) + ": " + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());

Serial.print (currentTimeHeader);
 //delay (1000);
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

void turnOnHeatingTimeout () { Inv_OnOn_Sec = Inv_OnOn_TotalTime;powrelay_Sec = 10; pin_Inv_OnState = "on";}
void turnOnHeatingTimeoutOFF () { Inv_OnOn_Sec = 0;powrelay_Sec = 0; pin_Inv_OnState = "off"; doBtnWasPress = false; doWebBtnWasPress = false; }

void Inv_On_Auto_on () {

    if (ActivateOneHourWhenPassed ) {
      

          ActivateOneHourWhenPassed = false; // but if any 1/4 condition is correct, then leave this disabler to prevent from multiple  on in same hour 
         if (output6State == "on" && timeClient.getHours() == 8) {Serial.println(" at 8h");  turnOnHeatingTimeout (); }
    else if (output7State == "on" && timeClient.getHours() == 10) {Serial.println(" at 10h");  turnOnHeatingTimeout (); }
    else if (output8State == "on" && timeClient.getHours() == 16) {Serial.println(" at 16h");  turnOnHeatingTimeout (); }
    else if (output9State == "on" && timeClient.getHours() == 20) {Serial.println(" at 20h");  turnOnHeatingTimeout (); }
    else { ActivateOneHourWhenPassed = true; }; // if failed to auto-on then reset to work for a next time

           Serial.print ("Stage:ActivateOneHourWhenPassed auto-on ["+ String (ActivateOneHourWhenPassed));
           Serial.println ("]     State20h " + String (output9State == "on") + ", getHours[" +String (timeClient.getHours()) + "]==20h:" + String (timeClient.getHours() == 20));
    }

}



void one30SecTimer () 
{

if (clock_1minCount > 30 )
  {
    
    clock_1minCount = 0;
    funTimeClient ();

    if (CheckOneHourPassed != timeClient.getHours() ) // check if hour is changed
    {
      CheckOneHourPassed = timeClient.getHours();
      ActivateOneHourWhenPassed = true ;// reset
    }

    Inv_On_Auto_on ();

  }
     else
       clock_1minCount = clock_1minCount + 1;


}

void turnOnByButtonEvent () { // when push button  is pressed

   if (!digitalRead (pin_button))
 {
        if (asBtnWasPress_Sec < 100)
            asBtnWasPress_Sec += 1;

      

      if (asBtnWasPress_Sec == 20){ // after some time, button will be on
         doBtnWasPress = !doBtnWasPress;
          Serial.println("Button is set a program");

          if (doBtnWasPress){
             turnOnHeatingTimeout ();
          }
          else{
            turnOnHeatingTimeoutOFF();
          }

      }else if (asBtnWasPress_Sec < 20) {
            Serial.println("Button is pressing");
          if (asBtnWasPress_Sec % 2 == 1)
             digitalWrite (pin_ledIndicator , LOW);
         else
             digitalWrite (pin_ledIndicator , HIGH);

      }


       
    

 }else{
  asBtnWasPress_Sec = 0;

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

void oneSecTimer () {
  
  quarterSecondTimer () ;

  if (clock_1secCounter >=5)
  {
    clock_1secCounter = 0;


     // Serial.println ("1 Sec");
    

    if ( Inv_OnOn_Sec > 0 ) { Inv_OnOn_Sec = Inv_OnOn_Sec -1;} // countdown timeout
    if (powrelay_Sec > 0)  {powrelay_Sec = powrelay_Sec - 1;}  

       one30SecTimer ();
  }

}
  


// in closed wifi loop and wise wersa
void inFullLoop ()
{


}


void setup() {
 // Serial.begin(115200);
   
  // Initialize the output variables as outputs
 // pinMode(output5, OUTPUT);
  pinMode(pin_Inv_On, OUTPUT);
  pinMode (Inv_Output220, OUTPUT);
  pinMode (pin_button, INPUT);
  pinMode (pin_ledIndicator , OUTPUT);
  // Set outputs to LOW
  //digitalWrite(output5, LOW);
  // digitalWrite(pin_Inv_On, HIGH );

  EEPROM32_INIT();
Serial.println("Load settings from EEPROM");
if (readMemoryBool (6))  output6State = "on";
if (readMemoryBool (7))  output7State = "on";
if (readMemoryBool (8))  output8State = "on";
if (readMemoryBool (9))  output9State = "on";



  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  timeClient.setUpdateInterval(10000UL); // 10 seconds interval to update a time
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  funTimeClient ();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
    
  if (client) {                             // If a new client connects,

    Serial.println("New Client.");          // print a message out in the //serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the //serial monitor
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
                previousInv_On__On = true;

            if (header.indexOf("GET /4/on") >= 0) {                                       
              
              //Serial.println("GPIO 4 on");
              pin_Inv_OnState = "on";
               doBtnWasPress = true; // not wery nessesery but be in sinck with a button
               doWebBtnWasPress = true;
              
             // digitalWrite(pin_Inv_On, HIGH);

               if (previousInv_On__On)
                    turnOnHeatingTimeout();
                  digitalWrite (pin_ledIndicator , HIGH);
                  previousInv_On__On = false;
              
            } else if (header.indexOf("GET /4/off") >= 0) {
               Serial.println("GPIO 4 off");
              pin_Inv_OnState = "off";

                digitalWrite (pin_ledIndicator , LOW);
                previousInv_On__On = true;
                Inv_OnOn_Sec = 0;
                powrelay_Sec = 0;
                doBtnWasPress = false;
                doWebBtnWasPress = false;
            }
            




            if (header.indexOf("GET /6/on") >= 0) {
              
              Serial.println("GPIO 6 on");
              output6State = "on";
              writeMemory (6,true);

            } else if (header.indexOf("GET /6/off") >= 0) {
               Serial.println("GPIO 6 off");
              output6State = "off";
               writeMemory (6,false);
            }


            if (header.indexOf("GET /7/on") >= 0) {
              
                Serial.println("GPIO 7 on");
              output7State = "on";
              writeMemory (7,true);

              
            } else if (header.indexOf("GET /7/off") >= 0) {
               Serial.println("GPIO 7 off");
              output7State = "off";
              writeMemory (7,false);

                
               
            }


            
             if (header.indexOf("GET /8/on") >= 0) {
              
                Serial.println("GPIO 8 on");
              output8State = "on";
              writeMemory (8,true);

              
            } else if (header.indexOf("GET /8/off") >= 0) {
                Serial.println("GPIO 8 off");
              output8State = "off";
              writeMemory (8,false);

               
            }


            if (header.indexOf("GET /9/on") >= 0) {
              
                Serial.println("GPIO 9 on");
              output9State = "on";
              writeMemory (9,true);

              
            } else if (header.indexOf("GET /9/off") >= 0) {
                Serial.println("GPIO 9 off");
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
            client.println("<body> <h1>Inverter ["+String (powrelay_Sec)+"] System "+String(doWebBtnWasPress)+" </h1> ");


            if (Inv_OnOn_Sec > 0 ){
              
              String h = (getHours() > 0) ? String(getHours()) +"h " : " " ;
              String m = (getMinutes() > 0) ? String(getMinutes()) +"m " : " " ;
              String s = (getSeconds() > 0) ? String(getSeconds()) +"s " : " " ;

              client.println("<div class=\"container\"><a href=\"/reload\" class=\"reload\" style=\"text-align:left;\">Working [" + h + m + s + "] </a>");
              // digitalWrite(pin_Inv_On, LOW);
              // if (powrelay_Sec < 2)
              // digitalWrite (Inv_Output220, LOW);
              
            }
            else{
              client.println("<div class=\"container\"><a href=\"/reload\" class=\"reload\" style=\"text-align:left;\">    [Reload]   </a>");
              // digitalWrite(pin_Inv_On, HIGH);
              // digitalWrite (Inv_Output220, HIGH);


               
            }
          

          client.println(" <h4 style=\"text-align:right;\">" + currentTimeHeader + "</h4>");
           
          client.println("</div>");


            
            // Display current state, and ON/OFF buttons for GPIO 4  
             
            if (pin_Inv_OnState=="off") {
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
             

              client.println("<p>Auto-on:");
             if (output6State =="on")
               client.println("8h,");

             if (output7State =="on")
               client.println("10h,");

             if (output8State =="on")
               client.println("16h,");
                
             if (output9State =="on")
               client.println("20h");

             client.println("</p>");

             if (doBtnWasPress && !doWebBtnWasPress) {client.println ("Buvo ijungta migtuku");}
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
      Serial.println("Client disconnected.");
      Serial.println("");
  }
   oneSecTimer (); // update rutine by eacht seconds
   inFullLoop ();
   



     if (Inv_OnOn_Sec > 0 || doBtnWasPress){ // if timeout still on then turn on 
              
             

               if (previousState && Inv_OnOn_Sec <= 1)
                {
                    Serial.print("RESET timer:pin_Inv_OnState from :"+pin_Inv_OnState + " " + String (previousState));
                  pin_Inv_OnState = "off"; // back to off state
                  
                    Serial.println(" new :" +pin_Inv_OnState);

                   previousState = false;
                }

              digitalWrite(pin_Inv_On, HIGH);
            if (asBtnWasPress_Sec  == 0)   digitalWrite (pin_ledIndicator , HIGH);

              if (powrelay_Sec  == 0)
              digitalWrite (Inv_Output220, HIGH);  
            }
            else{
              
               previousState = true;
              digitalWrite(pin_Inv_On, LOW);
              digitalWrite (Inv_Output220, LOW);
             if (asBtnWasPress_Sec  == 0)  digitalWrite(pin_ledIndicator , LOW);
              
            }
   


}







void EEPROM32_INIT() {
  
  EEPROM.begin(12);
  
};


// write to memory ones 
bool writeMemory(int16_t address, bool value) {

  if (EEPROM.read(address) != value) {
    EEPROM.write(address, value);
        Serial.println("writeMemory as bool adr["+String(address) + "] Wrote:" + String (value));
    //delay(200);

    if (EEPROM.commit()) {
        Serial.println("writeMemory:EEPROM successfully committed");
      return true;
      } else {
        Serial.println("writeMemory:ERROR! EEPROM commit failed");
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



int getHours()  {
  return ((Inv_OnOn_Sec  % 86400L) / 3600);
}
int getMinutes()  {
  return ((Inv_OnOn_Sec % 3600) / 60);
}
int getSeconds()  {
  return (Inv_OnOn_Sec % 60);
}