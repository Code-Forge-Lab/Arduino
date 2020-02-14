bool isMenuActive; // menu is active 
/////////////Water Flow Sencor Counter Per Seconds///////////////////////
byte Sensor_WaterFlowTime = 0;
byte Sensor_WaterFlowPerTimeSaved = 0; // save each second a value in here 
bool isWaterFlowPerTimePassed = false;; // void alway on state


void userSetDefault() {
	var_Water_Preasure_Minimum = 90;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 104;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 7; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 8; // react to external button
	var_Allow_Exeption_Source_Vin = 9; // react to when  heater  is on to turn on a water source unit
	
	var_Mode = 1; // takes city water
	var_TurnOnDelaySec = 5; //seconds, // Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	var_FlowWaterOwerworkTimer = 20; // mins not working rezult in 
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
	if (var_Water_Preasure_Maximum  <= var_Water_Preasure_Minimum ) // if minimum value is greater then hight value then reset to minimum
		var_Water_Preasure_Minimum = var_Water_Preasure_Maximum  - 1;
	
	print(String (map(var_Water_Preasure_Maximum, 0, 255, 0, 1023)) ); //  + "/" + String (value_FlowWaterOwerworkTimer) + " " + String (Sensor_WaterFlowPerTimeSaved)
	menu.menuPrintManuallyValue(map(var_Water_Preasure_Minimum,0,255,0,1023));
};
void func2() { 
	if (var_Water_Preasure_Minimum  >= var_Water_Preasure_Maximum  ) // if minimum value is greater then hight value then reset to minimum
		var_Water_Preasure_Maximum = var_Water_Preasure_Minimum + 1; // reset.
	
	print(String(map(var_Water_Preasure_Minimum, 0, 255, 0, 1023))  ); // + "/" + String (value_FlowWaterOwerworkTimer) + " " +String (Sensor_WaterFlowPerTimeSaved)
	menu.menuPrintManuallyValue(map(var_Water_Preasure_Maximum, 0, 255, 0, 1023));
};
void func3() { print("func3"); };
void func4() { if (var_Allow_External_Button <= 1) var_Allow_External_Button = 1; };
void func5() {};
void func6() { 
			  // menu.menuPrintManuallyValue(var_FlowWaterOwerworkTimer);
			   //display.setCursor(0, 45);
			   print("If not see flow water");
			   print("then need,timeout.");
			  // print("See by flow sensor");
};

void func7() {//display.println("[R5]");
	
	};

void func8() {};








void printbool(bool statement) {
	menu.print__(statement);
}


void printeach_1sec(String text) {

   if ( manualReapetEach1sec )
		menu.print__(text); 

};



void printeach_1sec(byte text) { // allow to print a only each second.

	if ( manualReapetEach1sec )
		menu.print__(text);

};


void printeach_1secWhenButtonSet(String text) { // if button right  is set then print this 

	if ( manualReapetEach1sec && allowPrintWhenRightButton )
		menu.print__(text);
}

void printeach_1secWhenButtonNotSet(String text) { // if button right  is unset then not print this 

	if (manualReapetEach1sec && !allowPrintWhenRightButton)
		menu.print__(text);
}



/////////////Water Flow Sencor Counter Per Seconds///////////////////////
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

