
/*

--Sensors--
-Water Preasure Sensor (analog value )
-Water Flow Sensor (return 1 or 0 each time, hall effect sensor)
-Well System Minimum Water Sensor (return 1 or zero)
-Exseption Water Heater when working
-Button, Start Program after was stopped

--Output Signals--
To a Pump (Water source from a ground)
To a Solenoid Valve (Water intake from paid source)
Bepper if out of water


-- Working principles
-Pump water fail from the well after 45m (if preasure not go up), Stops to rest for some time
-Keep up with a preasure but mously react to a  any reasonable flow of water out of pump.
-Forse on a motor for some time to fill a tanks




*/





#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Water Level in Well 
const static int minimumWaterSensor = A0;
const static int minimuWaterSensorTransistor = 9; // Transistor that activate current going throw sensor.
int minimumWaterLevel = 0;
int minimumWaterCounter = 0;// delay of incominge logic pulses from sensor

//Hidrofore Relay
const static int relayHidrofore = 13;
int hidroforeSensorLevel = 0;
const static int hidroforeSensor = A1;
int  relayHidroforeCounter = 0;

//Beep No Water Indicator
const static int BeepModeIndicator = 11; //analog
int8_t BeepModeCounter = 0;

//ValveRelay open water from the city
const static int valveRelay = 8;
int  valveRelayCounter = 0;

//led1Indicator No Water INDICATOR
const static int led1ModeIndicator = 10;


//pots
const static int pot1_pin = A4;

int captureValuePot1 = analogRead(pot1_pin);

//exseption 
const static int8_t  exeptionCurrentSensor = A5;
bool  exeptionCurrentSensorBool = false;


bool modeManualAuto = false; // determinate if react to water level chages or not / Manual Not Chenck Water Level/ Auto Check Water Level /\ true=Manual
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void setup() {
	pinMode(minimumWaterSensor, INPUT);
	pinMode(minimuWaterSensorTransistor, INPUT);
	pinMode(exeptionCurrentSensor, INPUT);

	pinMode(relayHidrofore, OUTPUT);
	pinMode(led1ModeIndicator, OUTPUT);
	pinMode(valveRelay, OUTPUT);
	pinMode(BeepModeIndicator, OUTPUT);
	//analogWrite(led1ModeIndicator,255);//testing purpose
	//analogWrite(8, 255);//testing purpose
	analogWrite(BeepModeIndicator, 35);
	Serial.begin(9600);
	Serial.print("ON");
	display.begin();
	display.setContrast(60); //Set contrast to 50
	display.clearDisplay();
	display.display();
	analogWrite(BeepModeIndicator, 0);
}

int c = 0;
void loop() {

	++c;



	int  pot1 = analogRead(pot1_pin);
	bool waterLevelSensorRezultBool = WaterLevelSensorRead();
	bool hidroforeSensorRezultBool = HidroforeSensorRead();
	exeptionCurrentSensorBool = ExeptionCurrentSensorRead();


	if (pot1 > captureValuePot1 + 5 || pot1 < captureValuePot1 - 5) //chech if after some time pot value is chahged
	{
		captureValuePot1 = pot1;
		display.clearDisplay();
		display.setCursor(0, 5);
		display.setTextSize(2);

		if ((pot1 < 512))
		{
			display.print("Auto");
			modeManualAuto = false;
		}
		else {
			display.print("Manual");
			modeManualAuto = true;
		}
		display.display();
		delay(300);
		return;
	}

	//Serial.println("pot1:" + String(pot1));
	bool allOn = false;

	if (waterLevelSensorRezultBool && (hidroforeSensorRezultBool || exeptionCurrentSensorBool) || modeManualAuto && hidroforeSensorRezultBool) // if find out about sensor are hight then means water is enouth
	{

		onRelayHidrofore();

		allOn = true;
	}
	else {
		offRelayHidrofore();
		// if fails to drain water then on valve relay vto provide city water
		if ((exeptionCurrentSensorBool || hidroforeSensorRezultBool) && !modeManualAuto) {
			onRelayValve();
		}
		else
			offRelayValve();
	}

	// LED indicate if Water Is In Well
	if (!waterLevelSensorRezultBool && !modeManualAuto) {
		analogWrite(led1ModeIndicator, 20);
		onBeepModeIndicator();
	}
	else {
		analogWrite(led1ModeIndicator, 0);
		offBeepModeIndicator();
	}





	if (allOn && !exeptionCurrentSensorBool) { delay(100); display.clearDisplay(); display.setTextSize(2); display.setCursor(16, 0); display.println("FREE  WATER"); display.display(); return; };
	if (exeptionCurrentSensorBool) { delay(100); display.clearDisplay(); display.setTextSize(2); display.setCursor(16, 0); display.println("BOOST--|:|--"); display.display(); return; }
	display.setTextSize(1);
	display.clearDisplay();
	//display.setTextColor(BLACK, WHITE);
	display.setCursor(0, 0);
	display.println("W-level:" + String(minimumWaterLevel));
	//display.setCursor(0,20);
	display.println("Hidro:" + String(hidroforeSensorLevel));
	if (relayHidroforeCounter > 0) { display.println("Hidro-time:"); display.setTextSize(3); display.println(String(" ") + centerInt(relayHidroforeCounter)); }
	else if (valveRelayCounter > 0) { display.println("Valve-time:"); display.setTextSize(3); display.println(String("") + centerInt(valveRelayCounter)); }
	display.setTextSize(1);




	if (c >= 9) c = 0;
	display.display();

	delay(10);

	delay(500);
}

bool WaterLevelSensorRead() {
	analogWrite(minimuWaterSensorTransistor, 255); //turn on transitor that transmit vcurrent
	delay(4);
	minimumWaterLevel = analogRead(minimumWaterSensor);
	delay(4);
	//        Serial.println("WaterLevel:" + String(minimumWaterLevel)+ bol(minimumWaterLevel>50));
	analogWrite(minimuWaterSensorTransistor, 0);

	//UpdateTimerCount
	delay(4);

	minimumWaterCounter--;

	if (minimumWaterLevel > 500) { //bugdplst
		minimumWaterCounter = 20; // ~ substract 2 = time
	};



	if (minimumWaterCounter > 0)
		return true;
	return false;

}


