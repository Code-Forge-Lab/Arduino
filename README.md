
## Arduino Clone [LGT8F328P](https://github.com/RalphBacon/LGT8F328P-Arduino-Clone-Chip-ATMega328P) as ATMega328P  [Youtube](https://www.youtube.com/watch?v=Myfeqrl3QP0&list=WL&index=101&t=0s)


# Arduino SPI, IC2, IO ##
## 'Processing 3' IDE For Arduino Graphic Sensors Processing
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

    
28BYJ-48
   if (Serial.available()) {
//      give signal when is active         
        digitalWrite(13, HIGH);


//     read the incoming byte:
//        incomingByte = Serial.read(); // bug with readBytes as missing first letter if bouth is reading 

//     read whole array of char           //char     , lenghthttps://stackoverflow.com/questions/7818811/import-error-no-module-named-numpy
      Serial.readBytes(incomingByte, 20);

//    // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte );
        
    
        
        delay(200);
        digitalWrite(13, LOW); 
    }


    // condition what to do if getting any instruction
    if ( strcmp (incomingByte, "alive")== 0  ) https://stackoverflow.com/questions/7818811/import-error-no-module-named-numpy
       Serial.print ("Command 'alive' accepted");
       
    if (strcmp (incomingByte,"responde") == 0 ) // compare char if math
       Serial.print ("Command 'responde' accepted");   
