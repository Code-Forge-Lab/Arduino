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




byte LED1 = 2;
byte LED2 = 16;
unsigned long currentTime = millis();
String tString;

// the setup function runs once when you press reset mor power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D8 ,HIGH);
  digitalWrite(D7 ,HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D4, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(D3, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D0, HIGH);

  Serial.println ( "HIGH: 1 sec");


  delay(4000);


  Serial.println ("LOW: 7 sec \n");

  digitalWrite(D8 , LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D4, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(D3, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D0, LOW);
  delay(5000);                       // wait for a second
  currentTime = millis();

}