bool HidroforeSensorRead() {

	analogWrite(minimuWaterSensorTransistor, 255);// activate +5 to go throw sensor
	delay(4);
	hidroforeSensorLevel = analogRead(hidroforeSensor);
	delay(4);
	//    Serial.println("Hidrofore:" + String(hidroforeSensorLevel)+ bol (hidroforeSensorLevel >50) );
	delay(2);
	return (hidroforeSensorLevel > 50);
}
void onRelayHidrofore() {
	analogWrite(relayHidrofore, 255);
	relayHidroforeCounter = 7;
};
void offRelayHidrofore() {
	if (relayHidroforeCounter <= 0)
		analogWrite(relayHidrofore, 0);
	else
		relayHidroforeCounter--; // fade out of fast swithing

};


bool ExeptionCurrentSensorRead() {
	int readthis = analogRead(exeptionCurrentSensor);

	if (readthis > 100) {
		Serial.println("Exeption: " + String(readthis));
		return true;
	}
	else {
		return false;
	}


};

void onRelayValve() {
	analogWrite(valveRelay, 255);
	valveRelayCounter = 10;
};
void offRelayValve() {
	if (valveRelayCounter <= 0)
		analogWrite(valveRelay, 0);
	else
		valveRelayCounter--; // fade out of fast swithing
};

void onBeepModeIndicator() {
	analogWrite(BeepModeIndicator, 0);
	if (BeepModeCounter <= 0) // set 1/3 sound and light signal on
		analogWrite(BeepModeIndicator, 35);

	BeepModeCounter++;

	if (BeepModeCounter >= 6)
		BeepModeCounter = 0;
}

void offBeepModeIndicator() {
	analogWrite(BeepModeIndicator, 0);
}
String centerInt(int txt) {
	if (txt > 10)
		return String(txt);
	else
		return (String(" ") + String(txt));
}

String bol(bool var) {
	if (var) return " true "; else return " false ";
}

#pragma once



--------------------------------------------------------



//  Programed for LGT8F328P Arduino Pro Mini Clone.
// 'LGT8F328P-LQFP32 MiniEVB'
// At 79% project progress , start do 'crashing-buggy' stuff... 23.428 bytes,maybe more  22,706
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
#include "EEPROM32.h"
//#include <time.h>


// Variables

bool boolSetButton = false;
bool boolSetMenu = false;
bool boolQuicklyChange = false;
byte meniuOptionSelected = 1;
byte meniuOptionsLenght = 8; // how much meniu option in meniu
bool meniuOptionIsPressing = false;// detect when button no longer is press
bool meniuOptionIsSelected = false; // When eventualy are at P0-P3 option then time to select what specificly are changing a values that are included in the statement.
unsigned long startedWaiting;
unsigned long startedWaitingmeniuOptionSelected;
//unsigned long startedWaitingOwerworkTimeout; // starting point for overwork protection
//bool boolStartedWaitingOwerworkTimeout = true; // trigger ones in owvrwork mode cycle to set caunter 'startedWaitingOwerworkTimeout' starting point 
bool boolUserOffsetTemperatureRange = false;  // return true when completed temperature range escape
bool boolUserP3_Timeout;
unsigned long clock_1min = 0;
// always count down when was worked 
byte userP2Timeout = 0;
bool boolUserP2Timeout = true;
// total working time counter userP6EnergyOffMin
unsigned long int userP6EnergyOnMin = 0;
unsigned long int userP6EnergyOffMin = 0;


/// button events variable
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

// user saved condition in long term memory 'EEPROM'
byte   userP0; // for 0 = C-H = 1 Cooling Heating  +
byte   userP1; // for temperature  logic range offset '0.5' range +
byte   userP2; // last on timeout to prevent frequent fridge on and off
byte   userP3; // over working timeout when working too long
byte   userP4; // over working cooldown when working too long
byte   userP5; //1byte, save temperature that must be reached

// user values that use to multyply or divide from 255(1byte) to get propper rezult

byte   userP3_TimeoutC = 0; //counter over working timeout when working too long
byte   userP4_CooldownC = 0; //counter  over working cooldown when working too long

//byte userFixP2 = 60;  // multyplies to get 255(1 byte) to 255 minutes
//byte userFixP3 = 60;  // multyplies to get 255(1 byte) to 255 minutes
//byte userFixP4 = 60;  // multyplies to get 255(1 byte) to 255 minutes


// Common Text Variables
String Cool = "Cool";
String Heat = "Heat";
String down = "down";

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
#define POWER_USAGE 65 // power usage per hour

// user saved condition in long term memory 'EEPROM'
// byte size is 1, int size is 2 byte's, float size is 4 byte's
#define MEMORY_FIRSTBOOT 0  // to set default values for a new arduino unit
#define MEMORY_P0 1  //1byte, for false = C-H = true Cooling Heating
#define MEMORY_P1 2  //1byte, for temperature  logic range offset '0.5' range
#define MEMORY_P2 3  //1byte, working timeout
#define MEMORY_P3 4  //1byte,  over working timeout when working too long
#define MEMORY_P4 5  //1byte,  over working cooldown when working too long
#define MEMORY_P5 6 //1byte, save temperature that must be reached
#define MEMORY_P6ON 7 // +4 bytes energy counter
#define MEMORY_P6OFF 11 // +4 bytes energy counter

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     1 // 4 or Reset pin # (or -1 if sharing Arduino reset pin) '1' one is with LGT8F328P32'pins' But //#include <avr/power.h> library not working before SSD1306_SWITCHCAPVCC placing
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ThermoSencor sensorTemp(SENSOR_TEMP0, 9370);

// include function after display was declarated 
//#include "functions.h"