// turn on ledhttps://www.instructables.com/id/3020-CNC-Arduino-GRBL-CNC-Shield-V3/
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
# Example #
 > - Serial **I/O** communication using integer imput as value
 ```cpp
 
int externalPinReact = 8;

void setup() {

    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(externalPinReact, OUTPUT); // output to external led

}

 char incomingByte[20] ; // for incoming serial data

 int incomingInt;


int counter;


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

        incomingInt = atoi( incomingByte)  ; // here efectivly contvert char numbers to integer using atoi( ) 
//        incomingString = String(incomingByte);
    
        
        delay(100);
        digitalWrite(13, LOW); 
    }


   
 // turn off led


     if (incomingInt <= counter )
      {
        digitalWrite(externalPinReact,HIGH); 
      
       Serial.println ("blink at: "+String(counter) );
        
          counter = 0;
        }
     
//     clearChar (incomingByte,20); 
    
        //Serial.println (" input time: "+String ( (incominhttps://tecadmin.net/install-python-3-7-on-ubuntu-linuxmint/gInt) ) + ",counter: "+String(counter) + " ,incomingInt < counter:" + String( (incomingInt <= counter) ? "true":"false" ) ); // only can tolerate 3.4v max.
        //Serial.println ("-"); 

    
   // Capture serial data if even is sleeping
   delay(100);    


   
     

  counter++;
  if (counter > 100) counter = 0;
  digitalWrite (externalPinReact, LOW);
               
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
 >- More information about GRBL can be found at [here](https://arduinoboardproject.com/en/how-to-install-grbl-on-arduino-uno-with-the-arduino-ide-software/) and  working IDE [here](https://cnc.js.org/)
 >- Basic set up for begginer are [here](https://www.instructables.com/id/How-to-Make-GRBL-CNC-V3-Shield-Based-Mini-CNC-Mach-1/)
 >- IDE [java based](https://winder.github.io/ugs_website/download/), [here](https://github.com/grbl/grbl/wiki/Using-Grbl)
 >- [Xloader](https://github.com/xinabox/xLoader/releases/tag/v1.339) or [else](http://www.mikrodb.com/index.php/85-arduino/85-emdebe-hex-loader-load-hex-to-arduino-board) or upluad hex's with [arduino IDE](https://www.youtube.com/watch?v=dAwmZDIUeuU)
 >- Clear [EEPROM](https://www.youtube.com/watch?v=zlRCzGwHft0) Memory 

## GRBL frimware with Servo ##
 * Get grbl + servo frimware for [arduino](https://github.com/robottini/grbl-servo) controller.
 * Control function up and down M3 and M5
 * GRBL 0.9i with servo motor support. Use the PIN D11 to drive the servo. Use the commands M03 Sxxx (xxx between 0 and 255) to rotate the servo between 0-180. The command M05 turn the servo to zero degrees As 'STOP'+Z from CNC v3.0 Shield
 
 ## How look Arduino Uno GRBL shield
![](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/cnc_v3_arduino_uno.jpg)
  >- Stepper Motor Driver Module DRV8825 
  >- How ajust DRV8825 Voltage [reference](https://www.youtube.com/watch?v=YJSmrbeZUp4)
  ![DRV8825 Driver](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/DRV8825%20.png)  
  >- Pin out a GRBL with Arduino Uno or atmel328p chip.
![pin out](https://github.com/Code-Forge-Lab/Arduino/blob/master/images/CNC%20Shield%20V3.0%20pinout.jpg)

# Parameters of GRBL 0.8-0.9 version #
 Sum information are [here](http://forum.arduino.cc/index.php?topic=576107.0) and [here with math https://stackoverflow.com/questions/7818811/import-error-no-module-named-numpyas bonus](https://www.instructables.com/id/3020-CNC-Arduino-GRBL-CNC-Shield-V3/)
 
 
 G-Code Generation and [more](https://www.norwegiancreations.com/2015/08/an-intro-to-g-code-and-how-to-generate-it-using-inkscape/)
# G-Code IDE Inksckape  install linux mint or ubuntu
* sudo add-apt-repository ppa:inkscape.dev/stable
* sudo apt-get update
* sudo apt-get install inkscape



## How Generate G-Code With Inkscape From Photos
1. Select Image ''Important''
2. Go Extension > GCodeTools
3. Tools Library: and give cutting bit option
4. Orentaition Point: and give 2D or 3D usage. If Ask somthing went wrong then GO Path/Object To Path or Path/Bitmat < convert image to black and white
5. In Extension > GCodeTools, Path to GCode and set path and name in 'Preferences'
## How Inskape Work With Servo Motor
[Go in here for full tutorial](https://www.instructables.com/id/How-to-Make-GRBL-CNC-V3-Shield-Based-Mini-CNC-Mach-1/)

## PCB engraving tool [flatcam](http://flatcam.org/download) download _[8.5v](https://bitbucket.org/jpcgt/flatcam/downloads/)_ and [video](https://www.youtube.com/watch?v=ILnc-7I0zQA)
* Convert GRBL to G-Code [video](https://www.youtube.com/watch?v=IbsBCMlQB_c), [video2](https://www.youtube.com/watch?v=1N5ZjYQXTiw)
* Works on Linux
* Parameters tool dia:0.25mm, Cut Z:-0.01, Travel Z:3cm, Feed rate:120(Travel Speed), Spindle Speed(2000),
* Can convert GRBL from EasyEda
* I Use to get G-Code from PCB and then use G-Code Sender


## G-Code Sender IDE Universal-G-Code-Sender (Best Expirence)
* Instalation [page](https://winder.github.io/ugs_website/download/) a 2.0 Platform (Beta).
* ![Image](https://github.com/winder/Universal-G-Code-Sender/raw/master/pictures/2.0_platform_customizable_panels.png)
* Download for windows but can work on Java's [2.0 Platform(Beta)](http://bit.ly/2Z3DIVC)
* Executable in bin folder...



## G-Code Sender With AutoLevel
* Download can be found [here bCNC-0.9.14.30](https://github.com/vlachoudis/bCNC/releases) and python set up [here](https://tecadmin.net/install-python-3-7-on-ubuntu-linuxmint/) and manual install bCNC [here](https://github.com/vlachoudis/bCNC)
* Python Errors, [numpy](https://stackoverflow.com/questions/7818811/import-error-no-module-named-numpy),pyserial:pip install pyserial
* How to use it are this [site](http://www.electronoobs.com/eng_impresoras_eleksmill.php) and a [video](https://www.youtube.com/watch?v=YkMgsRTXpyg)


## Motor **28BYJ-48**
###### **Half-Step** working option 
* $0=10 (step pulse, usec)
* $1=25 (step idle delay, msec)
* $2=0 (step port invert mask:00000000)
* $3=0 (dir port invert mask:00000000)
* $4=0 (step enable invert, bool)
* $5=0 (limit pins invert, bool)
* $6=0 (probe pin invert, bool)
* $10=3 (status report mask:00000011)
* $11=0.010 (junction deviation, mm)
* $12=0.002 (arc tolerance, mm)
* $13=0 (report inches, bool)
* $20=0 (soft limits, bool)
* $21=0 (hard limits, bool)
* $22=0 (homing cycle, bool)
* $23=0 (homing dir invert mask:00000000)
* $24=50.000 (homing feed, mm/min)
* $25=500.000 (homing seek, mm/min)
* $26=250 (homing debounce, msec)
* $27=1.000 (homing pull-off, mm)
* $100=136.000 (x, step/mm) <-Important for Steppers
* $101=136.000 (y, step/mm) <-Important for Steppers
* $102=250.000 (z, step/mm) <-Important for Steppers
* $110=500.000 (x max rate, mm/min)
* $111=500.000 (y max rate, mm/min)
* $112=500.000 (z max rate, mm/min)
* $120=10.000 (x accel, mm/sec^2) <-Important for Steppers
* $121=10.000 (y accel, mm/sec^2) <-Important for Steppers
* $122=20.000 (z accel, mm/sec^2) <-Important for Steppers
* $130=200.000 (x max travel, mm)
* $131=200.000 (y max travel, mm)
* $132=200.000 (z max travel, mm)
