

## Arduino SPI, IC2, IO ##

![arduino-spec-IO](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/Arduino-Uno-Pin-Diagram.png)
## ##
![arduino-spec-IO](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/ATmega328P-PU-PIN-Diagram-connection-configration.jpg)


## Bootloader and as SPI ##
![connection-image](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/2018-06-19%2020_50_22-Burning%20bootloader%20on%20standalone%20Atmega328(internal%20clock%208MHz)%20-%20Page%202.png)

 > * More Help can be found  [here](https://www.youtube.com/watch?v=j65N0kN1_3w)
 - Bootloader [information](https://www.baldengineer.com/arduino-bootloader.html).
![minimu component requared](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/Arduino-on-a-breadboard.jpg)
# #
![arduino-spec-IO](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/arduino-1284-unopro-pinout.jpg)
![atmel328p](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/4mY0y.png)
![minimu component requared](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/s-l300.jpg)
![atmel328p_2](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/main-qimg-8175532811e0a6fa855e137be63d7ef4.png)

# Atmega128A #
 Source [link](https://forum.arduino.cc/index.php?topic=410576.0).
![atmega128](https://camo.githubusercontent.com/1364255dc8fb3fec6f91046a123a410e4233854e/687474703a2f2f692e696d6775722e636f6d2f64456e6a3838422e6a7067)
# Interput #
 - Arduino Uno, Mega
 - ESP8260
![interupt pins](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/2018-06-22%2011_46_41-(74)%20Arduino%20Interrupts%20Tutorial%20-%20YouTube.png)
> Example
```cpp

```
https://github.com/Code-Forge-Lab/Arduino/blob/master/FTDI-328p.jpg
![ROTARY ENCODER](https://github.com/Code-Forge-Lab/Arduino/blob/master/FTDI-328p.jpg)
# Common library #
 * > [ROTARY ENCODER](https://github.com/0xPIT/encoder/tree/arduino)
 * >  Adafruit_ILI9341
 * > Adafruit_GFX_Library
 * > Adafruit_Circuit_Playground
 * > Adafruit_SSD1306
 * > Adafruit-GFX
 * > i2c_2device
 * > I2CScanner_hex
 * > U8glib
 
 
# Chips #
- Atmega8515l-8pu = Program Memory Size (KB)8
- ATMEGA1284P-PU 128kb Flash memory , 16kb SRAM

# Example #
 > - Serial **I/O** communication

```cpp
// the setup function runs once when you press reset or power the board
void setup() {

    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(11, OUTPUT); // output to external led

}

 char incomingByte[20] ; // for incoming serial data



// the loop function runs over and over again forever
void loop() {

    

   if (Serial.available()) {
//      give signal when is active         
        digitalWrite(13, HIGH);


//     read the incoming byte:
//        incomingByte = Serial.read(); // bug with readBytes as missing first letter if bouth is reading 

//     read whole array of char           //char     , lenght
      Serial.readBytes(incomingByte, 20);

//    // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte );
        
    
        
        delay(200);
        digitalWrite(13, LOW); 
    }


    // condition what to do if getting any instruction
    if ( strcmp (incomingByte, "alive")== 0  ) 
       Serial.print ("Command 'alive' accepted");
       
    if (strcmp (incomingByte,"responde") == 0 ) // compare char if math
       Serial.print ("Command 'responde' accepted");   
// turn on led
    if (strcmp (incomingByte,"on") == 0 ) 
       {
          digitalWrite(11, 1023); // output 5v max.
        }

 // turn off led
    if (strcmp (incomingByte,"off") == 0 ) 
       {
          digitalWrite(11, LOW);
       }    
     
     clearChar (incomingByte,20); 
    
   
    
   // Capture serial data if even is sleeping
   delay(1000);    


   Serial.println (" input "+String (analogRead (PB1 )) + " "); // only can tolerate 3.4v max.
   Serial.println ("-"); 
     
                 // wait for a second
}

void clearChar ( char *characters , int size ){

         
        for ( int x = 0; x <  size;  x++){
//             Serial.print (", "+String (x) + " -= " + String(characters [x]));
             // remove each character to nill
             characters[x]  = '\0';
          }
  }
```
# GRBL Arduino #
 >- More information about GRBL can be found at [here](https://arduinoboardproject.com/en/how-to-install-grbl-on-arduino-uno-with-the-arduino-ide-software/) and [here](https://cnc.js.org/)
 >- IDE [java based](https://winder.github.io/ugs_website/download/), [here](https://github.com/grbl/grbl/wiki/Using-Grbl)
 >- [Xloader](https://github.com/xinabox/xLoader/releases/tag/v1.339) or [else](http://www.mikrodb.com/index.php/85-arduino/85-emdebe-hex-loader-load-hex-to-arduino-board) or upluad hex's with [arduino IDE](https://www.youtube.com/watch?v=dAwmZDIUeuU)
 >- Clear [EEPROM](https://www.youtube.com/watch?v=zlRCzGwHft0) Memory 
  >- How look Arduino Uno GRBL shield
![](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/cnc_v3_arduino_uno.jpg)
  >- Stepper Motor Driver Module DRV8825   
  ![DRV8825 Driver](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/DRV8825%20.png)  
  >- Pin out a GRBL with Arduino Uno or atmel328p chip.
![pin out](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/CNC%20Shield%20V3.0%20pinout.jpg)