float powerUsageMin = POWER_USAGE / 60; // power used in minute

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
int tst = 55;
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
					display.println("[P" + String(meniuOptionSelectFun() - 1) + "]");

				if (meniuOptionSelectFun() == 1) { // P0 C-H
					//display.println("[P0]");
					meniuDescribeOptionDisplay("<Mode:" + Cool + "," + Heat);

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
				else if (meniuOptionSelectFun() == 3) { // Not included
					//display.println("[P2]");
					meniuDescribeOptionDisplay("Frequent on timeout ");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						userChangeMeniuValue(&userP2, 10, 0);
						// Display
						txtBigNumber(userP2, "min");
					}

				}

				else if (meniuOptionSelectFun() == 4) { // P3 Timer count while relay is on, untit set a limit are reached.
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

				else if (meniuOptionSelectFun() == 5) { // P4 Timer to off relay when are in cooldown mode
					//display.println("[P4]");
					meniuDescribeOptionDisplay("<Overworking " + Cool + down);

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

				else if (meniuOptionSelectFun() == 7) {
					/*
					//display.println("[R6]"); energyOffMin energyOffMin
					meniuDescribeOptionDisplay("<Reset energy wasted>");
					// do EEPROM changes
					if (meniuOptionIsSelected)
					{
						display.println("Press up to reset");

						if (buttonUP)
						{
							display.setTextSize(4);
							display.println(" Done");
							display.display();
							// reset to default
							userP6EnergyOffMin = 0;
							userP6EnergyOnMin = 0;

							writeEEPROM32(MEMORY_P6OFF,0);
							delay(1);
							writeEEPROM32(MEMORY_P6ON,0);

							delay(2000);
							meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
						}
					}
					*/

					// too much memory usage
					/*

							meniuDescribeOptionDisplay("Power Usage Per Day");
						//  power usage per day ratio
						if (meniuOptionIsSelected)
						{

							display.println(String( int(powerUsageMin * float(userP6EnergyOnMin)  / ((userP6EnergyOffMin / 60) / 24)) ) + "w");
						}
					*/


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
				userChangeMeniuValue(&userP5, 100);



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







	// if temperature sensor is connected then do program
	if (sensorTemp.temperature > -65)
	{


		overworkTimer(boolOverworkState || /* or ignore counting if protection timer is on*/ userP2Timeout > 0);

		if (((long)clock_1min + 60000UL) < millis())
		{

			clock_1min = millis(); // reset each  60 seconds  time

			if (userP2Timeout > 0)
				userP2Timeout--;


			userP6EnergyOffMin++; // count in each minute

		}



		display.setCursor(0, 9);

		// Display status about timeouts
		if (userP2Timeout == 0)
		{
			if (boolUserP3_Timeout) {
				display.println(Cool + down + ":" + String(userP4_CooldownC) + "/" + String(userP4) + "min" /*+ " bk:" + String(userP2Timeout)*/);
			}
			else
			{
				display.println("Timeout:" + String(userP3_TimeoutC) + "/" + String(userP3) + "min" /*+ " bk:" + String (userP2Timeout)*/);
			}
		}
		else
		{// delay when
			display.println("Next On:" + String(userP2Timeout) + "min");
		}
		display.println(convertMinutesToTime(&userP6EnergyOffMin) + "->" + String(int(powerUsageMin * float(userP6EnergyOnMin))) + "w");

		display.setCursor(0, 0);

		// for 0 = C-H = 1 Cooling Heating 6.59
		if (userP0 == 0)
		{


			display.print(Cool);
			if (!boolUserOffsetTemperatureRange)
				display.print("ing:" + String(userP5FixTemperature()));
			else
				display.print("-stoped:" + String(userP5FixTemperature() + userP2FixLogicRangeOffse()));
			display.println("c"/*+ String(boolUserP3_Timeout)*/);

			//display.println("TimeoutC:" + String(userP3_TimeoutC));
			//display.println("CooldownC:" + String(userP4_CooldownC));



		// offset mode
		// when temperatue are rising after feezer was on then wait more temperature offset to confirm that is not a fluctuation
		//sensor_25 > (24 + 2.0) = 25 > 27 
			if (sensorTemp.temperature > (userP5FixTemperature() + userP2FixLogicRangeOffse())) {
				boolUserOffsetTemperatureRange = false;

			}


			// if temperate is  less then should be,      //25 > (24 - 2.0)/ 25 > 22                         

			if ((sensorTemp.temperature > userP5FixTemperature() && !boolUserOffsetTemperatureRange))
			{



				// initiation completed, not count down working time until reached critical point
				boolOverworkState = false; // keep counting working time 


				if (!boolUserP3_Timeout && userP2Timeout == 0) // if reached overwork state then turn off relay and protection timer equal zero
				{
					boolUserP2Timeout = true; // // reset when fridge was on 
					digitalWrite(RELAY_FRIDGE0, 255);
				}
				else
					digitalWrite(RELAY_FRIDGE0, LOW);

			}
			else
			{

				if (boolUserP2Timeout) {  // set only ones to countdown timer
					userP2Timeout = userP2;
					boolUserP2Timeout = false;
				}

				boolUserOffsetTemperatureRange = true; // set to offset mode
				boolOverworkState = true; // disable counting wornking time and reset
				digitalWrite(RELAY_FRIDGE0, LOW);

			}



		}  // Heateing
		else
		{


			display.print(Heat);
			if (!boolUserOffsetTemperatureRange)
				display.print("ing:" + String(userP5FixTemperature()));
			else
				display.print("-stoped:" + String(userP5FixTemperature() - userP2FixLogicRangeOffse()));
			display.println("c"/* + String(boolUserP3_Timeout)*/);

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

				if (!boolUserP3_Timeout && userP2Timeout == 0) // if reached overwork state then turn off relay and protection timer equal zero  
				{
					boolUserP2Timeout = true; // reset when fridge was on
					digitalWrite(RELAY_FRIDGE0, 255);
				}
				else
					digitalWrite(RELAY_FRIDGE0, LOW);

			}
			else
			{

				if (boolUserP2Timeout) { // set only ones to countdown timer
					userP2Timeout = userP2;
					boolUserP2Timeout = false;
				}


				boolUserOffsetTemperatureRange = true; // set to offset mode
				boolOverworkState = true; // disable counting wornking time and reset
				digitalWrite(RELAY_FRIDGE0, LOW);

			}




		}
	}
	else { // if no sensor connected, print error message
		//display.clearDisplay();
		//display.setTextSize(2);

		//display.println("Nera");
		//display.println("Temperat.");
		//display.println("Sensor.");
		display.println("?:" + String(sensorTemp.temperature));
		digitalWrite(RELAY_FRIDGE0, LOW);

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
	userP1 = 5; // for temperature  logic range offset 5/10 = '0.5'celcius range
	userP2 = 4; //min last on timeout to prevent frequent fridge on and off
	userP3 = 25; //min over working timeout when working too long
	userP4 = 10;  //min over working cooldown when working too long
	userP5 = 20;  //50 celcius save temperature that must be reached

	userSetValuesToMemory();
}

// set user all values to memory
void userSetValuesToMemory() {

	byte c__ = 0;
	c__ += writeMemory(MEMORY_P0, userP0); // for C-H Cooling Heating
	c__ += writeMemory(MEMORY_P1, userP1); // for temperature  logic range offset '0.5' range
	c__ += writeMemory(MEMORY_P2, userP2); // last on timeout to prevent frequent fridge on and off
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
	//userP6EnergyOffMin = readEEPROM32(MEMORY_P6OFF);
	//userP6EnergyOnMin = readEEPROM32(MEMORY_P6ON);

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

		//clock_1min = millis(); // reset each  60 seconds  time


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
			userP6EnergyOnMin++;
		}

	}



}


