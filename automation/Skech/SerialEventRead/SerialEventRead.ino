/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/


         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial.print("Hey there , for more infromation type 'help'");
  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);
}
 String serialRead;
void loop() {

  
    String serialGetSpecial; // special simbol begins from ':'
    serialRead = serialEvent();// works perfectly with delay() function

    if (serialRead.length() > 0 ){// read comed arround
        serialRead = serialRead.substring (0,serialRead.length()-1); //to  remove incoming '\n'
       
        Serial.println (" ");// make space in new line after '_ _ _ _ _'
       

         if (serialRead.lastIndexOf(":") > 0) {
          serialGetSpecial = serialRead.substring (serialRead.lastIndexOf(":") + 1,serialRead.length()); //separate text after:
          serialRead = serialRead.substring (0,serialRead.indexOf(":"));
          Serial.println ("found ':' " + serialGetSpecial);
          Serial.println ("serialRead:"  + serialRead) ;
         }

        if (serialRead == "help")
           Serial.println ("Awailable commands to input a credential for login in local network.\nssd:wifiname\npswd:password");

         else if (serialRead == "pswd" ){
           Serial.println ("Registered wifi password:" + serialGetSpecial);
         }

         else if (serialRead == "ssd"){

           Serial.println ("Registered ssd as wifi name:" + serialGetSpecial );
         }

         else {
          Serial.println ("For more information type 'help'");
         }

       // Serial.println ("Received " +String (serialRead.length()) + ":" + String (serialRead) + String (serialRead == "help") + " == help");
    }

//Serial.println ("ping");
  //delay ( 2000);
}

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
