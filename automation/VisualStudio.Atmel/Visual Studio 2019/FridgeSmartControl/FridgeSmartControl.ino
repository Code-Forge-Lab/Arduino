

//  Programed for LGT8F328P Arduino Pro Mini Clone.
// 'LGT8F328P-LQFP32 MiniEVB'
// At 79% project progress , start do 'crashing-buggy' stuff... 23.428 bytes
#pragma once

/*
 Name:		FridgeSmartControl.ino
 Created:	9/5/2019 11:42:28 AM
 Author:	zick
*/
//#include <avr/power.h> // library not working propietly with oled display '<Adafruit_SSD1306.h>' if placed before initiliaziniot in setup() clock_prescale_set(clock_div_1); loop
#include <Arduino.h>
//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ThermoSencor.h"
#include <EEPROM.h>


// Variables

bool boolSetButton = false;
bool boolSetMenu = false;
bool boolQuicklyChange = false;
byte meniuOptionSelected = 1;
byte meniuOptionsLenght = 7; // how much meniu option in meniu
bool meniuOptionIsPressing = false;// detect when button no longer is press
bool meniuOptionIsSelected = false; // When eventualy are at P0-P3 option then time to select what specificly are changing a values that are included in the statement.
unsigned long startedWaiting;
unsigned long startedWaitingmeniuOptionSelected;
//unsigned long startedWaitingOwerworkTimeout; // starting point for overwork protection
//bool boolStartedWaitingOwerworkTimeout = true; // trigger ones in owvrwork mode cycle to set caunter 'startedWaitingOwerworkTimeout' starting point 
bool boolUserOffsetTemperatureRange = false;  // return true when completed temperature range escape
bool boolUserP3_Timeout;
unsigned long clock_1min = 0;
/// button events variable
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

// user saved condition in long term memory 'EEPROM'
byte   userP0; // for 0 = C-H = 1 Cooling Heating  +
byte   userP1; // for temperature  logic range offset '0.5' range +
byte   userP2; // working temperature gap time
byte   userP3; // over working timeout when working too long
byte   userP4; // over working cooldown when working too long
byte   userP5; //1byte, save temperature that must be reached

// user values that use to multyply or divide from 255(1byte) to get propper rezult

byte   userP3_TimeoutC = 0; //counter over working timeout when working too long
byte   userP4_CooldownC = 0; //counter  over working cooldown when working too long

//byte userFixP2 = 60;  // multyplies to get 255(1 byte) to 255 minutes
//byte userFixP3 = 60;  // multyplies to get 255(1 byte) to 255 minutes
//byte userFixP4 = 60;  // multyplies to get 255(1 byte) to 255 minutes


// Defines
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// buttons
#define BUTTON_DOWN 10
#define BUTTON_SET 12
#define BUTTON_UP 11
// sensors
#define SENSOR_TEMP0 A1
#define SENSOR_TEMP1BACKUP A4
// relays
#define RELAY_FRIDGE0 13
#define RELAY_FRIDGE1BACKUPRead A4

// user saved condition in long term memory 'EEPROM'
// byte size is 1, int size is 2 byte's, float size is 4 byte's
#define MEMORY_FIRSTBOOT 0  // to set default values for a new arduino unit
#define MEMORY_P0 1  //1byte, for false = C-H = true Cooling Heating
#define MEMORY_P1 2  //1byte, for temperature  logic range offset '0.5' range
#define MEMORY_P2 3  //1byte, working timeout
#define MEMORY_P3 4  //1byte,  over working timeout when working too long
#define MEMORY_P4 5  //1byte,  over working cooldown when working too long
#define MEMORY_P5 6 //1byte, save temperature that must be reached

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     1 // 4 or Reset pin # (or -1 if sharing Arduino reset pin) '1' one is with LGT8F328P32'pins' But //#include <avr/power.h> library not working before SSD1306_SWITCHCAPVCC placing
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ThermoSencor sensorTemp(SENSOR_TEMP0, 9370);

// include function after display was declarated 
//#include "functions.h"


