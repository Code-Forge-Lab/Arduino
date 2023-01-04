/*
 ESP8266 Serial Communication
 
 The blue LED on the ESP-12 module is connected to GPIO2
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
*/

void setup() {
  Serial.begin(9600);     // Initialize the Serial interface with baud rate of 9600
  inputString.reserve(200);
  delay (100);
  Serial.println ("Initiated!");
}

// the loop function runs over and over again forever
String serialRead;
void loop() {  
  if(Serial.available()>1)    //Checks is there any data in buffer 
  {
    Serial.print("We got"+String (Serial.available())+":");
    Serial.println(char(Serial.read()));  //Read serial data byte and send back to serial monitor

    serialRead+= char(Serial.read());
  }
  else 
    {
    if (serialRead.length() > 1)
   {
        Serial.println ("We got in full text: " + serialRead );
        serialRead = "";
        delay (400);
   }    
  }
}
