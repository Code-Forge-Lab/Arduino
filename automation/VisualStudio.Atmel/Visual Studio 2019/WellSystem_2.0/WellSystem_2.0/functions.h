

void userSetDefault() {
	var_Water_Preasure_Minimum = 5;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 6;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 7; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 8; // react to external button
	var_Allow_Exeption_Source_Vin = 9; // react to when  heater  is on to turn on a water source unit
	var_Source_Unit_Timeout = 10;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	var_manualMode = 11;
	

};




void func1() {
	print("func1");
};
void func2() { print("func2"); };
void func3() { print("func3"); };
void func4() { print("4Aliejaus Reik"); display.set2X(); print("DABAR"); display.set1X(); };
void func5() {};

void func6Default() {
	print("HEYY DEF");
	delay(2000);
};
void func7() {//display.println("[R5]");
	print("func7"); print("func7");
	};
void func8() {};




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