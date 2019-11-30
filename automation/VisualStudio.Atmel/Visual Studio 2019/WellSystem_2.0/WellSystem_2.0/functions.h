


void func1() { print("func1"); };
void func2() { print("func2"); };
void func3() { print("func3"); };
void func4() { print("4Aliejaus Reik"); display.set2X(); print("DABAR"); display.set1X(); };
void func5() {};
void func6() {};
void func7() {};
void func8() {};


/*
void userSetValuesToMemory() {
	writeMemory(MEMORY_Water_Preasure_Minimum, var_Water_Preasure_Minimum);  // minimum of water preasure to turn on a water source unit
	writeMemory(MEMORY_Water_Preasure_Maximum, var_Water_Preasure_Maximum);  // Maximum of water preasure to turn on a water source unit
	writeMemory(MEMORY_Water_Flow_Sensor_Minimum, var_Water_Flow_Sensor_Minimum); //  minimum of water flow to turn on a water source unit
	writeMemory(MEMORY_Allow_External_Button, var_Allow_External_Button); // react to external button
	writeMemory(MEMORY_Allow_Exeption_Source_Vin, var_Allow_Exeption_Source_Vin); // react to when  heater  is on to turn on a water source unit
	writeMemory(MEMORY_Source_Unit_Timeout, var_Source_Unit_Timeout);	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	writeMemory(MEMORY_manualMode, var_manualMode);


};




void userGetValues() {

	readMemoryByte(MEMORY_Water_Preasure_Minimum, &var_Water_Preasure_Minimum);  // minimum of water preasure to turn on a water source unit
	readMemoryByte(MEMORY_Water_Preasure_Maximum, &var_Water_Preasure_Maximum);  // Maximum of water preasure to turn on a water source unit
	readMemoryByte(MEMORY_Water_Flow_Sensor_Minimum, &var_Water_Flow_Sensor_Minimum); //  minimum of water flow to turn on a water source unit
	readMemoryByte(MEMORY_Allow_External_Button, &var_Allow_External_Button); // react to external button
	readMemoryByte(MEMORY_Allow_Exeption_Source_Vin, &var_Allow_Exeption_Source_Vin); // react to when  heater  is on to turn on a water source unit
	readMemoryByte(MEMORY_Source_Unit_Timeout, &var_Source_Unit_Timeout);	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	readMemoryByte(MEMORY_manualMode, &var_manualMode);

};
*/



void userSetDefault() {
	var_Water_Preasure_Minimum = 5;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 5;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 5; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 5; // react to external button
	var_Allow_Exeption_Source_Vin = 5; // react to when  heater  is on to turn on a water source unit
	var_Source_Unit_Timeout = 5;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	var_manualMode = 6;
	menu.userSetValuesToMemory();

};

void onOff(int index, bool enable, bool manual_enable = true) {

	if (enable && manual_enable) {
		analogWrite(index, 255);
		//printLn("on:",index);
		//delay(sleep);
	}
	else {
		analogWrite(index, 0);
		//printLn("off:",index);
		//delay(sleep);
	}
}




/*
void printLn(String text, int value) {
	display.print(text);


	// lane are changed
	int ln[8] = {};

	//  delete changed letters
	if (ln[display.row()] != value)
	{
		ln[display.row()] = value;

		display.clearField(display.col(), display.row(), byte(String(value).length() + 2));
		display.println(value);
	}
	else
		display.println(value);
}
*/

void println(String txt) {
	
	display.print(txt);

}


void printbool(bool statement) {
	display.print(statement);
}



// meniu function

