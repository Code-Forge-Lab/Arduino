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




// buttons
#define BUTTON_DOWN 5
#define BUTTON_SET 6
#define BUTTON_UP 4
//  Swithing signals relays
#define RELAY_To_a_Pump_Output 9
#define RELAY_To_a_Solenoid_Valve_Output 8

// sensors
#define SENSOR_Well_System_Minimum_Water_Sensor 12
#define SENSOR_Exeption_Source_Vin 11   // Read Heater On condition , also source a own power to a board of Uno
#define SENSOR_Button_External 10 // power out 
#define SENSOR_WATER_PREASURE A3
#define SENSOR_WATER_FLOW A2

// Interrupts
const int   INTERRUPT_SignalRight = 2; // Rotary Encoder 0
const int   INTERRUPT_SignalLeft = 3; // Rotary Encoder 1



// Variables
// to memory
byte var_Mode = 1; //0 = auto (select well or city water),1 = city water only, 2 = well water only.
byte var_Water_Preasure_Minimum ;  // minimum of water preasure to turn on a water source unit
byte var_Water_Preasure_Maximum ;  // Maximum of water preasure to turn on a water source unit
byte var_Water_Flow_Sensor_Minimum ; //  minimum of water flow to turn on a water source unit
byte var_Allow_External_Button = 0; // react to external button
byte var_Allow_Exeption_Source_Vin; // react to when  heater  is on to turn on a water source unit

byte var_TurnOnDelaySec; // how much seconds wait (on) unti values are reached for a well or city water relays


// casual





//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_manual = 0;
unsigned long clock_1sec = 0;
unsigned long clock_1mlsec = 0;
// timer variabls
byte timerMotorWork = 0; // timer for reapeting 
bool timerMotorWorkBool = false;
//
bool button_SET_PRESSED = false;
//
bool manualReapetEach1sec = false; // allow to print at 1 second rate on the screen
bool allowPrintWhenRightButton = false; // if right button was pressed then allow to show extra menu 
bool isWaterTurnedOn = false; // when reached minimu water preasure, this condition allow to wait until get maximum preasure.

byte WaterSourcePreasureRequest = 0;
byte WaterSourcePreasure = 0;

byte meniuIndex = 0;
byte meniuValue = 0;

//memory temporery stored values
//byte value_Water_Preasure_Minimum;
//byte value_Water_Preasure_Maximum;
//byte value_Water_Flow_Sensor_Minimum;
//byte value_Allow_External_Button;
byte value_SourceCityWaterTimeout;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
byte value_SourceWellWaterTimeout;   // Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
byte value_FlowWaterOwerworkTimer = 20; // protection against a unpumped water that in rezult doesnt build a preasure and a any flow rate 
bool bool_FlowWaterOverwork = false;  // protection against a unpumped water that in rezult doesnt build a preasure and a any flow rate( condition trigering timeout protetion) 


