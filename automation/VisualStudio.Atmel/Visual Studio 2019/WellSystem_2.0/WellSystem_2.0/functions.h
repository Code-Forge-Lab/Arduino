

void userSetDefault() {
	var_Water_Preasure_Minimum = 5;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 6;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 7; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 8; // react to external button
	var_Allow_Exeption_Source_Vin = 9; // react to when  heater  is on to turn on a water source unit
	
	var_Mode = 1; // takes city water
	var_TurnOnDelaySec = 5; //seconds, // Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
};


void printBigthenSmallLetters(String Bigtext, String Smalltext) {

	display.set2X();
	print(Bigtext);
	display.set1X();
	print(Smalltext);

}

void func0() { 

	if (var_Mode > 2)
		var_Mode = 2;

	display.set2X();
	switch (var_Mode) {
	case 0:
		print("Auto");
		break; 
	case 1:
		print("City Water");
		break; 
	case 2:
		print("Well Water");
	}
	display.set1X();
}

void func1() {
	menu.menuQuckAccesPrintManuallyValue(map(var_Water_Preasure_Minimum,0,255,0,1023));
};
void func2() { 
	menu.menuQuckAccesPrintManuallyValue(map(var_Water_Preasure_Maximum, 0, 255, 0, 1023));
};
void func3() { print("func3"); };
void func4() { print("4Aliejaus Reik"); display.set2X(); print("DABAR"); display.set1X(); };
void func5() {};
void func6() {};

void func7() {//display.println("[R5]");
	
	};

void func8() {};








void printbool(bool statement) {
	display.print(statement);
}


void printeach_1sec(String text) {

   if (manualReapetEach1sec)
		print(text); 

};



void printeach_1sec(byte text) { // allow to print a only each second.

	if (manualReapetEach1sec)
		print(text);

};


void printeach_1secWhenButtonSet(String text) { // if button right  is set then print this 

	if (manualReapetEach1sec && allowPrintWhenRightButton)
		print(text);
}

void printeach_1secWhenButtonNotSet(String text) { // if button right  is unset then not print this 

	if (manualReapetEach1sec && !allowPrintWhenRightButton)
		print(text);
}

byte Sensor_WaterFlowTime = 0;
byte Sensor_WaterFlowPerTimeSaved = 0; // save each second a value in here 
bool isWaterFlowPerTimePassed = false;; // void alway on state
void SensorFun_WaterFlowPerSec() {

	if (raw_SENSOR_WATER_FLOW > 80 && !isWaterFlowPerTimePassed)
	{
		Sensor_WaterFlowTime++;
		isWaterFlowPerTimePassed = true;
	}
	else if (raw_SENSOR_WATER_FLOW < 80) {
		isWaterFlowPerTimePassed = false;
	}
}

