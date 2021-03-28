
//	This program was made for ESP8266

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
 Name:		lib_meniuInterface128x64OLEDSSD1306AsciiWire.ino
 Created:	2/8/2020 8:43:53 PM
 Author:	zick
*/

#include "lib_meniuInterface128x64OLEDBased(SSD1306AsciiWire).h"
#include "buttons.h"

// buttons
 byte BUTTON_SET =  D0;
 byte BUTTON_DOWN = D3;
 byte BUTTON_UP = D4;
 byte OUTPUT_ALARMSOUND = D5;
 
//variables
byte var_one; 
byte var_two;  
byte var_three;
byte var_four; 
byte var_five; 
byte var_six;	

bool  bool_pswdPassed = false;	

byte alarm_totalTime = 60; // 1s*60 = 60s 
byte alarm_strenght;                   // pulse width modulated strength of alarm sound
byte alarm_strenghtWarning	= 50	;  // when password doesn't inputed correctly
byte alarm_strenghFullBlast	= 255	;  // when someone passed throw laser or password was incorrect to many times
byte alarm_delay = 10;				   //  delay to alarm to turn on with laser
byte alarm_timer; // 				   // this timer depends only from input password buttons and works as timeout to turn on alarm
byte alarm_timerLaser;				   // react only laser input and are to store time how much should be on a alarm
byte alarm_countLaser;				   // count laser interrupts
byte alarm_incorrectScoreChanged=0;

byte relay_lamp_totalTime = 80; // store value how much time should work a relay
byte relay_lamp_timer;         // timer for turn on relay for a lamps


//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long clock_btnPswTimeout = 0;  

// buttons variables
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

buttons btnUp   (buttonUP,BUTTON_UP , true);//(bool &BTN/*button variable that change at demand*/,byte &PinRead /*what kind of pin to read*/ , bool InvertDigitalRead = true/*Read input statement invertecly !*/,bool PressSingleTime = false /*Disable fast pulses and leave single press*/ ,unsigned long CLOCK_ADD = 0UL /*add time  away to slower the speed of pulses*/) {
buttons btnDown (buttonDOWN,BUTTON_DOWN , true);
lib_meniuInterface128x64OLEDSSD1306AsciiWire menu(buttonUP, buttonDOWN, buttonSET); // set a buttons to in.

#include "EEPROM32.h"
#include "functions.h"

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);
	//EEPROM.begin(512);
	menu.IncludeFunction(&func1, alarm_totalTime, "Alarm Total Time", "sec");
	menu.IncludeFunction(&func2, alarm_strenghtWarning, "Alarm  Warning", "pwm%" );
	menu.IncludeFunction(&func3, alarm_strenghFullBlast, "Alarm Full Blast", "pwm%");
	menu.IncludeFunction(&func4, alarm_delay, "Alarm Start Delay", "sec");
	menu.IncludeFunction(&func5, alarm_countLaser, "Alarm Laser Count", "count");
	menu.IncludeFunction(&func6, relay_lamp_totalTime, "Lamp Total Time", "sec");
	
	menu.IncludeQuckAccessFunction(&func5, var_five, "Quick Access", "psi", false);
	// a default function are saved in here.
	menu.IncludeFunctionSetDefault(&userSetDefault);
	
	menu.initiate();


	
	for  (int i = 0; i < 8; i++)
	{
		display.setCursor(i * 3 ,1);
		println (".");
		delay(100);
	}

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);
	
	pinMode(OUTPUT_ALARMSOUND,OUTPUT);
	
	Serial.print("Complete Loading");
	delay(50);
	//print("Laser Security Ready.");
	//delay(2500);
	display.clear();
	display.setCursor(1,1);
}




void loop() {


	// read buttons values 
	buttonSET = !digitalRead(BUTTON_SET);
	
	
	
	
	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed
	if (((long)clock_1min + 60000UL) < millis()) // 120000UL a double clock speed by
	{
		clock_1min = millis(); // reset each  60 seconds  time
		display.clear();
		manualReapetEach1sec = true; // reset for fast print
	}


	if (((long)clock_1sec + 1000UL) < millis())
	{
		clock_1sec = millis(); // reset each  60 seconds  time
		manualReapetEach1sec = true; // reset each second to print one time
		
		if (alarm_timer > 0)
			 {
				  analogWrite(OUTPUT_ALARMSOUND,alarm_strenght);
				  //Serial.println("A alrm:" + String (alarm_timer)+ " pswIncorrect: "+String(psw_incorrectScore));
				 
				 
				 display.setCursor(0,7);
				 display.print ("Alarm on:"+ String (alarm_timer)+ "  ");
					 alarm_timer--;
			 } else {
				  analogWrite(OUTPUT_ALARMSOUND,0);
				  //Serial.println("B");
				 
			 }
			
			 
			 
	}

	
	
	
	
	
	
	btnUp.scaning();
	btnDown.scaning();
	
	
	

	

	// return condition about opened menu
		if (! menu.InterfaceDinamic() ) 
	{
			
			
			collectPasswordRuntime ();
			
			
		




   
	}
	
	
	if (psw_incorrectScore > 19) // 5 incorrect times then alarm
		{	
			alarm_timer = alarm_totalTime;
			psw_incorrectScore = 0; // reset for non repeating all the time
			alarm_strenght = alarm_strenghFullBlast;
		}
	else if (alarm_timer ==0 && psw_incorrectScore > 0  && alarm_incorrectScoreChanged != psw_incorrectScore)	// 
			{
				alarm_incorrectScoreChanged = psw_incorrectScore;
				alarm_timer = 1; // short alarm signal to alarm bugler from touching this buttons
				alarm_strenght=alarm_strenghtWarning;
			}
		
	
	if (pswMatching) // if alarm is on, but later inputed correct to disable siren noise as soon as possible
	{
			alarm_timer = 0; 
	}
	
	
	
	
	
	
	manualReapetEach1sec = false;
	btnUp.endScaning();
	btnDown.endScaning();
	
}