// SPECIAL BUGGY FUNCTION! WANA BE ON TOP
// user when changin something in meniu options, doest trow here 
void userChangeMeniuValue(byte* userPx, byte __delay = 10, byte minValue = 1, byte maxValue = 255) {

	// incrament
	if (buttonUP && *userPx < maxValue)
	{
		*userPx = *userPx + 1;

		delay(__delay);
		//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
	}

	// substract 
	if (buttonDOWN && *userPx > minValue)
	{
		*userPx = *userPx - 1;
		delay(__delay);
		//startedWaiting = millis();  //reset meniu delay counter // LAG When  push SET Button
	}

}


void setup() {



	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
	  //Serial.println(F("SSD1306 allocation failed"));
		for (;;); // Don't proceed, loop forever
	}
	delay(100);
	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display.display();

	// Clear the buffer
	display.clearDisplay();

	// gives first reading of temperature and Fix jump of values by 10 points
	for (int_fast8_t i = 0; i < 40; i++)
	{
		sensorTemp.InCustomTimeAverageUpdate(10);
		delay(100);
	}
	// display temperatue 
	txtBigNumber(sensorTemp.temperature, "C");
	//display.display(); 
	//delay(2000);

	//display.setTextColor(WHITE);        // Draw white text // not printing if aren't use

	// Vertical 64 small lines

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);

	// include temp. sensor
	pinMode(SENSOR_TEMP0, INPUT);

	// include control fridge relay
	pinMode(RELAY_FRIDGE0, OUTPUT);
	pinMode(RELAY_FRIDGE1BACKUPRead, INPUT);

	// This runs MCU at full speed at 32Mhz for LGT8F328P  as ATMEGA328P clone.
		//clock_prescale_set(clock_div_1);

	// set default values to a program to a frist time
	// memory is empty and arduino is booting first time
	if (EEPROM.read(MEMORY_FIRSTBOOT) == 255)
	{
		bool owerwrite = false;
		userSetValuesToMemory();
		userSetDefault();
		writeMemory(MEMORY_FIRSTBOOT, 0);

		display.clearDisplay();
		display.setCursor(0, 0);
		display.setTextSize(1);
		display.println("MEMORY_FIRSTBOOT");
		/*
		display.println("----------------");
		display.println("C-H:[" + String(userP0) + "]"); // for false = C-H = true Cooling Heating
		display.println("offset temp: [" + String(userP1) + "]"); // for temperature  logic range offset '0.5' range
		display.println("gap time: [" + String(userP2) + "]"); // working gap time
		display.println("timeout: [" + String(userP3) + "]"); // over working timeout when working too long
		display.println("cooldown: [" + String(userP4) + "]"); // over working cooldown when working too long
		display.println("temperature: [" + String(userP5) + "]"); // over working cooldown when working too long
		*/
		display.display();
		delay(25000); //25sec
	}
	else // Always give saved values back to program
	{
		userGetValues();
	}
}






byte toggleDisplay = 0;
bool boolOverworkState;

