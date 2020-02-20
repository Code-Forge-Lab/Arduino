bool isMenuActive; // menu is active 
bool isMenuActiveLag; // menu is active lag with once cycle of the program
/////////////Water Flow Sencor Counter Per Seconds///////////////////////
byte Sensor_WaterFlowTime = 0;
byte Sensor_WaterFlowPerTimeSaved = 0; // save each second a value in here 
bool isWaterFlowPerTimePassed = false;; // void alway on state





int funFillingWaterOvertime(byte setNewValue = 0) {

	if (setNewValue > 0)
		var_FillingWaterOvertime = setNewValue;
	return  var_FillingWaterOvertime * 10;
}




void userSetDefault() {
	var_Water_Preasure_Minimum = 40;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 42;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 25; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 1; // 0= no react, 1=when button reset all errors, 2=when pressed-turn on water
	funFillingWaterOvertime(30);//  (multiplier)10 * 30min = 300min = 5h
	//var_Allow_Exeption_Source_Vin = 1; // react to when  heater  is on to turn on a water source unit
	
	var_Mode = 1; // takes city water
	var_TurnOnDelaySec = 5; //seconds, // Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	var_FlowWaterOwerworkTimer = 20; // mins not working rezult in 
	testingMode = 0;
};

String modeStateRezult() {

	switch (var_Mode) {
	case 0:
		return ("Auto");
		break;
	case 1:
		return ("City Water");
		break;
	case 2:
		return ("Well Water");
	}

}


void printBigthenSmallLetters(String Bigtext, String Smalltext) {

	display.set2X();
	print(Bigtext);
	display.set1X();
	print(Smalltext);

}


void errorCachingTimersIn1min() {


	//////////////////////////////// Error Level Timers///////////////////////////
		// then working too long time, counter 
	if (value_SourceCityWaterTimeout > 0 || value_SourceWellWaterTimeout > 0) { // any of this timer that turn on a water flow works very long time then 

		if (value_FillingWaterOvertime > 0)
			value_FillingWaterOvertime = value_FillingWaterOvertime - 1; // too long working time decrease each minute.

		if (value_FillingWaterOvertime == 0)
			isFillingWaterOvertime = true; // if reached a value_SourceCityWaterTimeout == 0 then rezult is equal to overwoked too long.

	}
	else if (value_FillingWaterOvertime < funFillingWaterOvertime() && !isFillingWaterOvertime)
	{//							         double rate for 
		value_FillingWaterOvertime = value_FillingWaterOvertime + 2; // increase each time when  not water is turned on 
	}

	//overwork/emty load protetion counter then no flow of water for a to long time 
	if (!isPossibleWaterTurnOn && value_FlowWaterOwerworkTimer > 0 && !isFillingWaterOvertime) // if even flow rate is less then expeted then do countdown protection
		value_FlowWaterOwerworkTimer = value_FlowWaterOwerworkTimer - 1; // isFillingWaterOvertime for see  clearly original error level only


}


String booltoText(bool cond) {

	if (cond)
		return "true";
	else
		return "false";
}


void func0() { 

	if (var_Mode > 2)
		var_Mode = 2;

	display.set2X();
	print(modeStateRezult());
	display.set1X();
}

void func1() {
	//print("When lower, always");
	//print("request water.");
	if (var_Water_Preasure_Maximum  <= var_Water_Preasure_Minimum ) // if minimum value is greater then hight value then reset to minimum
		var_Water_Preasure_Minimum = var_Water_Preasure_Maximum  - 1;
	
	print(String (map(var_Water_Preasure_Maximum, 0, 255, 0, 1023)) ); //  + "/" + String (value_FlowWaterOwerworkTimer) + " " + String (Sensor_WaterFlowPerTimeSaved)
	menu.menuPrintManuallyValue(map(var_Water_Preasure_Minimum,0,255,0,1023));
};
void func2() { 
	//print("Stop getting a water");
	//print("source");

	if (var_Water_Preasure_Minimum  >= var_Water_Preasure_Maximum  ) // if minimum value is greater then hight value then reset to minimum
		var_Water_Preasure_Maximum = var_Water_Preasure_Minimum + 1; // reset.
	
	print(String(map(var_Water_Preasure_Minimum, 0, 255, 0, 1023))  ); // + "/" + String (value_FlowWaterOwerworkTimer) + " " +String (Sensor_WaterFlowPerTimeSaved)
	menu.menuPrintManuallyValue(map(var_Water_Preasure_Maximum, 0, 255, 0, 1023));
};
void func3() {
	//print("Trigger when turn on");
	//print("water source.");
};

void func4() { 
	if (var_Allow_External_Button >= 2)
	{
		var_Allow_External_Button = 2;
	}

	if (var_Allow_External_Button == 0)
		print("Ignore.");
	else if (var_Allow_External_Button == 1)
		print("Reset errors.");
	else if (var_Allow_External_Button == 2)
		print("Turn on water.");
};
void func5()
{

	print("Filter false sensors");
	print("flickering");
};
void func6() { 
			  // menu.menuPrintManuallyValue(var_FlowWaterOwerworkTimer);
			   //display.setCursor(0, 45);
			   print("If not see flow water");
			   print("then on,timeout.");
			  // print("See by flow sensor");
};

void func7() {//display.println("[R5]");
	if (var_FillingWaterOvertime < 1)
		var_FillingWaterOvertime = 1;
	   value_FillingWaterOvertime = funFillingWaterOvertime(); // update program counter

	print("Timer when something");
	print ("take to long to fill.");
		menu.menuPrintManuallyValue(funFillingWaterOvertime());
	};

void func8() {

	print("Timers works shorter");
	if (testingMode > 1) 
		testingMode = 1;

	display.set2X();
	print(booltoText(testingMode));
	display.set1X();
	
		//menu.menuPrintManuallyValue(testingMode);
};








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


void printeach_1secWhenButtonSetPage1(String text) { // if button right  is set then print this 

	if ( manualReapetEach1sec && allowPrintWhenRightButton == 1 )
		menu.print__(text);
}

void printeach_1secWhenButtonSetPage2(String text) { // if button right  is set then print this 

	if (manualReapetEach1sec && allowPrintWhenRightButton == 2)
		menu.print__(text);
}

void printeach_1secWhenButtonNotSet(String text) { // if button right  is unset then not print this 

	if (manualReapetEach1sec && allowPrintWhenRightButton == 0)
		menu.print__(text);
}

void printlneach_1secWhenButtonNotSet(String text) { // if button right  is unset then not print this 

	if (manualReapetEach1sec && allowPrintWhenRightButton == 0)
		menu.println__(text);
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