bool uploadValues(byte index, byte* value, bool changeOption) {
/*
	println(String(index) + ".");




	switch (index)
	{

	case 0:
		println("Start Program:");
		print(*value);

		if (changeOption) {
			startWokProgram = *value;
		}
		else // always read until user request a changes
		{
			*value = startWokProgram;
		}
		break;
	case 1:
		println("Fill water:");
		print(*value);

		if (*value < 2 && changeOption) {
			
			value_ON_WaterInValveSignal = *value;
		}
		else // always read until user request a changes
		{
			*value = value_ON_WaterInValveSignal;
		}

		break;
	case 2:
		println("Out water:");
		print(*value);

		if (*value < 2 && changeOption) {
			
			value_ON_WaterOutPumpSingnal = *value;
		}
		else // always read until user request a changes
		{
			*value = value_ON_WaterOutPumpSingnal;
		}

		break;
	case 3:
		println("Sprayer:");
		print(*value);

		if (*value < 2 && changeOption) {
			
			value_ON_WaterPumpSprayerSignal = *value;
		}
		else	// always read until user request a changes
		{
			*value = value_ON_WaterPumpSprayerSignal;
		}

		break;
	case 4:
		println("Heater:");
		print(*value);

		if (*value < 2 && changeOption) {
			value_ON_HeaterWaterSignal = *value;
		}
		else // always read until user request a changes
		{
			*value = value_ON_HeaterWaterSignal;
		}

		break;
	case 5:
		println("M.Normal power:");
		print(*value);

		if (changeOption) {
			value_M_NORMALWASHPOWER = *value;
		}
		else // always read until user request a changes
		{
			*value = value_M_NORMALWASHPOWER;
		}

		break;
	case 6:
		println("M.Normal speed:");
		print(*value);

		if (changeOption) {
			value_M_NORMALWASHSPEED = *value;
		}
		else // always read until user request a changes
		{
			*value = value_M_NORMALWASHSPEED;
		}

		break;
	case 7:
		println("M.Rise power:");
		print(*value);


		if (changeOption) {
			value_M_RISEWASHSPOWER = *value;
		}
		else// always read until user request a changes
		{
			*value = value_M_RISEWASHSPOWER;
		}

		break;
	case 8:
		println("M.Rise speed:");
		print(*value);


		if (changeOption) {
			value_M_RISEWASHSPEED = *value;
		}
		else // always read until user request a changes
		{
			*value = value_M_RISEWASHSPEED;
		}
		
		break;
	case 9:
		print("MotorWashIntervalON"); // motorNormalWashInterval
		

		if (changeOption) {
			value_M_NORMALWASHINTERVALON = *value;
		}
		else// always read until user request a changes
		{
			*value = value_M_NORMALWASHINTERVALON;
		}

		if (value_M_NORMALWASHINTERVALON == 0) // reset to protection mode
			value_M_NORMALWASHINTERVALON = 1;
		break;
	case 10:
		print("MotorWashIntervalOFF"); // motorNormalWashInterval
		if (changeOption) {
			value_M_NORMALWASHINTERVALOFF = *value;
		}
		else// always read until user request a changes
		{
			*value = value_M_NORMALWASHINTERVALOFF;
		}

		if (value_M_NORMALWASHINTERVALOFF == 0) // reset to protection mode
			value_M_NORMALWASHINTERVALOFF = 1;
		break;
	case 11:
		print("Not used!");
		break;

	default:
		print("no options");
		return false;
	};

	if (changeOption) // display option when it needed
		print(*value);

		*/
};



void updateMemoryValues() {
	/*
	writeMemory(MEMORY_ON_WaterOutPumpSingnal, value_ON_WaterOutPumpSingnal);
	writeMemory(MEMORY_ON_WaterInValveSignal, value_ON_WaterInValveSignal);
	writeMemory(MEMORY_ON_WaterPumpSprayerSignal, value_ON_WaterPumpSprayerSignal);
	writeMemory(MEMORY_ON_HeaterWaterSignal, value_ON_HeaterWaterSignal);
	writeMemory(MEMORY_M_NORMALWASHPOWER, value_M_NORMALWASHPOWER);
	writeMemory(MEMORY_M_NORMALWASHSPEED, value_M_NORMALWASHSPEED);
	writeMemory(MEMORY_M_RISEWASHSPOWER, value_M_RISEWASHSPOWER);
	writeMemory(MEMORY_M_RISEWASHSPEED, value_M_RISEWASHSPEED);
	writeMemory(MEMORY_M_NORMALWASHINTERVALON , value_M_NORMALWASHINTERVALON);
	writeMemory(MEMORY_M_NORMALWASHINTERVALOFF, value_M_NORMALWASHINTERVALOFF);

	*/

}

// update value from memory to local RAM memory 
void updateValuesfromMemory() {
	/*
	readMemoryBool(MEMORY_ON_WaterOutPumpSingnal, &value_ON_WaterOutPumpSingnal);
	readMemoryBool(MEMORY_ON_WaterInValveSignal, &value_ON_WaterInValveSignal);
	readMemoryBool(MEMORY_ON_WaterPumpSprayerSignal, &value_ON_WaterPumpSprayerSignal);
	readMemoryBool(MEMORY_ON_HeaterWaterSignal, &value_ON_HeaterWaterSignal);
	readMemoryByte(MEMORY_M_NORMALWASHPOWER, &value_M_NORMALWASHPOWER);
	readMemoryByte(MEMORY_M_NORMALWASHSPEED, &value_M_NORMALWASHSPEED);
	readMemoryByte(MEMORY_M_RISEWASHSPOWER, &value_M_RISEWASHSPOWER);
	readMemoryByte(MEMORY_M_RISEWASHSPEED, &value_M_RISEWASHSPEED);
	readMemoryByte(MEMORY_M_NORMALWASHINTERVALON, &value_M_NORMALWASHINTERVALON);
	readMemoryByte(MEMORY_M_NORMALWASHINTERVALOFF, &value_M_NORMALWASHINTERVALOFF);
	*/
}

byte spinSide = 0;
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


// simplify to manualy adding next time value
int_fast16_t timeChangeVar = 0;
int_fast16_t timeChangeTb[30]; 
int_fast16_t timeChangeCnt = 0;
int_fast16_t timeChange(int_fast16_t timeToAdd , byte positional ) {
	
	// fix initiation

	if (timeChangeTb[0] == 0)
		timeChangeTb[0] = timeToAdd;


	if (clockTotalMin >= timeChangeVar) {
		timeChangeVar = timeChangeVar + timeToAdd;
		timeChangeTb[timeChangeCnt] = timeChangeVar; // save a 'was' value to the table
		timeChangeCnt++;
	}

	return timeChangeTb[positional]; // by added position return a value 
}


void timeChangeReset() {
	timeChangeVar = 0;
	// timeChangeTb[30];
	timeChangeCnt = 0;
}



