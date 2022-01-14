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
 Name:		lib_meniuInterface20x4_LiquidCrystal_I2C.ino
 Created:	2/8/2020 8:43:53 PM
 Author:	zick
*/

#include "lib_meniuInterface20x4_LiquidCrystal_I2C.h"
#include "buttons.h"

// buttons
 byte BUTTON_SET =  D0;
 byte BUTTON_DOWN = D3;
 byte BUTTON_UP = D4;

//variables
byte var_one; 
byte var_two;  
byte var_three;
byte var_four; 
byte var_five; 
byte var_six;	


//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long timer5Sec = 0;
// buttons variables
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

int inc = 0;

buttons btnUp   (buttonUP,BUTTON_UP , true , false , 100UL);//(bool &BTN/*button variable that change at demand*/,byte &PinRead /*what kind of pin to read*/ , bool InvertDigitalRead = true/*Read input statement invertecly !*/,bool PressSingleTime = false /*Disable fast pulses and leave single press*/ ,unsigned long CLOCK_ADD = 0UL /*add time  away to slower the speed of pulses*/) {
buttons btnDown (buttonDOWN,BUTTON_DOWN , true , false , 100UL);

lib_meniuInterface20x4_LiquidCrystal_I2C menu(buttonUP, buttonDOWN, buttonSET); // set a buttons to in.

#include "EEPROM32.h"
#include "functions.h"

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);
	//EEPROM.begin(512);
	menu.IncludeFunction(&func1, var_one, "var_one", "psi", false);
	menu.IncludeFunction(&func2, var_two, "var_two", "tankis");
	menu.IncludeFunction(&func3, var_three, "var_three", "L/min");
	menu.IncludeFunction(&func4, var_four, "var_five", "Lux");
	menu.IncludeFunction(&func4, var_four, "var_six", "Lux");
	menu.IncludeFunction(&func4, var_four, "var_seven", "Lux");
	menu.IncludeFunction(&func4, var_four, "var_eight", "Lux");
	menu.IncludeQuckAccessFunction(&func5, var_five, "Boiler", "C",false);
	// a default function are saved in here.
	menu.IncludeFunctionSetDefault(&userSetDefault);
	
	menu.initiate();


	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);
	
	Serial.print("Complete Loading");
	delay(50);
	print("Loading complete.",0,0);
	delay(2500);
	display.clear ();
}

// the loop function runs over and over again until power down or reset
byte count_buttonUp = 0;
byte count_buttonDown = 0;
void loop() {


	// read buttons values 
	buttonSET = !digitalRead(BUTTON_SET);
	//buttonDOWN = !digitalRead(BUTTON_DOWN);
	//buttonUP = !digitalRead(BUTTON_UP);
	btnUp.scaning();
	btnDown.scaning();

	 menu.updates ();

	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed 
	if (((long)clock_1min + 60000UL) < millis()) // 120000UL a double clock speed by
	{
		clock_1min = millis(); // reset each  60 seconds  time
		display.clear();
	}


	if (((long)clock_1sec + 500UL) < millis()) 
	{
		clock_1sec = millis(); // reset each  60 seconds  time
		
		

	}

	

	// return condition about opened menu
	isMenuActive = menu.InterfaceDinamic();// save condition about usage of menu.
		menu.displayButtonsValue();
	    

		if (btnUp.onlyReadPressedSignleTime())
			   count_buttonUp++;
		if (btnDown.onlyReadPressedSignleTime())
				count_buttonDown++;

		
	// everything is as open loop  method
	 // if (manualReapetEach1sec)
			//menu.displayStoredMemoryValues();
		
		
		menu.print__("Hello world" , 1,0);
		menu.print__("countsUps:" + String (count_buttonUp) + ",Dns:"+String (count_buttonDown),2,0);
		menu.print__("LOOT"+String (inc),3,0);
		
//		print("::" + String (menu.isclearedDisplayCommon));

		
		if (millis() > (long)(timer5Sec + 5000L) )
    {
        timer5Sec = millis();
        inc++;
    };




	btnUp.endScaning();
	btnDown.endScaning();
}
