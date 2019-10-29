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
		



	/// CLOCK 1 min
	if (((long)clock_1min + 60000UL * 1) < millis())
	{
		
		clock_1min = millis(); // reset each  60 seconds  time
		clockTotalMin+=1;
		display.clear();

		if (timerMotorWorkBool) {
			timerMotorWork += 1;

			print("ADDDDD" + String (timerMotorWork) );
			delay(4000);
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

		println(""+String (clockTotalMin)+ "m|");

		if (clockTotalMin < 2) {
			
			WaterInValveSignal(true);
			print("Filling water>");
		}
		else if (clockTotalMin < 9) {
			WaterInValveSignal(false);
			print("Spray water>");
			print("Spin motor");

			motorNormalWashInterval( 2,  1);
			// turn on sprayer
			
			WaterOutPumpSingnal(true);
			WaterPumpSprayerSignal(true);
			
		}		
		else if (clockTotalMin < 10) {
			print("Heat Water>");
			
			
			HeaterWaterSignal(true);
			WaterPumpSprayerSignal(false);
			WaterOutPumpSingnal(false);
			motorTunOff();
		}
		else if (clockTotalMin < 11) {
			print("Push water out>");
			
			WaterOutPumpSingnal(true);
			HeaterWaterSignal(false);
			motorNormalWashInterval(2, 1);
		}
		else if (clockTotalMin < 15) {
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


// Spin a motor by time interval
void motorNormalWashInterval(byte timeOn, byte timeOff) {
	

		timerMotorWorkBool = true;
		

		if (timerMotorWork < timeOn ) {
			motorNormalWash();
			print("M@ wash " + String(timerMotorWork));
		}
		else if (timerMotorWork < timeOn + timeOff ) {
			motorTurnOffRequest();
			print("M@ off " + String(timerMotorWork));
		}
		else {
			timerMotorWork = 0; // set to reapet
			
		}


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





