

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

void print(String txt) {
	String SPACE = "                   ";
	display.print(txt);
	display.println(SPACE);

}


void print(byte txt) {
	String SPACE = "                   ";
	display.print(txt);
	display.println(SPACE);

}

void println(String txt) {
	
	display.print(txt);

}


void printbool(bool statement) {
	display.print(statement);
}



// meniu function

bool uploadValues(byte index, byte* value, bool changeOption) {

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
		print("Not used!");
		break;
	default:
		print("no options");
		return false;
	};

	if (changeOption) // display option when it needed
		print(*value);
};



void updateMemoryValues() {

	writeMemory(MEMORY_ON_WaterOutPumpSingnal, value_ON_WaterOutPumpSingnal);
	writeMemory(MEMORY_ON_WaterInValveSignal, value_ON_WaterInValveSignal);
	writeMemory(MEMORY_ON_WaterPumpSprayerSignal, value_ON_WaterPumpSprayerSignal);
	writeMemory(MEMORY_ON_HeaterWaterSignal, value_ON_HeaterWaterSignal);
	writeMemory(MEMORY_M_NORMALWASHPOWER, value_M_NORMALWASHPOWER);
	writeMemory(MEMORY_M_NORMALWASHSPEED, value_M_NORMALWASHSPEED);
	writeMemory(MEMORY_M_RISEWASHSPOWER, value_M_RISEWASHSPOWER);
	writeMemory(MEMORY_M_RISEWASHSPEED, value_M_RISEWASHSPEED);

}

// update value from memory to local RAM memory 
void updateValuesfromMemory() {

	readMemoryBool(MEMORY_ON_WaterOutPumpSingnal, &value_ON_WaterOutPumpSingnal);
	readMemoryBool(MEMORY_ON_WaterInValveSignal, &value_ON_WaterInValveSignal);
	readMemoryBool(MEMORY_ON_WaterPumpSprayerSignal, &value_ON_WaterPumpSprayerSignal);
	readMemoryBool(MEMORY_ON_HeaterWaterSignal, &value_ON_HeaterWaterSignal);
	readMemoryByte(MEMORY_M_NORMALWASHPOWER, &value_M_NORMALWASHPOWER);
	readMemoryByte(MEMORY_M_NORMALWASHSPEED, &value_M_NORMALWASHSPEED);
	readMemoryByte(MEMORY_M_RISEWASHSPOWER, &value_M_RISEWASHSPOWER);
	readMemoryByte(MEMORY_M_RISEWASHSPEED, &value_M_RISEWASHSPEED);

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



void WaterInValveSignal(bool on) {
	//bool value = readMemoryBool(MEMORY_ON_WaterInValveSignal);
	onOff(RELAY_WaterInValveSignal, value_ON_WaterInValveSignal, on);
	//if (value_ON_WaterInValveSignal && on)
		reportComponentsWork += ",in:" + String(value_ON_WaterInValveSignal && on);
}


void WaterOutPumpSingnal(bool on) {
	//bool value = readMemoryBool(MEMORY_ON_WaterOutPumpSingnal);
	onOff(RELAY_WaterOutPumpSingnal, value_ON_WaterOutPumpSingnal, on);
	
	//if (value_ON_WaterOutPumpSingnal && on)
		reportComponentsWork += ",out:" + String(value_ON_WaterOutPumpSingnal && on);
}


void WaterPumpSprayerSignal(bool on) {
	//bool value = readMemoryBool(MEMORY_ON_WaterPumpSprayerSignal);
	onOff(RELAY_WaterPumpSprayerSignal, value_ON_WaterPumpSprayerSignal, on);
	//if (value_ON_WaterPumpSprayerSignal && on) 
		reportComponentsWork += ",spr:" + String(value_ON_WaterPumpSprayerSignal && on);
	
}


void HeaterWaterSignal(bool on) {
	//bool value = readMemoryBool(MEMORY_ON_HeaterWaterSignal);
	onOff(RELAY_HeaterWaterSignal, value_ON_HeaterWaterSignal, on);
	//if (value_ON_HeaterWaterSignal && on)
		reportComponentsWork += ",heatr:" + String(value_ON_HeaterWaterSignal && on);
}


void M_NORMALWASHPOWER(bool on) {
	//onOff(RELAY_WaterOutPumpSingnal, MEMORY_ON_WaterOutPumpSingnal, on);
}


void M_NORMALWASHSPEED(bool on) {
	//onOff(RELAY_WaterOutPumpSingnal, MEMORY_ON_WaterOutPumpSingnal, on);
}


void M_RISEWASHSPOWER(bool on) {
	//onOff(RELAY_WaterOutPumpSingnal, MEMORY_ON_WaterOutPumpSingnal, on);
}


void M_RISEWASHSPEED(bool on) {
	//onOff(RELAY_WaterOutPumpSingnal, MEMORY_ON_WaterOutPumpSingnal, on);
}


