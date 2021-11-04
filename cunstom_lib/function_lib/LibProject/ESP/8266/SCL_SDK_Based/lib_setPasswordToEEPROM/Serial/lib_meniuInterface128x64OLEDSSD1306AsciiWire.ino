/*
 Name:		lib_meniuInterface128x64OLEDSSD1306AsciiWire.ino
 Created:	2/8/2020 8:43:53 PM
 Author:	zick
*/

#include "lib_meniuInterface128x64OLEDBased(SSD1306AsciiWire).h"

// buttons
#define BUTTON_DOWN 5
#define BUTTON_SET 4
#define BUTTON_UP 6

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

// buttons variables
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

lib_meniuInterface128x64OLEDSSD1306AsciiWire menu(buttonUP, buttonDOWN, buttonSET); // set a buttons to in.

#include "EEPROM32.h"
#include "functions.h"

// the setup function runs once when you press reset or power the board
void setup() {
	menu.IncludeFunction(&func1, var_one, "var_one", "psi", false);
	menu.IncludeFunction(&func2, var_two, "var_two", "psi");
	menu.IncludeFunction(&func3, var_three, "var_three", "l/min",true);
	menu.IncludeFunction(&func4, var_four, "var_four", "val");

	menu.IncludeQuckAccessFunction(&func5, var_five, "Quic Access", "psi", false);
	// a default function are saved in here.
	menu.IncludeFunctionSetDefault(&userSetDefault);

	menu.initiate();

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {


	// read buttons values 
	buttonSET = !digitalRead(BUTTON_SET);
	buttonDOWN = !digitalRead(BUTTON_DOWN);
	buttonUP = !digitalRead(BUTTON_UP);


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
	isMenuActive = menu.InterfaceDinamic();// save condition about usage of menu.
		menu.displayButtonsValue();
		
	// everything is as open loop  method
	 // if (manualReapetEach1sec)
			//menu.displayStoredMemoryValues();
		
		printeach_1sec(String (clock_1sec)); //allow to print each second
		printeach_1sec("");
		printeach_1sec("");
		printeach_1sec("");
		
//		print("::" + String (menu.isclearedDisplayCommon));

		


	manualReapetEach1sec = false;
}
