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
	menu.IncludeFunction(&func1, var_one, "var_one", "psi", false);
	menu.IncludeFunction(&func2, var_two, "var_two", "psi");
	menu.IncludeFunction(&func3, var_three, "var_three", "l/min",true);
	menu.IncludeFunction(&func4, var_four, "var_four", "val");
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
	
	Serial.print("Complete Loading");
	delay(50);
	//print("Laser Security Ready.");
	//delay(2500);
	display.clear();
	display.setCursor(1,1);
}

// the loop function runs over and over again until power down or reset
byte count_buttonUp = 0;
byte count_buttonDown = 0;
byte count_buttonDownUp = 0;

byte state_btnPswrd = 0;
const  byte pswdLenght = 4;
const  byte		   array_pswd[pswdLenght] = {2, 2, 1, 2};
       byte array_inputedpswd[pswdLenght] = {0, 0, 0, 0} ; //  // buggy stuff, worked simple vars BELOW. Reason, cant add count_buttonDownUp, always give random result
		   
// store values from the user to compare with array_pswd
		   
	   byte arr0 = 0;  
	   byte arr1 = 0;
	   byte arr2 = 0;
	   byte arr3 = 0; 
	   
String p1;
String p2;

bool  bool_pswdPassed = false;	

bool prsUp;
bool prsDown;
void loop() {


	// read buttons values 
	buttonSET = !digitalRead(BUTTON_SET);
	btnUp.scaning();
	btnDown.scaning();

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
	}

	

	// return condition about opened menu
		if (! menu.InterfaceDinamic() ) 
{
	    

			
			if (  ( btnUp.onlyReadPressedSignleTime() || btnDown.onlyReadPressedSignleTime()) ) //  // if button up or down then activate timer to work and while pressing, reset with millis()
			{
				clock_btnPswTimeout = millis() + 5000UL; // reset each  10 seconds  time
				  //Serial.println( millis() < (long)clock_btnPswTimeout );
				
			
			

				if ( 	 millis() < (long)clock_btnPswTimeout ) // timer when to work
				
					{
						
						if (btnUp.onlyReadPressedSignleTime()) {
							//array_inputedpswd[count_buttonDownUp] = 2;
							set_inputedpswd (count_buttonDownUp, 2);
							Serial.println("[]btnUp:"+String(count_buttonDownUp));
							
						}
						
						if (btnDown.onlyReadPressedSignleTime()){
							//array_inputedpswd[count_buttonDownUp] = 1;
							set_inputedpswd (count_buttonDownUp, 1);
							Serial.println("[]btnDown:"+String(count_buttonDownUp));
							
						}
							
								
					
						

								
									for (byte i = 0; i < pswdLenght; i++) // prining out
									{
											 p1 +=String (array_pswd[i])+",";
									}
										display.print ("Paswd:" + p1 +"\n");
								
							
									for (byte i = 0; i < pswdLenght; i++)
									{
										p2 +=String (get_inputedpswd(i))+",";
									}
									display.print ("Input:" + p2 +"\n");	
								


							
							
									
							
									p1.clear();
									p2.clear();
									countInputs ();
			
					}
		
			} 
			
			
			if ( millis() > (long)clock_btnPswTimeout )
			{
				
				clearInputArray ();
				count_buttonDownUp = 0;
				display.clear();
			}
		
		
}



 
 
	manualReapetEach1sec = false;
	prsDown= false;
	prsUp= false;
	btnUp.endScaning();
	btnDown.endScaning();
}


void clearInputArray (){
	for (byte i = 0; i < pswdLenght; i++) //clear user input array
	{
		set_inputedpswd(i,0);
		Serial.print(String (i)+"]clear:"+String (array_inputedpswd[i]) );
	}
	
}

void countInputs () {
		//Serial.println("+++add:count_buttonDownUp" + String(count_buttonDownUp));
		count_buttonDownUp = count_buttonDownUp + byte(1); // count each pressed buttons; //bugg reacted
		
		if (count_buttonDownUp > count_buttonDownUp)  { // detect when user pushed time to high
			clearInputArray ();
			count_buttonDownUp = 0;
			Serial.print("===Clear:count_buttonDownUp");
		}
		
}

void set_inputedpswd (byte index, byte value) {
	
	
	switch (index) {
		
		case 0:
		arr0 = value;
		break;
		
		
		case 1:
		arr1=value;
		break;
		
		
		case 2:
		arr2=value;
		break;
		
		
		case 3:
		arr3=value;
		break;
		
	}	
}

byte get_inputedpswd (byte index) {
	
	
	switch (index) {
		
		case 0:
		return arr0 ;
		break;
		
		
		case 1:
		return arr1;
		break;
		
		
		case 2:
		return arr2;
		break;
		
		
		case 3:
		return arr3;
		break;
		
	}
}