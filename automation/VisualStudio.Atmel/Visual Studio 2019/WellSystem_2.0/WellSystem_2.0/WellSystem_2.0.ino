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


#include "lib_meniuInterface128x64OLEDBased(SSD1306AsciiWire).h"



/*
// memory address
	// on units if selected
int_fast16_t MEMORY_Water_Preasure_Minimum = 1; // minimum of water preasure to turn on a water source unit
int_fast16_t MEMORY_Water_Preasure_Maximum = 2;  // Maximum of water preasure to turn on a water source unit
int_fast16_t MEMORY_Water_Flow_Sensor_Minimum = 3; //  minimum of water flow to turn on a water source unit
int_fast16_t MEMORY_Allow_External_Button =  4; // react to external button
int_fast16_t MEMORY_Allow_Exeption_Source_Vin =  5; // react to when  heater  is on to turn on a water source unit
int_fast16_t MEMORY_Source_Unit_Timeout =  6;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
int_fast16_t MEMORY_manualMode =  7;
*/

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
const int   INTERRUPT_SignalRight = 2; // Rotary Encoder 0
const int   INTERRUPT_SignalLeft = 3; // Rotary Encoder 1



// Variables
// to memory 
byte var_Water_Preasure_Minimum ;  // minimum of water preasure to turn on a water source unit
byte var_Water_Preasure_Maximum ;  // Maximum of water preasure to turn on a water source unit
byte var_Water_Flow_Sensor_Minimum ; //  minimum of water flow to turn on a water source unit
byte var_Allow_External_Button = 0; // react to external button
byte var_Allow_Exeption_Source_Vin; // react to when  heater  is on to turn on a water source unit
byte var_Source_Unit_Timeout;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
byte var_manualMode ; // manual pull water from the well with no conditional logic, auto does oposite
// casual


String reportComponentsWork = "";


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


bool INTERUPT_UP = false;
bool INTERUPT_DOWN = false;
byte INTERUPT_SIDE = 0;


volatile int virtualPosition = 50;
//------------------------------------------------------------------------------



// functions  
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

lib_meniuInterface128x64OLEDSSD1306AsciiWire menu(buttonUP, buttonDOWN, buttonSET);

#include "EEPROM32.h"
#include "functions.h"

byte  rotory;


//------------------------------------------------------------------------------




void setup() {
	
	// add function to menu 

	menu.IncludeFunction(&func1, var_Water_Preasure_Minimum, "Water MInimum","psi");
	menu.IncludeFunction(&func2, var_Water_Preasure_Maximum, "Water maXimum","psi");
	menu.IncludeFunction(&func3, var_Water_Flow_Sensor_Minimum, "Flow minimum","l/min");
	menu.IncludeFunction(&func4,var_Allow_External_Button, "Sensor Grab","val",false);
	menu.IncludeFunction(&func4, var_Allow_External_Button, "Farger Potato", "val", true);

	menu.IncludeQuckAccessFunction(&func7, var_manualMode, "Quic Access","psi",true);
	menu.IncludeFunctionSetDefault(&func6Default);

	menu.initiateDisplay();

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);

	pinMode(INTERRUPT_SignalLeft, INPUT);
	pinMode(INTERRUPT_SignalRight, INPUT);

	pinMode(RELAY_To_a_Pump_Output, OUTPUT);
	pinMode(RELAY_To_a_Solenoid_Valve_Output, OUTPUT);
	pinMode(SENSOR_Exeption_Source_Vin, INPUT);
	pinMode(SENSOR_WATER_FLOW, INPUT);
	pinMode(SENSOR_WATER_PREASURE, INPUT);
	pinMode(SENSOR_Well_System_Minimum_Water_Sensor, INPUT);
	//pinMode(2, INPUT);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_SignalLeft), INTSignalLeft, FALLING);

	attachInterrupt(digitalPinToInterrupt(INTERRUPT_SignalRight), INTSignalRight, FALLING);




	// set up memory default 
	if (readMemoryByte(0)!= true ) {

		print("MEMORY SET!");
		writeMemory(0, true);
		menu.defaultFunc();// call a default function to set a new values 
		menu.userSetValuesToMemory();
	};
	 
	menu.userGetValues();
};



bool setButtonPressed = false;
void loop() {


	


	display.setCursor(0, 0);
	//display.clear();

	 buttonSET = digitalRead(BUTTON_SET);
	 buttonDOWN = digitalRead(BUTTON_DOWN);
	 buttonUP = digitalRead (BUTTON_UP);

	 rotory = rotaryEncoderDirection(&buttonUP, &buttonDOWN);


	if (rotory == 1) {

		if (!button_SET_PRESSED && meniuIndex != 0)
			meniuIndex--; // select meniu index
		

	}
	else if (rotory == 2)
	{

		if (!button_SET_PRESSED)
			meniuIndex++; // select meniu index

	}

	



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

	






	



	//print(meniuOption[meniuIndex]);

	if (!menu.InterfaceDinamic()) {
		//menu.displayButtonsValue();

		//print(menu.func_stored[1].__functionName); // access a unit from menu memory 
		
		print("PRogram Are Here");
		
		
	}
	

	
	bool INTERUPT_UP = false;
	bool INTERUPT_DOWN = false;
	byte INTERUPT_SIDE = 0;
}











// interup rotary encoder not workiyng proietly at all!
void INTSignalLeft() {
	 
	//buttonDOWN = digitalRead(BUTTON_DOWN);
	//buttonUP = digitalRead(BUTTON_UP);
	/*
	
	INTERUPT_UP = true;
	if (INTERUPT_DOWN)
		INTERUPT_SIDE = 1;
	INTERUPT_DOWN = false;

	*/
	
};

// interup rotary encoder not workiyng proietly at all!
void INTSignalRight() {

	//buttonDOWN = digitalRead(BUTTON_DOWN);
	//buttonUP = digitalRead(BUTTON_UP);

	/*
	INTERUPT_DOWN = true;
	if (INTERUPT_UP)
		INTERUPT_SIDE = 2;
	INTERUPT_UP = false;
	*/
};