void loop() {
	// clear display
	


	buttonUP = digitalRead(BUTTON_UP);
	buttonSET = digitalRead(BUTTON_SET);
	buttonDOWN = digitalRead(BUTTON_DOWN);
	bool temp = analogRead(SENSOR_TEMP0);

	

	//read in long term temperature
	sensorTemp.InCustomTimeAverageUpdate(50); // 
	
	if (sensorTemp.temperature > -65)
		txtBigNumber(sensorTemp.temperature, "C");
	
	// set display default position
	display.setCursor(0, 0);
	display.setTextSize(1);
	

	// detect when no termo sensor connected
	

	//display.println(":" + String(boolQuicklyChange));
	//display.println("boolSetMenu:" + String(boolSetMenu));


	if (buttonSET || boolSetButton) {
		boolSetButton = true;

		// Set menu for more options
		/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
		if (buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
			// set loop from boolean


			// exit from tight loops!
			// event trigger if after some time will react to button set
			// reset whole set loop
			if ((millis() > (long)(startedWaiting + 5000UL + 950UL)) && buttonSET) {

				// After meniu option was selected 
				if (meniuOptionIsSelected) {
					userSetValuesToMemory();
				}
				//  if meniu option selected and need change a values to EEPROM
				meniuOptionIsSelected = !meniuOptionIsSelected;

				// Exit from progra  and main boolSetButton loop...if meniu not selected
				if (meniuOptionSelectFun() == meniuOptionsLenght) { // Exit

					boolSetButton = false; // exit from boolSetButton statement
					boolSetMenu = false; // exit from boolSetMenu statement
					meniuOptionSelected = 1; //reset meniu position to P0
					meniuOptionIsSelected = false; // when user exit from meniu set to default 
				}
				userSetValuesToMemory();
				delay(111);
			}
			else
			{
				// set whole set loop by default if no buttons event accures
				boolSetMenu = true;
				boolSetButton = true;

			}

			//progra-------------------------------------------------------
			if ((buttonSET || boolSetMenu)) {

				display.clearDisplay();

				display.setCursor(0, 0);
				// if meniu option not selected print about meniu abbreviation
				if (!meniuOptionIsSelected) {
					display.setTextSize(1);
					display.setCursor(40, 1);
					display.println("MENIU");
					display.setTextSize(2);
					display.setCursor(35, 22);
				}

				  if (meniuOptionSelectFun() < meniuOptionsLenght) // only display meniu option values exept on 'EXIT'
						display.println("[P"+String(meniuOptionSelectFun()-1)+"]");
					
				if (meniuOptionSelectFun() == 1) { // P0 C-H
					//display.println("[P0]");
					meniuDescribeOptionDisplay("<Mode:Cool,Heat>");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						userChangeMeniuValue(&userP0, 1, 0);

						if (userP0 == 0) {
							// -C-
							txtBigNumber("C", "ooling");
							boolUserOffsetTemperatureRange = false; // reset after chaning performance mode to work first time
						}
						else {
							// -H-
							txtBigNumber("H", "eating");
							boolUserOffsetTemperatureRange = false; // reset after chaning performance mode to work first time
						}

					}

				}
				else if (meniuOptionSelectFun() == 2) { // P1 Temp Logic offset 0.5
					//display.println("[P1]");
					meniuDescribeOptionDisplay("<Logic Temp. Offset>");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						userChangeMeniuValue(&userP1);
						// Display
						txtBigNumber(userP2FixLogicRangeOffse(), "C");
					}

				}
				else if (meniuOptionSelectFun() == 3) { // P2 Timer to ignore after working some time
					//display.println("[P2]");
					meniuDescribeOptionDisplay("-");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP2);
						// Display
						//txtBigNumber(userP2, "min");
					}

				}

				else if (meniuOptionSelectFun() == 4) { // P2 Timer to ignore after working some time
					//display.println("[P3]");
					meniuDescribeOptionDisplay("<Overworking timeout>");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						userChangeMeniuValue(&userP3);
						// Display
						txtBigNumber(userP3, "min");
					}

				}

				else if (meniuOptionSelectFun() == 5) { // P2 Timer to ignore after working some time
					//display.println("[P4]");
					meniuDescribeOptionDisplay("<Overworking cooldown");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						userChangeMeniuValue(&userP4);
						// Display
						txtBigNumber(userP4, "min");
					}

				}

				else if (meniuOptionSelectFun() == 6) { // Reset to default
					//display.println("[R5]");
					meniuDescribeOptionDisplay("<Reset to default>");
					// do EEPROM changes
					if (meniuOptionIsSelected) {
						display.println("Press up to reset");

						if (buttonUP)
						{
							display.setTextSize(4);
							display.println(" Done");
							display.display();
							// reset to default
							userSetDefault();
							userSetValuesToMemory();


							delay(2000);
							meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
						}
					}
				}


				else if (meniuOptionSelectFun() == meniuOptionsLenght) { // Exit
					display.println("[EXIT]");

				}

				display.display();

			}


		}





		// Quickly change temperature
		else if (buttonSET == LOW || boolQuicklyChange) {
			// set loop from boolean



			// exit from tight loops!
			// event trigger if after some time will react to button set
			if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 230UL)) && buttonSET || /*or timeout for 45s*/(millis() > (long)(startedWaiting + 45100UL))) {
				// reset whole set loop
				boolQuicklyChange = false;
				boolSetButton = false;
				userSetValuesToMemory(); // write temperature changes to memory
				delay(100);
			}
			else
			{
				// set whole set loop
				boolQuicklyChange = true;
				boolSetButton = true;

			}

			//progra-------------------------------------------------------
			if (boolQuicklyChange) {
				//display.setTextColor(BLACK, WHITE); // Draw 'inverse' text

				display.clearDisplay();

				
				
					display.println("Keisti temperatura");

					// Change values interface
					userChangeMeniuValue(&userP5,100);

					

					if (toggleDisplay > 20) // work as text blinker)
					{
						txtBigNumber(" ", " ");
						toggleDisplay = 0; //reset display flashing counter
					}
					else
					// display temperature 
					txtBigNumber(userP5FixTemperature(), "C");

					toggleDisplay++;
				
			
				
				//delay(4000);


			}
		}

	}
	else
	{
		startedWaiting = millis(); //by default will register values until condition
		boolQuicklyChange = false; //activate condition ones until event is trigered
		boolSetMenu = false; // set default to meniu option
		boolSetMenu = false; //activate condition ones until event is trigered
	}

	/*
	if (buttonUp)
		digitalWrite(RELAY_FRIDGE0, 255);
	else if (buttonSet)
		digitalWrite(RELAY_FRIDGE0, 0);

*/


