

void userSetDefault() {
	 var_one = 10;
	 var_two = 22;
	 var_three = 33;
	 var_four = 44;
	 var_five = 55;
	 var_six = 66;
	
};




void func1() {	print("func1");};
void func2() { print("func2" + String(var_two)); };
void func3() { print("func3"); };
void func4() { print("four life"); display.set2X(); print("DABAR"); display.set1X(); };
void func5() { print("five"); };
void func6Default() {
	userSetDefault();
	print("HEY DEFAULT");
	delay(2000);
};






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


