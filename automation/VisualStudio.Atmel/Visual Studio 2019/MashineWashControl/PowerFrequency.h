// Simple I2C test for ebay 128x64 oled.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire display;

#include "functions.h"
// overall
#define MAINSOCKET_Frequency 50 /* *  2  */ // Frequency of main power source rectified!
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
}





// Variables
byte spinSide = 0;
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long clock_1mlsec = 0;

// Frequency Control a load 
byte frequencyControlLoadCounter = 0; // each time count a progress
byte frequencyControlLoad = 30; // compare if Counter is equal to This variable value to execute selecte frequency

int_fast16_t clockTotalMin = 0;
int_fast16_t pulseWidthTime; // save stored timer for triggering a triac

//------------------------------------------------------------------------------
String Stop = " Stop";

String meniuText[] = {
	"1.Start Program" ,// "1_program"+Stop , // 1,2
	"2.Fill water", //"2_Fill"+Stop , // 3, 4
	"3.Drain water",// "3_Drain"+Stop , // 5, 6
	"4.On Sprayer", //"4_ Sprayer"+Stop, // 7, 8
	"5.On Heater", //"5_Heater"+Stop,// 9,10
};
bool  meniuOption[] = { 0,1,1,1,0 };


bool setButtonPressed = false;
void loop() {

	display.setCursor(0, 0);
	//display.clear();

	bool buttonUP = !digitalRead(BUTTON_UP);
	bool buttonSET = !digitalRead(BUTTON_SET);
	bool buttonDOWN = !digitalRead(BUTTON_DOWN);


	byte rotory = rotaryEncoderDirection(&buttonUP, &buttonDOWN);


	if (rotory == 1 && meniuIndex > 0) {

		if (!setButtonPressed)
			meniuIndex -= 1;
		else
			frequencyControlSubstract(true);

	}
	else if (rotory == 2 && 160 > meniuIndex)
	{


		if (!setButtonPressed)
			meniuIndex += 1;
		else
			frequencyControlAdd(true);

	}

	/// CLOCK 1 min
	if (((long)clock_1min + 60000UL) < millis())
	{

		clock_1min = millis(); // reset each  60 seconds  time
		clockTotalMin += 4;

	}

	/// CLOCK 1 sec
	if (((long)clock_1sec + 100UL) < millis())
	{

		clock_1sec = millis(); // reset each  1 seconds  time



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

	pulseWidthTime = map(meniuIndex, 160, 0, 150, 8990); //7200 


	//print(String (clockTotalMin) + " min");
	//print(meniuText[meniuIndex]);
	//print(String (meniuOption[meniuIndex]));



	if (buttonSET) {


		meniuOption[meniuIndex] = !meniuOption[meniuIndex];

		setButtonPressed = !setButtonPressed;

		delay(500);
	}

	/*
	if (meniuOption[0] == 1 ) {

		//if (display.availableForWrite())

		if (clockTotalMin < 1) {
			onOff(RELAY_WaterInValveSignal, meniuOption[1]);
			print("Filling water>");
		}
		else if (clockTotalMin < 2) {
			onOff(RELAY_WaterInValveSignal, meniuOption[1] , false); // turn off water in valve

			print("Spray water>");
			print("Spin motor");
			// turn on sprayer
			onOff(RELAY_WaterOutPumpSingnal, meniuOption[2]);
			onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3]);
		}
		else if (clockTotalMin < 3) {
			print("Heat Water>");
			print(""); //clear last extra line
			onOff(RELAY_HeaterWaterSignal, meniuOption[4]);
			onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3],false);
			onOff(RELAY_WaterOutPumpSingnal, meniuOption[2],false);

		}
		else if (clockTotalMin < 4) {
			print("Push water out>");
			onOff(RELAY_WaterOutPumpSingnal, meniuOption[2]);
			onOff(RELAY_HeaterWaterSignal, meniuOption[4],false);
			//onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3]);
		}
		else if (clockTotalMin < 5) {
			print("Off pumps>");
			onOff(RELAY_WaterOutPumpSingnal, meniuOption[2], false);
			onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3], false);
		}
		else
		{
			print("Finished!");
			meniuOption[0] = 0; // program end
		}


		}

			//onOff(RELAY_WaterInValveSignal, meniuOption[1]);

			//onOff(RELAY_WaterOutPumpSingnal, meniuOption[2]);

			//onOff(RELAY_WaterPumpSprayerSignal, meniuOption[3]);

			//onOff(RELAY_HeaterWaterSignal, meniuOption[4]);



	else
	{

		onOff(RELAY_HeaterWaterSignal, 0);
		onOff(RELAY_WaterInValveSignal, 0);
		onOff(RELAY_WaterOutPumpSingnal, 0);
		onOff(RELAY_WaterPumpSprayerSignal, 0);

		clockTotalMin = 0; // reset clock if program not started
	}

	*/

	print("motor:" + String(setButtonPressed));

	print(String(meniuIndex));






	if (setButtonPressed)
		print("Hz:" + String(frequencyControlLoad) + "/" + String(frequencyControlLoadCounter));
	else
		print("mltime:" + String((pulseWidthTime)));
}





byte rotaryEncoderDirection(bool* sideUp, bool* sideDown) {

	String side = "";
	if (spinSide == 0)
	{
		if (*sideUp && !*sideDown)
			spinSide = 1;

		else if (*sideDown && !*sideUp)
			spinSide = 2;
	}




	if (!*sideUp && !*sideDown)
	{
		byte xspin = spinSide;
		spinSide = 0;
		return xspin;

	}
	return 0;
}


byte changedValueInterrupt = 0;
// Interrupts functions
void MotorSpeedControlFuncZeroCrossDetected() {





	// triac on condition

	//if (/*changedValueInterrupt != meniuIndex &&*/  frequencyControlLoadCounter >= frequencyControlLoad) {
		/*changedValueInterrupt = meniuIndex;*/
		// power control

	if (meniuIndex > 0) {


		delayMicroseconds(pulseWidthTime); //only delayMicroseconds can work in interrupt functions
		digitalWrite(RELAY_TRIAC_MotorSpeedControl, HIGH);

		// Serial.println(digitalRead(triacPulse));

		delayMicroseconds(50);  //delay   uSec on output pulse to turn on triac and 50uS no trigger all the time

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