//raw sensor rezult 
bool raw_SENSOR_Well_System_Minimum_Water_Sensor;
bool raw_SENSOR_Exeption_Source_Vin;
bool raw_SENSOR_Button_External;
int_fast16_t raw_SENSOR_WATER_PREASURE;
int_fast16_t raw_SENSOR_WATER_FLOW; // but option to read analog

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

	//menu.IncludeFunction(&func0, var_Mode, "Do Modes", "mode" , false);
	menu.IncludeFunction(&func1, var_Water_Preasure_Minimum, "Water pressure minimum","psi",false);
	menu.IncludeFunction(&func2, var_Water_Preasure_Maximum, "Water pressure maximum","psi",false);
	menu.IncludeFunction(&func3, var_Water_Flow_Sensor_Minimum, "Flow minimum rate","u/sec");
	menu.IncludeFunction(&func4, var_Allow_External_Button, "Allow External Button", "");
	menu.IncludeFunction(&func4, var_Allow_Exeption_Source_Vin, "Exeption Source Vin","");
	menu.IncludeFunction(&func5, var_TurnOnDelaySec, "Delay On Water", "sec");

	menu.IncludeQuckAccessFunction(&func0, var_Mode, "Modes","",false);
	menu.IncludeFunctionSetDefault(&userSetDefault); 
	
	menu.initiate();

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
	pinMode(SENSOR_Button_External, INPUT);
	
	pinMode(LED_BUILTIN, OUTPUT);// chost glow of led bug
	digitalWrite(LED_BUILTIN, LOW);
	//pinMode(2, INPUT);
	//attachInterrupt(digitalPinToInterrupt(INTERRUPT_SignalLeft), INTSignalLeft, FALLING);

	//attachInterrupt(digitalPinToInterrupt(INTERRUPT_SignalRight), INTSignalRight, FALLING);




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

	  raw_SENSOR_Well_System_Minimum_Water_Sensor = digitalRead(SENSOR_Well_System_Minimum_Water_Sensor);
	  raw_SENSOR_Exeption_Source_Vin = digitalRead(SENSOR_Exeption_Source_Vin);
	  raw_SENSOR_Button_External = digitalRead(SENSOR_Button_External);
	  raw_SENSOR_WATER_PREASURE = analogRead (SENSOR_WATER_PREASURE) ;
	  raw_SENSOR_WATER_FLOW = analogRead(SENSOR_WATER_FLOW);


	  /*
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

	

*/

	//                      16MHz	 32MHz
	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed 
	if (((long)clock_1min + 60000UL) < millis()) // 120000UL a double clock speed by
	{

		clock_1min = millis(); // reset each  60 seconds  time
		display.clear();

	}

	

	/// CLOCK 1 sec
	if (((long)clock_1sec + 1000L) < millis())
	{

		clock_1sec = millis(); // reset each  1 seconds  time
		manualReapetEach1sec = true;
		
		if (buttonUP) // if button right was pressed
			allowPrintWhenRightButton = !allowPrintWhenRightButton; // enter to extra menu state

		// timeouts
		if (value_SourceWellWaterTimeout > 0)
		value_SourceWellWaterTimeout = value_SourceWellWaterTimeout - 1; // timeout countdown
		
		if (value_SourceCityWaterTimeout > 0)
		value_SourceCityWaterTimeout = value_SourceCityWaterTimeout - 1; // timeout countdown
		//

		//overwork/emty load protetion counter 
		if (var_Water_Flow_Sensor_Minimum > Sensor_WaterFlowPerTimeSaved && value_FlowWaterOwerworkTimer > 0) // if even flow rate is less then expeted then do countdown protection
			value_FlowWaterOwerworkTimer = value_FlowWaterOwerworkTimer - 1;
		

		//programe variables 
		Sensor_WaterFlowPerTimeSaved = Sensor_WaterFlowTime; // save progress value in here for another time 
		Sensor_WaterFlowTime = 0; // then progress value will be returned to new start
	}

	
	if (((long)clock_manual + 1000L) < millis())
	{

		clock_manual = millis(); // reset each  1 seconds  time
		

	}





	SensorFun_WaterFlowPerSec();// scan flow sensor signals rate 



	//print(meniuOption[meniuIndex]);

	if (!menu.InterfaceDinamic()) {
		//menu.displayButtonsValue();


		//print("Minimum Well Water " + String(raw_SENSOR_Well_System_Minimum_Water_Sensor));
		//print("Exeption_Source_Vin " + String(raw_SENSOR_Exeption_Source_Vin) );
		//print("Button_External " + String(raw_SENSOR_Button_External) );
		//print("WATER_PREASURE " + String (raw_SENSOR_WATER_PREASURE));
		//print("WATER_FLOW " + String(raw_SENSOR_WATER_FLOW));


/*

		}

	*/

		printeach_1secWhenButtonSet("Tekme:" + String(Sensor_WaterFlowPerTimeSaved) +  " (" + String(raw_SENSOR_WATER_FLOW) + ")");

		if (var_Water_Flow_Sensor_Minimum < Sensor_WaterFlowPerTimeSaved) // or flow of water is greater then turn on a water!
		{
			isWaterTurnedOn = true;
			value_FlowWaterOwerworkTimer = 20;  // if flow exist, reset protection timer
			printeach_1secWhenButtonNotSet("FLow ON");
		}
		else
		{
			isWaterTurnedOn = false;
		}


		if (raw_SENSOR_WATER_PREASURE > 50) {// if greater pressure as usual ( 99 of 1024) when do filling water process 

			printeach_1secWhenButtonSet("Slegis:" + String(raw_SENSOR_WATER_PREASURE));

			// cach a preasure condition then 
			if (!isWaterTurnedOn && raw_SENSOR_WATER_PREASURE <= map(var_Water_Preasure_Minimum, 0, 255, 0, 1023)) // map(var_Water_Preasure_Minimum, 0, 255, 0, 1023)
			{
				printeach_1secWhenButtonNotSet("Preasure On");
				isWaterTurnedOn = true;
			}
			else if (/*isWaterTurnedOn &&*/ raw_SENSOR_WATER_PREASURE >= map(var_Water_Preasure_Maximum, 0, 255, 0, 1023)) // map(var_Water_Preasure_Maximum, 0, 255, 0, 1023)
			{
				printeach_1secWhenButtonNotSet("Preasure Off");
				isWaterTurnedOn = false;
			}

		}
		else {
			printeach_1secWhenButtonSet("Nera Slegio sensoriaus!");
			isWaterTurnedOn = false;
		}








		printeach_1secWhenButtonNotSet("Hey am here! " + String (raw_SENSOR_WATER_PREASURE) + "[ " + String (isWaterTurnedOn) + " ]");
		printeach_1secWhenButtonNotSet("No water: " + String(value_FlowWaterOwerworkTimer));
		
		

		if (value_FlowWaterOwerworkTimer == 0) // protect pump from not sucking water from the well to long.
		{
				
			isWaterTurnedOn = false;
		}


		// Everything before start working 
///////////////////////////////////////////////////////////////////////////////////////////////////
		if (isWaterTurnedOn) // condition that turn on a timers of water to turn on 
		{
			switch (var_Mode) { // by mode, give timeout for a well or city turn on.
			case 0:
				//print("Auto");
				if (raw_SENSOR_Well_System_Minimum_Water_Sensor)
				{
					value_SourceWellWaterTimeout = var_TurnOnDelaySec;
				}
				else
				{
					value_SourceCityWaterTimeout = var_TurnOnDelaySec;
				}
				break;
			case 1:
				//print("City Water");
				value_SourceCityWaterTimeout = var_TurnOnDelaySec;
				break;
			case 2:
				//print("Well Water");
				value_SourceWellWaterTimeout = var_TurnOnDelaySec;
			}
		}

		if (value_SourceCityWaterTimeout > 0) {
			printeach_1secWhenButtonNotSet("On City:" + String (value_SourceCityWaterTimeout));
			digitalWrite(RELAY_To_a_Solenoid_Valve_Output, HIGH);
		}
			else
			digitalWrite(RELAY_To_a_Solenoid_Valve_Output, LOW);


		if (value_SourceWellWaterTimeout > 0)
		{
			printeach_1secWhenButtonNotSet("On Well:"+String (value_SourceWellWaterTimeout));
			digitalWrite(RELAY_To_a_Pump_Output, HIGH);
		}
		else
			digitalWrite(RELAY_To_a_Pump_Output, LOW);

		//print("");
		//print("");
		printeach_1sec("");
		printeach_1sec("");
		printeach_1sec("");
		printeach_1sec("");
		
		// end of InterfaceDinamic()
	}
	


//-------------------------------- resets 
	manualReapetEach1sec = false; // reset after everything  was done before this statement

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