
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
//                                       //INPUT VCC-->[5.1k]-|-[btn]--GND                     INPUT VCC-->[btn]-|-[5.1k]--GND
//INPUT or buttons                               PULLUP                                                PULLDOWN

static const uint8_t DTX  = 1;  //                   {!1}                                                 {!1}
static const uint8_t DRX  = 3;   //                  {!1}                                                 {!1}
static const uint8_t D0   = 16;  //               Yes                                                      Yes  but can't be use as interupts
static const uint8_t D1   = 5;   //               Yes                                                      Yes
static const uint8_t D2   = 4;   //               Yes                                                      Yes
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








void setup() {
  // put your setup code here, to run once:


pinMode (D0, INPUT);
pinMode (D1, INPUT);
pinMode (D2, INPUT);

pinMode (D3, INPUT);
pinMode (D4, INPUT);
pinMode (D5, INPUT);

pinMode (D6, INPUT);
pinMode (D7, INPUT);
pinMode (D8, INPUT);

/*
pinMode (D9, INPUT );
pinMode (D10, INPUT);
//pinMode (D14, INPUT);
*/

//Disable Serial.print , and become regular GPIO INPUT/OUTPUT
pinMode (DTX, FUNCTION_3);
//pinMode (DRX, FUNCTION_3);

pinMode (DTX, INPUT);
//pinMode (DRX, INPUT);
//-----------------------


Serial.begin (115200);


}


bool lock = false;
unsigned long timer1Sec;

bool readDTX;
bool readDRX;

bool readD0;
bool readD1;
bool readD2;

bool readD3;
bool readD4;
bool readD5;

bool readD6;
bool readD7;
bool readD8;

bool readD9;
bool readD10;
bool readD11;

bool readD12;
bool readD13;
bool readD14;

void loop() {
 
// on/off build in led  
  /*
digitalWrite (LEDpin, true);
delay (1000);
digitalWrite(LEDpin, false);
delay (1000);
*/

readD0 =  digitalRead (D0);
readD1=   digitalRead (D1);
readD2 =  digitalRead (D2);

readD3 =  digitalRead (D3);
readD4=  digitalRead  (D4);
readD5 =  digitalRead (D5);

readD6 =  digitalRead (D6);
readD7 =  digitalRead (D7);
readD8 =  digitalRead (D8);
/*
readD9 =  digitalRead (D9);
readD10 =  digitalRead (D10);
//readD11 =  digitalRead (D11);
*/
readDTX =  !digitalRead (readDTX);
//readDRX =  !digitalRead (readDRX);


/*
if (millis() > (long)(timer1Sec + 2000L) )
    {
        timer1Sec = millis();
        Serial.println ("D3 " + String (readD3));
        Serial.println ("D4 " + String (readD4));
        Serial.println ("D5 " + String (readD5));
        Serial.println ("-----------------------------" );
    };
   
  if (readD3  || readD4 ||  readD5)
 {
    digitalWrite(LED_BUILTIN , LOW);
    Serial.println ("LED!!");
    delay (500);
  } else
  {
    digitalWrite(LED_BUILTIN , HIGH);
    delay (100);
  } 
  
*/







  
  if (millis() > (long)(timer1Sec + 2000L) )
    {
        timer1Sec = millis();
        Serial.println ("D0 " + String (readD0));
        Serial.println ("D1 " + String (readD1));
        Serial.println ("D2 " + String (readD2));
        Serial.println ("----------]" );
        Serial.println ("D3 " + String (readD3));
        Serial.println ("D4 " + String (readD4));
        Serial.println ("D5 " + String (readD5));
        Serial.println ("----------]" );
        Serial.println ("D6 " + String (readD6));
        Serial.println ("D7 " + String (readD7));
        Serial.println ("D8 " + String (readD8));
        Serial.println ("----------]" );
        Serial.println ("DTX " + String (readDTX));
        Serial.println ("DRX " + String (readDRX));
        Serial.println ("----------]" );
       
       /*
        Serial.println ("D9_S3 " + String (readD9));
        Serial.println ("D10_S2 " + String (readD10));
      //  Serial.println ("D11_S1 " + String (readD11));
      //  Serial.println ("-----------------------------" );
      */

      
    };

    
    if (  readDTX   )
 {
    digitalWrite(LED_BUILTIN , LOW);
    //Serial.println ("LED!!");
    delay (500);
  } else
  {
    digitalWrite(LED_BUILTIN , HIGH);
    delay (100);
  } 


   


}