// display all stuff was updated

	display.display();
	display.clearDisplay();
	display.setTextSize(1);
	display.setCursor(0, 0);
	display.setTextColor(WHITE); // Draw 'inverse' text

	//if (sensorTemp.temperature < -65)
	//	return; //break execution with relay
	

	// Do program brain here------------------------------------------------------------------------------------







	// if temperature sensor is connected do program
	if (sensorTemp.temperature > -65)
	{ 


		overworkTimer(boolOverworkState);
		
		if (boolUserP3_Timeout) {
			display.setCursor(0, 9);
			display.println("CooldownC:" + String(userP4_CooldownC) + "/" + String (userP4) );
		}
		else
		{
			display.setCursor(0, 9);
			display.println("TimeoutC:" + String(userP3_TimeoutC) + "/" + String (userP3) );
		}
		display.setCursor(0, 0);

			// for 0 = C-H = 1 Cooling Heating 6.59
			if (userP0 == 0) 
			{


					display.print("Cool");
				if (!boolUserOffsetTemperatureRange)
					display.print("ing:" + String(userP5FixTemperature()));
				else
					display.print("-stop:" + String(userP5FixTemperature() + userP2FixLogicRangeOffse()));
					display.println("c:"+ String(boolUserP3_Timeout));
					
					//display.println("TimeoutC:" + String(userP3_TimeoutC));
					//display.println("CooldownC:" + String(userP4_CooldownC));

					
				
				// offset mode
				// when temperatue are rising after feezer was on then wait more temperature offset to confirm that is not a fluctuation
				//sensor_25 > (24 + 2.0) = 25 > 27 
				if (sensorTemp.temperature > (userP5FixTemperature() + userP2FixLogicRangeOffse())) {
					boolUserOffsetTemperatureRange = false; 
					 
				}


				// if temperate is  less then should be,      //25 > (24 - 2.0)/ 25 > 22                         
				 
				if ( (sensorTemp.temperature > userP5FixTemperature() && !boolUserOffsetTemperatureRange) )
				{

						// initiation completed, not count down working time until reached critical point
						boolOverworkState = false; // keep counting working time 

					if (!boolUserP3_Timeout) // if reached overwork state then turn off relay
						digitalWrite(RELAY_FRIDGE0, 255);
					else 
						digitalWrite(RELAY_FRIDGE0, LOW);

				}
				else
				{

					boolUserOffsetTemperatureRange = true; // set to offset mode
					boolOverworkState = true; // disable counting wornking time and reset
					digitalWrite(RELAY_FRIDGE0, LOW);
					
				}

	

			}  // Heateing
			else
			{


				display.print("Heat");
				if (!boolUserOffsetTemperatureRange)
					display.print("ing:" + String(userP5FixTemperature()));  
				else
					display.print("-stop:" + String(userP5FixTemperature() - userP2FixLogicRangeOffse()));
					display.println("c:" + String(boolUserP3_Timeout));

					//display.println("TimeoutC:" + String(userP3_TimeoutC));
					//display.println("CooldownC:" + String(userP4_CooldownC));

				// offset mode
				// when temperatue are falling after heating was on then wait more temperature offset to confirm that is not a fluctuating
				//sensor_25 > (24 - 2.0) = 25 < 24 
				if (sensorTemp.temperature < (userP5FixTemperature() - userP2FixLogicRangeOffse())) {
					boolUserOffsetTemperatureRange = false;
				}


				if (sensorTemp.temperature < userP5FixTemperature() && !boolUserOffsetTemperatureRange)
				{

					// initiation completed, not count down working time until reached critical point
					boolOverworkState = false; // keep counting working time 

					if (!boolUserP3_Timeout) // if reached overwork state then turn off relay
						digitalWrite(RELAY_FRIDGE0, 255);
					else
						digitalWrite(RELAY_FRIDGE0, LOW);

				}
				else
				{

					boolUserOffsetTemperatureRange = true; // set to offset mode
					boolOverworkState = true; // disable counting wornking time and reset
					digitalWrite(RELAY_FRIDGE0, LOW);

				}




			}
	}
	else { // if no sensor connected, print error message
		//display.clearDisplay();
		//display.setTextSize(2);

		display.println("Nera");
		display.println("Temperat.");
		display.println("Sensor.");
		display.println("?:" + String(sensorTemp.temperature));

	}


}









 