String convertMinutesToTime(unsigned long int* time) {

	String text = "";

	/*	*min
		43,200 mouth
		1,440 day

		int days = input_seconds / 60 / 60 / 24;
		int hours = (input_seconds / 60 / 60) % 24;
		int minutes = (input_seconds / 60) % 60;
		int seconds = input_seconds % 60;


	*/

	int_fast16_t min = (int_fast16_t)(*time % 60);
	int_fast16_t h = (int_fast16_t)((*time / 60) % 24);
	int_fast16_t d = (int_fast16_t)((*time / 60) / 24);
	//int_fast16_t motuh = (int_fast16_t)( ((*time / 60) / 24) / 30 );
	// int_fast16_t year = (int_fast16_t)((((*time / 60) / 24) / 30) / 12 );

		//text += String(year) + "y";

		//text += String(motuh) + "m";

	text += String(d) + "d";

	text += String(h) + "h";

	text += String(min);


	return text;
}



// Simple I2C test for ebay 128x64 oled.
 // double a speed and delay time are reduced 2 times
#include <avr/power.h> // library not working propietly with oled display '<Adafruit_SSD1306.h>' if placed before initiliaziniot in setup() clock_prescale_set(clock_div_1); loop

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire display;




// overall
#define MAINSOCKET_Frequency 50 /* *  2  */ // Frequency of main power source rectified!

// memory address
	// on units if selected
#define MEMORY_ON_WaterInValveSignal  1
#define MEMORY_ON_WaterOutPumpSingnal 2
#define MEMORY_ON_WaterPumpSprayerSignal 3
#define MEMORY_ON_HeaterWaterSignal 4
	//speed control of motor
#define MEMORY_M_NORMALWASHPOWER 5 // normal power o a motor that usuly determent how triac is on
#define MEMORY_M_NORMALWASHSPEED 6 // speed nominal program read and align

#define MEMORY_M_RISEWASHSPOWER 7 // power of maximum that usuly determent how triac is on
#define MEMORY_M_RISEWASHSPEED 8 // speed maximu program read and align
#define MEMORY_M_NORMALWASHINTERVALON 9 // how much time in the normal wash mode a motor must work
#define MEMORY_M_NORMALWASHINTERVALOFF 10 // how much time in the normal wash mode a motor must Not to work
// buttons
#define BUTTON_DOWN 5
#define BUTTON_SET 4
#define BUTTON_UP 6
//  Swithing signals
#define RELAY_WaterInValveSignal 7
#define RELAY_WaterOutPumpSingnal 9
#define RELAY_WaterPumpSprayerSignal 10
#define RELAY_HeaterWaterSignal 8
#define RELAY_MotorRelayOn 13
#define RELAY_TRIAC_MotorSpeedControl 11
// sensors
#define SENSOR_TEMP0 A1
#define SENSOR_TEMP1BACKUP A4
// relays
#define RELAY_FRIDGE0 13
#define RELAY_FRIDGE1BACKUPRead A4
#define POWER_USAGE 65 // power usage per hour

// Interrupts
#define INTERRUPT_ZeroCrossDetection 0 // detect when phase angle reach zero volts crossing in AC
#define INTERRUPT_TachoMeterSensor 1 // read pulses from a motor in real time 
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1


// Variables
String reportComponentsWork = "";
byte spinSide = 0;
//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long clock_1mlsec = 0;
// timer variabls
byte timerMotorWork = 0; // timer for reapeting 
bool timerMotorWorkBool = false;
//
bool button_SET_PRESSED = false;
bool startWokProgram = false;

// Frequency Control a load 
byte frequencyControlLoadCounter = 0; // each time count a progress
byte frequencyControlLoad = 30; // compare if Counter is equal to This variable value to execute selecte frequency

int_fast16_t clockTotalMin = 0;
int_fast16_t motorPulseWidthTime; // save stored timer for triggering a triac

byte motorSpeedRequest = 0;
byte motorSpeed = 0;

byte meniuIndex = 0;
byte meniuValue = 0;

//memory temporery stored values
bool value_ON_WaterInValveSignal = 0;
bool value_ON_WaterOutPumpSingnal = 0;
bool value_ON_WaterPumpSprayerSignal = 0;
bool value_ON_HeaterWaterSignal = 0;
//speed control of motor
byte value_M_NORMALWASHPOWER = 0;// normal power o a motor that usuly determent how triac is on
byte value_M_NORMALWASHSPEED = 0; // speed nominal program read and align

byte value_M_RISEWASHSPOWER = 0; // power of maximum that usuly determent how triac is on
byte value_M_RISEWASHSPEED = 0;// speed maximu program read and align

byte value_M_NORMALWASHINTERVALON = 1; 
byte value_M_NORMALWASHINTERVALOFF = 1;

