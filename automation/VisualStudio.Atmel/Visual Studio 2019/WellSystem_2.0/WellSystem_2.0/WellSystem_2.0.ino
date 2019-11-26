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
-Also in the home, while working to long time , try to cooldown by given time 
-Keep up with a preasure but mously react to a  any reasonable flow of water out of pump.
-Forse on a motor for some time to fill a tanks(pipes) to get desired preasure 

 


*/


// Simple I2C test for ebay 128x64 oled.
 // double a speed and delay time are reduced 2 times
//#include <avr/power.h> // library not working propietly with oled display '<Adafruit_SSD1306.h>' if placed before initiliaziniot in setup() clock_prescale_set(clock_div_1); loop

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire display;




// memory address
	// on units if selected
#define MEMORY_Water_Preasure_Minimum  1  // minimum of water preasure to turn on a water source unit
#define MEMORY_Water_Preasure_Maximum  2  // Maximum of water preasure to turn on a water source unit
#define MEMORY_Water_Flow_Sensor_Minimum 3 //  minimum of water flow to turn on a water source unit
#define MEMORY_Allow_External_Button 4 // react to external button
#define MEMORY_Allow_Exeption_Source_Vin 5 // react to when  heater  is on to turn on a water source unit
#define MEMORY_Source_Unit_Timeout 6	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs

// buttons
#define BUTTON_DOWN 5
#define BUTTON_SET 6
#define BUTTON_UP 4
//  Swithing signals relays
#define RELAY_To_a_Pump_Output 9
#define RELAY_To_a_Solenoid_Valve_Output 8

// sensors
#define SENSOR_Well_System_Minimum_Water_Sensor A1
#define SENSOR_Exeption_Source_Vin 11   // Read Heater On condition
#define SENSOR_WATER_PREASURE A3
#define SENSOR_WATER_FLOW A2

// Interrupts
#define INTERRUPT_SignalRight 3 // Rotary Encoder
#define INTERRUPT_SignalLeft 2 // Rotary Encoder
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

byte WaterSourcePreasureRequest = 0;
byte WaterSourcePreasure = 0;

byte meniuIndex = 0;
byte meniuValue = 0;

//memory temporery stored values
byte value_Water_Preasure_Minimum;
byte value_Water_Preasure_Maximum;
byte value_Water_Flow_Sensor_Minimum;
byte value_Allow_External_Button;



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

	pinMode(RELAY_To_a_Pump_Output, OUTPUT);
	pinMode(RELAY_To_a_Solenoid_Valve_Output, OUTPUT);
	pinMode(SENSOR_Exeption_Source_Vin, INPUT);
	pinMode(SENSOR_WATER_FLOW, INPUT);
	pinMode(SENSOR_WATER_PREASURE, INPUT);
	pinMode(SENSOR_Well_System_Minimum_Water_Sensor, INPUT);
	//pinMode(2, INPUT);
	attachInterrupt(INTERRUPT_SignalLeft, INTSignalLeft, FALLING);

	attachInterrupt(INTERRUPT_SignalRight, INTSignalRight, FALLING);




	// set up memory default 
	if (readMemoryByte(0) != true) {

		print("MEMORY SET!");
		writeMemory(0, true);

		/*
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
	*/
	};

};



bool setButtonPressed = false;
void loop() {

	display.setCursor(0, 0);
	//display.clear();

	 buttonUP = digitalRead(BUTTON_UP);
	 buttonSET = digitalRead(BUTTON_SET);
	 buttonDOWN = digitalRead(BUTTON_DOWN);


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
	if (((long)clock_1min + 60000UL) < millis()) // 120000UL a double clock speed by
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
		/*
		if ( > motorSpeed) {
			motorSpeed += 1;
		}
		else if (motorSpeed > 0) {
			motorSpeed -= 1;
		}

		*/
	}

	



	if (buttonSET) {


		//meniuOption[meniuIndex] = !meniuOption[meniuIndex];

		button_SET_PRESSED = !button_SET_PRESSED;

		if (!button_SET_PRESSED) // update changed option to a memory when pressing out of meniu
			updateMemoryValues();

		//meniuValue = 0; // reset 
		delay(300 * 2);
	}



	


	 // if meniu is not selected
	if (!meniuInterface()) {
		print("Hello work");
		print("Me in the wind");
		print("");
		print("");
	}
	

	//print(meniuOption[meniuIndex]);





	

	print("");
	print("");
	print("");
	print("");
	print("");
	print("");

	// clear last lines



	
}












void INTSignalLeft() {
};

void INTSignalRight() {

};