/*
// Print big numbers in screen // compiler do not construct correctly giving wrong function
void txtBigNumber(int value, String valueType) {
	display.setCursor(0, 29);
	// set text size auto
	(String(value).length() <= 5 ? display.setTextSize(4) : display.setTextSize(3));

	display.setTextColor(WHITE);
	display.print(subLargeText(String(value)));
	display.setTextSize(2);
	display.println(valueType);
};
*/

//Print big numbers in screen // compiler do not construct correctly giving wrong function
void txtBigNumber(float value, String valueType) {
	display.setCursor(0, 29);
	// set text size auto
	(String(value).length() <= 5 ? display.setTextSize(4) : display.setTextSize(3));

	//display.setTextColor(WHITE);
	display.print(subLargeText(String(value)));
	(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	display.println(valueType /*+ String(value).length()*/);
};
//Print big numbers in screen // compiler do not construct correctly giving wrong function
void txtBigNumber(byte value, String valueType) {
	display.setCursor(0, 29);
	// set text size auto
	(String(value).length() <= 5 ? display.setTextSize(4) : display.setTextSize(3));

	//display.setTextColor(WHITE);
	display.print(value);
	(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	display.println(valueType);
};

void txtBigNumber(String value, String valueType) {
	display.setCursor(0, 29);
	display.setTextSize(4);

	//display.setTextColor(WHITE);
	display.print(value);
	(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	display.println(valueType);
};

String subLargeText(String value) {
	
	return (value.length() > 6) ? (value.substring(0, value.length() - 2)) : (value.substring(0, value.length() - 1));
};




byte meniuOptionSelectFun() {


	// if pressing a button and meniu where are selected are not to big then...
	if ((buttonUP && meniuOptionSelected < meniuOptionsLenght) && meniuOptionIsPressing && !meniuOptionIsSelected) {

		++meniuOptionSelected;
		startedWaitingmeniuOptionSelected = millis();// time out start point


	}
	// if pressing a button and meniu where are selected are not to small then...
	if ((buttonDOWN && meniuOptionSelected > 1) && meniuOptionIsPressing && !meniuOptionIsSelected) {

		--meniuOptionSelected;
		startedWaitingmeniuOptionSelected = millis();// time out start point

	}

	// Give new timeout and starting point for menu fuctions to work
	if (buttonSET && !meniuOptionIsSelected) {
		//meniuOptionIsPressing = true;
		startedWaitingmeniuOptionSelected = millis();// time out start point
	}

	// if user do not do for a while , accures a time out.
	if ((millis() > (long)(startedWaitingmeniuOptionSelected + 25000UL)) && !meniuOptionIsSelected)
	{
		boolSetButton = false; // exit from boolSetButton statement
		boolSetMenu = false; // exit from boolSetMenu statement
		meniuOptionSelected = 1; //reset meniu position to P0
	}

	// capture when button is pressing and set a one time button push
	if (buttonUP || buttonDOWN) {
		meniuOptionIsPressing = false; //While pressing ignore incrementing a meniuOptionSelected
	}
	else
	{
		meniuOptionIsPressing = true; //but when no longer is pressing set to default 
	}

	return meniuOptionSelected;
}


void meniuDescribeOptionDisplay(String txt) {
	display.setCursor(0, 9);
	display.setTextSize(1);
	display.println(txt);
}

// set user defaults all values
void userSetDefault() {
	userP0 = 1; // for 0 = C-H = 1 Cooling Heating
	userP1 = 5; // for temperature  logic range offset 5/10 = '0.5' range
	userP2 = 20; // working temperature gap time
	userP3 = 120; // over working timeout when working too long
	userP4 = 25;  // over working cooldown when working too long
	userP5 = 20;  // save temperature that must be reached

	userSetValuesToMemory();
}

// set user all values to memory
void userSetValuesToMemory() {

	byte c__ = 0;
	c__ += writeMemory(MEMORY_P0, userP0); // for C-H Cooling Heating
	c__ += writeMemory(MEMORY_P1, userP1); // for temperature  logic range offset '0.5' range
	c__ += writeMemory(MEMORY_P2, userP2); // working temperature gap time
	c__ += writeMemory(MEMORY_P3, userP3); // over working timeout when working too long
	c__ += writeMemory(MEMORY_P4, userP4); // over working cooldown when working too long
	c__ += writeMemory(MEMORY_P5, userP5); // temperature
	
	// debug if in memory is writen ones
	/*
	display.clearDisplay();
	display.setCursor(0,0);
	display.println(" WRITING MEMORY:" + String(c__ > 0 ?"true":"false") );
	display.display();
	delay(2000);
	*/
}

// get user  values
void userGetValues() {
	// user saved condition in long term memory 'EEPROM'
	userP0 = EEPROM.read(MEMORY_P0); // for false = C-H = true Cooling Heating
	userP1 = EEPROM.read(MEMORY_P1); // for temperature  logic range offset '0.5' range
	userP2 = EEPROM.read(MEMORY_P2); // working gap time
	userP3 = EEPROM.read(MEMORY_P3); // over working timeout when working too long
	userP4 = EEPROM.read(MEMORY_P4); // over working cooldown when working too long
	userP5 = EEPROM.read(MEMORY_P5); // over working cooldown when working too long
}


// write to memory ones 
bool writeMemory(int16_t address, byte value) {
	
	if (EEPROM.read(address) != value) {
		EEPROM.write(address, value);
		return true;
	}

	return false;
}

// use for converting byte 255 to decimal 25.5
float userP2FixLogicRangeOffse() {
	byte userFixP1 = 10;  // divide from 255 to get 0.5 float

	return float(userP1) / userFixP1;
}

//fix temperature from 1 byte 255, divinding to half 255/2 = 127 and 127 substracting by 63 to get minus -63 to plus +63 range
float userP5FixTemperature() {
	byte userFixP5 = 2;  // divide to get 255(1 byte) to 255 minutes

	// convert to decimal dividing 256/2 = 127.5
	float fix = float(userP5) / userFixP5;
	// detect when reched zero point
	// set zero point work like signed int but from the byte
		fix = 63.0 - fix;
	return fix;
}


// detect when offset range was entered and exaped, return true when escaped
bool userP2FixLogicRangeOffsetHeatingCompleted() {

	return false;
}

bool userP2FixLogicRangeOffsetCoolingCompleted() {

	return false;
	
} 





// loop working and not working timing by time
void overworkTimer(bool reset_timer) {

	if (reset_timer)
	{
		userP4_CooldownC = 0;
		userP3_TimeoutC = 0;
		boolUserP3_Timeout = false;
		//clock_1min = millis(); // reset each  60 seconds  time // may take 
		return;// jump out of function 
	}

	// over working timer reset and set
	// trigger when program is started
	// disable when working passively


	if (((long)clock_1min + 60000UL) < millis())
	{

		clock_1min = millis(); // reset each  60 seconds  time


		// Cooldown time cownting up
		// boolUserP3_Timeout can be use as mode result
		if (boolUserP3_Timeout) {
			userP4_CooldownC++;

			if (userP4 <= userP4_CooldownC) // reset to 'do' job until overwork mode are entered
			{
				boolUserP3_Timeout = false;
				userP4_CooldownC = 0;
			}
		}
		// when overwork time are reached then set time to cooldown timer
		else if (userP3 <= userP3_TimeoutC) {
			boolUserP3_Timeout = true;
			userP3_TimeoutC = 0; // reset timert to 0
		}
		else {
			// Overworking time counting up
			userP3_TimeoutC++;
		}

	}

	

}