//------------------------------------------------------------------------------

#include "EEPROM32.h"
#include "functions.h"


//------------------------------------------------------------------------------
void setup() {
	Wire.begin();
	Wire.setClock(800000L);

#if RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

	display.setFont(Adafruit5x7);

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);

	pinMode(RELAY_HeaterWaterSignal, OUTPUT);
	pinMode(RELAY_WaterInValveSignal, OUTPUT);
	pinMode(RELAY_WaterOutPumpSingnal, OUTPUT);
	pinMode(RELAY_WaterPumpSprayerSignal, OUTPUT);
	pinMode(RELAY_MotorRelayOn, OUTPUT);
	pinMode(RELAY_TRIAC_MotorSpeedControl, OUTPUT);
	//pinMode(2, INPUT);
	attachInterrupt(INTERRUPT_ZeroCrossDetection, MotorSpeedControlFuncZeroCrossDetected, FALLING);
	
	attachInterrupt(INTERRUPT_TachoMeterSensor, TachoMeterSensor, FALLING);

		
	clock_prescale_set(clock_div_1);// double the speed setup

	// set up memory default 
	if (readMemoryByte(0) != true) {

		print("MEMORY SET!");
		writeMemory(0, true);

		
		writeMemory(MEMORY_ON_WaterInValveSignal, true);
		writeMemory(MEMORY_ON_WaterOutPumpSingnal, true);
		writeMemory(MEMORY_ON_WaterPumpSprayerSignal, true);
		writeMemory(MEMORY_ON_HeaterWaterSignal, false);

		writeMemory(MEMORY_M_NORMALWASHPOWER, byte (4));
		writeMemory(MEMORY_M_NORMALWASHSPEED, byte(20));

		writeMemory(MEMORY_M_RISEWASHSPOWER, byte(4));
		writeMemory(MEMORY_M_RISEWASHSPEED, byte(20));

		writeMemory(MEMORY_M_NORMALWASHINTERVALON, byte(2));
		writeMemory(MEMORY_M_NORMALWASHINTERVALOFF, byte(3));
	}


	updateValuesfromMemory();
}





		bool setButtonPressed = false;
void loop() {
	
	display.setCursor(0, 0);
	//display.clear();

	bool buttonUP = !digitalRead(BUTTON_UP);
	bool buttonSET = !digitalRead(BUTTON_SET);
	bool buttonDOWN = !digitalRead(BUTTON_DOWN);
	

	byte rotory = rotaryEncoderDirection(&buttonUP, &buttonDOWN);


	if (rotory == 1  ) {
		
		if (!button_SET_PRESSED && meniuIndex != 0)
			meniuIndex --; // select meniu index
		else
		{
			if (meniuValue != 0)
				meniuValue--; // when se;ecting meniu , change a value
		}

	}
	else if (rotory == 2 )
	{
		
		if (!button_SET_PRESSED)
			meniuIndex++; // select meniu index
		else {
			
			//if (meniuIndex >)
				meniuValue++; // when se;ecting meniu , change a value
		}
		
	
	}

	// meniu set and show
	
		uploadValues(meniuIndex, &meniuValue, button_SET_PRESSED);
		


	//                      16MHz	 32MHz
	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed 
	if (((long)clock_1min + 120000UL) < millis()) // 120000UL a double clock speed by
	{
		
		clock_1min = millis(); // reset each  60 seconds  time
		clockTotalMin+=1;
		display.clear();

		if (timerMotorWorkBool) {
			timerMotorWork += 1;

		//	print("ADDDDD" + String (timerMotorWork) );
		//	delay(4000);
		}

	}
	
	/// CLOCK 1 sec
	if (((long)clock_1sec + 600UL) < millis())
	{

		clock_1sec = millis(); // reset each  1 seconds  time
		
		if (motorSpeedRequest > motorSpeed) {
			motorSpeed+=1;
		}
		else if (motorSpeed > 0) {
			motorSpeed-=1;
		}
		
		
	}

	// using diac

	//7200ms in 170 bits start to colapse and disappear
	//8200ms in 180 bits start to colapse and disappear
	//0.6 ms delay while triac start to react looking in scope
	//in 9379ms start open half sine wave// buggy
	//in 8991ms start open full sine wave //  risky in practice // in pratice 8200ms open triac, 
	//
	//----------
	//not using diac protection
	//in 9961 start open full sine wave// not buggy but dangerous
	//

	motorPulseWidthTime = map(motorSpeed, 160, 0, 150, 8990); //7200 


	//print(String (clockTotalMin) + " min");
	//print(meniuText[meniuIndex]);
	//print(String (meniuOption[meniuIndex]));



	if (buttonSET) {

		
			//meniuOption[meniuIndex] = !meniuOption[meniuIndex];
		
				button_SET_PRESSED = !button_SET_PRESSED;

			if (!button_SET_PRESSED) // update changed option to a memory when pressing out of meniu
				updateMemoryValues();

					//meniuValue = 0; // reset 
				delay(300*2);
	}

	




	if (startWokProgram ) {

		println("" + String(clockTotalMin) + "m|");// nx:"+String(timeChangeVar) );

		if (clockTotalMin < 10) { // if (clockTotalMin < timeChange(7 ,0)) {
			
			WaterInValveSignal(true);
			print("Filling water>");
		}
		else if (clockTotalMin < 24) {
			WaterInValveSignal(false);
			print("Spin motor");
			print("Heat>");
		

			motorNormalWashInterval( );
			// turn on sprayer
			
		
			HeaterWaterSignal(true);
			
		}

		else if (clockTotalMin < 25) { // soft relays transition
			print("Spray On>");
			WaterPumpSprayerSignal(true);
			HeaterWaterSignal(false);
		}
		else if (clockTotalMin < 40) {
			print("Spin motor");
			print("Spray water>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(true);
			WaterPumpSprayerSignal(true);
			

		}
		
		else if (clockTotalMin < 55) {
			print("Spin motor");
			print("Spray's Rest>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(false);
			WaterPumpSprayerSignal(false);


		}
		else if (clockTotalMin < 56) {
			print("Spray On>");
			WaterPumpSprayerSignal(true);

		}
		else if (clockTotalMin < 73) {
			print("Spin motor");
			print("Spray water>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(true);
			WaterPumpSprayerSignal(true);


		}
		else if (clockTotalMin < 76) {
			print("Wait time>");
			
			
			WaterPumpSprayerSignal(false);
			WaterOutPumpSingnal(false);
			motorTunOff();
		}
		else if (clockTotalMin < 90) {
			print("Push water out>");
			
			WaterOutPumpSingnal(true);
			HeaterWaterSignal(false);
			//motorNormalWashInterval(2, 1);
			motorNormalWashInterval(); // was by selected interval
		}
		else if (clockTotalMin < 93) {
			print("Off pumps>");

			WaterOutPumpSingnal(false);
			WaterPumpSprayerSignal(false);
			motorTunOff();
		}
		else 
		{	

			print("Finished!");
			
			startWokProgram = 0; // program end 
			motorTunOff();
			timeChangeReset();
		}
		
		
	}
	
		

			//onOff(RELAY_WaterInValveSignal, meniuOption[1]);

			//onOff(RELAY_WaterOutPumpSingnal, meniuOption[2]);

			//onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3]);

			//onOff(RELAY_HeaterWaterSignal, meniuOption[4]);
		

	
	else
	{

		onOff(RELAY_HeaterWaterSignal, false);
		onOff(RELAY_WaterInValveSignal, false);
		onOff(RELAY_WaterOutPumpSingnal, false);
		onOff(RELAY_WaterPumpSprayerSignal, false);
		motorTunOff();


		clockTotalMin = 0; // reset clock if program not started


		if (timeChangeCnt != 0) // reset single time a timeChange() function to work next time 
			timeChangeReset();

	}

	

	
	//print(meniuOption[meniuIndex]);

	

	
	

		


		print("mltime:" + String((motorPulseWidthTime)));
		print("PWM:" + String(motorSpeed) + " / "+ String (motorSpeedRequest));
		
		print("W-"+reportComponentsWork);
		// clear last lines
		
		
		print("");
		print("");

		reportComponentsWork = ""; // reset report string 
}






byte changedValueInterrupt = 0;
// Interrupts functions
void MotorSpeedControlFuncZeroCrossDetected() {

	
	


	// triac on condition
		
	//if (/*changedValueInterrupt != meniuIndex &&*/  frequencyControlLoadCounter >= frequencyControlLoad) {
		/*changedValueInterrupt = meniuIndex;*/
		// power control

		if (motorSpeed > 0) {


			delayMicroseconds(motorPulseWidthTime*2); //only delayMicroseconds can work in interrupt functions
			digitalWrite(RELAY_TRIAC_MotorSpeedControl, HIGH);

			// Serial.println(digitalRead(triacPulse));

			delayMicroseconds(50*2);  //delay   uSec on output pulse to turn on triac and 50uS no trigger all the time
			
			// reset a signal was set in triac condition
			
		}
		// reset frequency counter  
		//frequencyControlLoadCounter = 0;
	//}
	 
		digitalWrite(RELAY_TRIAC_MotorSpeedControl, LOW);
	//frequencyControlLoadCounter++; // counting runtime
	

	 
}



void TachoMeterSensor() {

	
}

void frequencyControlAdd(bool validate) {

	if (validate && (frequencyControlLoad < MAINSOCKET_Frequency)) {
		frequencyControlLoad= frequencyControlLoad + 2;
		//frequencyControlLoadCounter = 0; // reset to not delay in next cycle
	}

}


void frequencyControlSubstract(bool validate) {

	if (validate && (frequencyControlLoad > 1) ) {
		frequencyControlLoad= frequencyControlLoad - 2;
		//frequencyControlLoadCounter = 0; // reset to not delay in next cycle
	}
}




void motorNormalWash() {

	motorSpeedRequest = value_M_NORMALWASHPOWER; //  readMemoryByte(MEMORY_M_NORMALWASHPOWER);

}




void motorTurnOffRequest() { // only turn off request of a motor 
	motorSpeedRequest = 0;
}


// turn off everything 
void motorTunOff() {

	motorSpeedRequest = 0;
	timerMotorWork = 0; //reset timer for turn off a motorNormalWashInterval function
	timerMotorWorkBool = false;
}

---------------------------------------------------------------------------



// Simple I2C test for ebay 128x64 oled.
 // double a speed and delay time are reduced 2 times
#include <avr/power.h> // library not working propietly with oled display '<Adafruit_SSD1306.h>' if placed before initiliaziniot in setup() clock_prescale_set(clock_div_1); loop

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
	SSD1306AsciiWire display;




// overall
#define MAINSOCKET_Frequency 50 /* *  2  */ // Frequency of main power source rectified!

// memory address
	// on units if selected
#define MEMORY_ON_WaterInValveSignal  1
#define MEMORY_ON_WaterOutPumpSingnal 2
#define MEMORY_ON_WaterPumpSprayerSignal 3
#define MEMORY_ON_HeaterWaterSignal 4
	//speed control of motor
#define MEMORY_M_NORMALWASHPOWER 5 // normal power o a motor that usuly determent how triac is on
#define MEMORY_M_NORMALWASHSPEED 6 // speed nominal program read and align

#define MEMORY_M_RISEWASHSPOWER 7 // power of maximum that usuly determent how triac is on
#define MEMORY_M_RISEWASHSPEED 8 // speed maximu program read and align
#define MEMORY_M_NORMALWASHINTERVALON 9 // how much time in the normal wash mode a motor must work
#define MEMORY_M_NORMALWASHINTERVALOFF 10 // how much time in the normal wash mode a motor must Not to work
// buttons
#define BUTTON_DOWN 5
#define BUTTON_SET 4
#define BUTTON_UP 6
//  Swithing signals
#define RELAY_WaterInValveSignal 7
#define RELAY_WaterOutPumpSingnal 9
#define RELAY_WaterPumpSprayerSignal 10
#define RELAY_HeaterWaterSignal 8
#define RELAY_MotorRelayOn 13
#define RELAY_TRIAC_MotorSpeedControl 11
// sensors
#define SENSOR_TEMP0 A1
#define SENSOR_TEMP1BACKUP A4
// relays
#define RELAY_FRIDGE0 13
#define RELAY_FRIDGE1BACKUPRead A4
#define POWER_USAGE 65 // power usage per hour

// Interrupts
#define INTERRUPT_ZeroCrossDetection 0 // detect when phase angle reach zero volts crossing in AC
#define INTERRUPT_TachoMeterSensor 1 // read pulses from a motor in real time 
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1


// Variables
String reportComponentsWork = "";
byte spinSide = 0;
//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long clock_1mlsec = 0;
// timer variabls
byte timerMotorWork = 0; // timer for reapeting 
bool timerMotorWorkBool = false;
//
bool button_SET_PRESSED = false;
bool startWokProgram = false;

// Frequency Control a load 
byte frequencyControlLoadCounter = 0; // each time count a progress
byte frequencyControlLoad = 30; // compare if Counter is equal to This variable value to execute selecte frequency

int_fast16_t clockTotalMin = 0;
int_fast16_t motorPulseWidthTime; // save stored timer for triggering a triac

byte motorSpeedRequest = 0;
byte motorSpeed = 0;

byte meniuIndex = 0;
byte meniuValue = 0;

//memory temporery stored values
bool value_ON_WaterInValveSignal = 0;
bool value_ON_WaterOutPumpSingnal = 0;
bool value_ON_WaterPumpSprayerSignal = 0;
bool value_ON_HeaterWaterSignal = 0;
//speed control of motor
byte value_M_NORMALWASHPOWER = 0;// normal power o a motor that usuly determent how triac is on
byte value_M_NORMALWASHSPEED = 0; // speed nominal program read and align

byte value_M_RISEWASHSPOWER = 0; // power of maximum that usuly determent how triac is on
byte value_M_RISEWASHSPEED = 0;// speed maximu program read and align

byte value_M_NORMALWASHINTERVALON = 1;
byte value_M_NORMALWASHINTERVALOFF = 1;

//------------------------------------------------------------------------------

#include "EEPROM32.h"
#include "functions.h"


//------------------------------------------------------------------------------
void setup() {
	Wire.begin();
	Wire.setClock(800000L);

#if RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

	display.setFont(Adafruit5x7);

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);

	pinMode(RELAY_HeaterWaterSignal, OUTPUT);
	pinMode(RELAY_WaterInValveSignal, OUTPUT);
	pinMode(RELAY_WaterOutPumpSingnal, OUTPUT);
	pinMode(RELAY_WaterPumpSprayerSignal, OUTPUT);
	pinMode(RELAY_MotorRelayOn, OUTPUT);
	pinMode(RELAY_TRIAC_MotorSpeedControl, OUTPUT);
	//pinMode(2, INPUT);
	attachInterrupt(INTERRUPT_ZeroCrossDetection, MotorSpeedControlFuncZeroCrossDetected, FALLING);

	attachInterrupt(INTERRUPT_TachoMeterSensor, TachoMeterSensor, FALLING);


	clock_prescale_set(clock_div_1);// double the speed setup

	// set up memory default 
	if (readMemoryByte(0) != true) {

		print("MEMORY SET!");
		writeMemory(0, true);


		writeMemory(MEMORY_ON_WaterInValveSignal, true);
		writeMemory(MEMORY_ON_WaterOutPumpSingnal, true);
		writeMemory(MEMORY_ON_WaterPumpSprayerSignal, true);
		writeMemory(MEMORY_ON_HeaterWaterSignal, false);

		writeMemory(MEMORY_M_NORMALWASHPOWER, byte(4));
		writeMemory(MEMORY_M_NORMALWASHSPEED, byte(20));

		writeMemory(MEMORY_M_RISEWASHSPOWER, byte(4));
		writeMemory(MEMORY_M_RISEWASHSPEED, byte(20));

		writeMemory(MEMORY_M_NORMALWASHINTERVALON, byte(2));
		writeMemory(MEMORY_M_NORMALWASHINTERVALOFF, byte(3));
	}


	updateValuesfromMemory();
}





bool setButtonPressed = false;
void loop() {

	display.setCursor(0, 0);
	//display.clear();

	bool buttonUP = !digitalRead(BUTTON_UP);
	bool buttonSET = !digitalRead(BUTTON_SET);
	bool buttonDOWN = !digitalRead(BUTTON_DOWN);


	byte rotory = rotaryEncoderDirection(&buttonUP, &buttonDOWN);


	if (rotory == 1) {

		if (!button_SET_PRESSED && meniuIndex != 0)
			meniuIndex--; // select meniu index
		else
		{
			if (meniuValue != 0)
				meniuValue--; // when se;ecting meniu , change a value
		}

	}
	else if (rotory == 2)
	{

		if (!button_SET_PRESSED)
			meniuIndex++; // select meniu index
		else {

			//if (meniuIndex >)
			meniuValue++; // when se;ecting meniu , change a value
		}


	}

	// meniu set and show

	uploadValues(meniuIndex, &meniuValue, button_SET_PRESSED);



	//                      16MHz	 32MHz
	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed 
	if (((long)clock_1min + 120000UL) < millis()) // 120000UL a double clock speed by
	{

		clock_1min = millis(); // reset each  60 seconds  time
		clockTotalMin += 1;
		display.clear();

		if (timerMotorWorkBool) {
			timerMotorWork += 1;

			//	print("ADDDDD" + String (timerMotorWork) );
			//	delay(4000);
		}

	}

	/// CLOCK 1 sec
	if (((long)clock_1sec + 600UL) < millis())
	{

		clock_1sec = millis(); // reset each  1 seconds  time

		if (motorSpeedRequest > motorSpeed) {
			motorSpeed += 1;
		}
		else if (motorSpeed > 0) {
			motorSpeed -= 1;
		}


	}

	// using diac

	//7200ms in 170 bits start to colapse and disappear
	//8200ms in 180 bits start to colapse and disappear
	//0.6 ms delay while triac start to react looking in scope
	//in 9379ms start open half sine wave// buggy
	//in 8991ms start open full sine wave //  risky in practice // in pratice 8200ms open triac, 
	//
	//----------
	//not using diac protection
	//in 9961 start open full sine wave// not buggy but dangerous
	//

	motorPulseWidthTime = map(motorSpeed, 160, 0, 150, 8990); //7200 


	//print(String (clockTotalMin) + " min");
	//print(meniuText[meniuIndex]);
	//print(String (meniuOption[meniuIndex]));



	if (buttonSET) {


		//meniuOption[meniuIndex] = !meniuOption[meniuIndex];

		button_SET_PRESSED = !button_SET_PRESSED;

		if (!button_SET_PRESSED) // update changed option to a memory when pressing out of meniu
			updateMemoryValues();

		//meniuValue = 0; // reset 
		delay(300 * 2);
	}






	if (startWokProgram) {

		println("" + String(clockTotalMin) + "m|");// nx:"+String(timeChangeVar) );

		if (clockTotalMin < 10) { // if (clockTotalMin < timeChange(7 ,0)) {

			WaterInValveSignal(true);
			print("Filling water>");
		}
		else if (clockTotalMin < 24) {
			WaterInValveSignal(false);
			print("Spin motor");
			print("Heat>");


			motorNormalWashInterval();
			// turn on sprayer


			HeaterWaterSignal(true);

		}

		else if (clockTotalMin < 25) { // soft relays transition
			print("Spray On>");
			WaterPumpSprayerSignal(true);
			HeaterWaterSignal(false);
		}
		else if (clockTotalMin < 40) {
			print("Spin motor");
			print("Spray water>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(true);
			WaterPumpSprayerSignal(true);


		}

		else if (clockTotalMin < 55) {
			print("Spin motor");
			print("Spray's Rest>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(false);
			WaterPumpSprayerSignal(false);


		}
		else if (clockTotalMin < 56) {
			print("Spray On>");
			WaterPumpSprayerSignal(true);

		}
		else if (clockTotalMin < 73) {
			print("Spin motor");
			print("Spray water>");


			motorNormalWashInterval();
			// turn on sprayer

			WaterOutPumpSingnal(true);
			WaterPumpSprayerSignal(true);


		}
		else if (clockTotalMin < 76) {
			print("Wait time>");


			WaterPumpSprayerSignal(false);
			WaterOutPumpSingnal(false);
			motorTunOff();
		}
		else if (clockTotalMin < 90) {
			print("Push water out>");

			WaterOutPumpSingnal(true);
			HeaterWaterSignal(false);
			//motorNormalWashInterval(2, 1);
			motorNormalWashInterval(); // was by selected interval
		}
		else if (clockTotalMin < 93) {
			print("Off pumps>");

			WaterOutPumpSingnal(false);
			WaterPumpSprayerSignal(false);
			motorTunOff();
		}
		else
		{

			print("Finished!");

			startWokProgram = 0; // program end 
			motorTunOff();
			timeChangeReset();
		}


	}



	//onOff(RELAY_WaterInValveSignal, meniuOption[1]);

	//onOff(RELAY_WaterOutPumpSingnal, meniuOption[2]);

	//onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3]);

	//onOff(RELAY_HeaterWaterSignal, meniuOption[4]);



	else
	{

		onOff(RELAY_HeaterWaterSignal, false);
		onOff(RELAY_WaterInValveSignal, false);
		onOff(RELAY_WaterOutPumpSingnal, false);
		onOff(RELAY_WaterPumpSprayerSignal, false);
		motorTunOff();


		clockTotalMin = 0; // reset clock if program not started


		if (timeChangeCnt != 0) // reset single time a timeChange() function to work next time 
			timeChangeReset();

	}




	//print(meniuOption[meniuIndex]);









	print("mltime:" + String((motorPulseWidthTime)));
	print("PWM:" + String(motorSpeed) + " / " + String(motorSpeedRequest));

	print("W-" + reportComponentsWork);
	// clear last lines


	print("");
	print("");

	reportComponentsWork = ""; // reset report string 
}






byte changedValueInterrupt = 0;
// Interrupts functions
void MotorSpeedControlFuncZeroCrossDetected() {





	// triac on condition

	//if (/*changedValueInterrupt != meniuIndex &&*/  frequencyControlLoadCounter >= frequencyControlLoad) {
		/*changedValueInterrupt = meniuIndex;*/
		// power control

	if (motorSpeed > 0) {


		delayMicroseconds(motorPulseWidthTime * 2); //only delayMicroseconds can work in interrupt functions
		digitalWrite(RELAY_TRIAC_MotorSpeedControl, HIGH);

		// Serial.println(digitalRead(triacPulse));

		delayMicroseconds(50 * 2);  //delay   uSec on output pulse to turn on triac and 50uS no trigger all the time

		// reset a signal was set in triac condition

	}
	// reset frequency counter  
	//frequencyControlLoadCounter = 0;
//}

	digitalWrite(RELAY_TRIAC_MotorSpeedControl, LOW);
	//frequencyControlLoadCounter++; // counting runtime



}



void TachoMeterSensor() {


}

void frequencyControlAdd(bool validate) {

	if (validate && (frequencyControlLoad < MAINSOCKET_Frequency)) {
		frequencyControlLoad = frequencyControlLoad + 2;
		//frequencyControlLoadCounter = 0; // reset to not delay in next cycle
	}

}


void frequencyControlSubstract(bool validate) {

	if (validate && (frequencyControlLoad > 1)) {
		frequencyControlLoad = frequencyControlLoad - 2;
		//frequencyControlLoadCounter = 0; // reset to not delay in next cycle
	}
}




void motorNormalWash() {

	motorSpeedRequest = value_M_NORMALWASHPOWER; //  readMemoryByte(MEMORY_M_NORMALWASHPOWER);

}




void motorTurnOffRequest() { // only turn off request of a motor 
	motorSpeedRequest = 0;
}


// turn off everything 
void motorTunOff() {

	motorSpeedRequest = 0;
	timerMotorWork = 0; //reset timer for turn off a motorNormalWashInterval function
	timerMotorWorkBool = false;
}

