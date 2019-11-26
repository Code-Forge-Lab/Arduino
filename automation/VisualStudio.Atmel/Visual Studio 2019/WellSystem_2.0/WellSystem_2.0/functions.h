

// functions  
bool buttonUP;
bool buttonSET;
bool buttonDOWN;
  
 


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





//------------------------------------------------- Meniu Starts-----------------------------



bool boolSetButton = false;
bool boolSetMenu = false;
bool boolQuicklyChange = false;
byte meniuOptionSelected = 1;
byte meniuOptionsLenght = 8; // how much meniu option in meniu
bool meniuOptionIsPressing = false;// detect when button no longer is press
bool meniuOptionIsSelected = false; // When eventualy are at P0-P3 option then time to select what specificly are changing a values that are included in the statement.
bool meniuOptionWhenSelected = false;// for clearing one time a meniu option are change
unsigned long startedWaiting;
unsigned long startedWaitingmeniuOptionSelected;
byte toggleDisplay = 0;
String Cool = "Cool";
String Heat = "Heat";




void meniuDescribeOptionDisplay(String txt) {
	display.setCursor(0, 9);
	/*
	display.setTextSize(1);
	display.println(txt);
	*/

	print(txt);
}


//Print big numbers in screen // compiler do not construct correctly giving wrong function
void txtBigNumber(float value, String valueType) {
	display.setCursor(0, 29);
	// set text size auto
	//(String(value).length() <= 5 ? display.setTextSize(4) : display.setTextSize(3));

	//display.setTextColor(WHITE);
	//print(subLargeText(String(value)));
	//(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	println(valueType /*+ String(value).length()*/);
};
//Print big numbers in screen // compiler do not construct correctly giving wrong function
void txtBigNumber(byte value, String valueType) {
	display.setCursor(0, 29);
	// set text size auto
	//(String(value).length() <= 5 ? display.setTextSize(4) : display.setTextSize(3));

	//display.setTextColor(WHITE);
	print(value);
	//(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	println(valueType);
};

void txtBigNumber(String value, String valueType) {
	display.setCursor(0, 29);
	//display.setTextSize(4);

	//display.setTextColor(WHITE);
	print(value);
	//	(String(value).length() <= 5 ? display.setTextSize(2) : display.setTextSize(1));
	println(valueType);
};


String subLargeText(String value) {

	return (value.length() > 6) ? (value.substring(0, value.length() - 2)) : (value.substring(0, value.length() - 1));
};


byte meniuOptionSelectFun() {


	// if pressing a button and meniu where are selected are not to big then...
	if ((buttonUP && meniuOptionSelected < meniuOptionsLenght) && meniuOptionIsPressing && !meniuOptionIsSelected) {

		++meniuOptionSelected;
		startedWaitingmeniuOptionSelected = millis();// time out start point


	}
	// if pressing a button and meniu where are selected are not to small then...
	if ((buttonDOWN && meniuOptionSelected > 1) && meniuOptionIsPressing && !meniuOptionIsSelected) {

		--meniuOptionSelected;
		startedWaitingmeniuOptionSelected = millis();// time out start point

	}

	// Give new timeout and starting point for menu fuctions to work
	if (buttonSET && !meniuOptionIsSelected) {
		//meniuOptionIsPressing = true;
		startedWaitingmeniuOptionSelected = millis();// time out start point
	}

	// if user do not do for a while , accures a time out.
	if ((millis() > (long)(startedWaitingmeniuOptionSelected + 25000UL)) && !meniuOptionIsSelected)
	{
		boolSetButton = false; // exit from boolSetButton statement
		boolSetMenu = false; // exit from boolSetMenu statement
		meniuOptionSelected = 1; //reset meniu position to P0
	}

	// capture when button is pressing and set a one time button push
	if (buttonUP || buttonDOWN) {
		meniuOptionIsPressing = false; //While pressing ignore incrementing a meniuOptionSelected
	}
	else
	{
		meniuOptionIsPressing = true; //but when no longer is pressing set to default 
	}

	return meniuOptionSelected;
}



// SPECIAL BUGGY FUNCTION! WANA BE ON TOP
// user when changin something in meniu options, doest trow here 
void userChangeMeniuValue(byte* userPx, byte __delay = 10, byte minValue = 1, byte maxValue = 255) {

	// incrament
	if (buttonUP && *userPx < maxValue)
	{
		*userPx = *userPx + 1;

		delay(__delay);
		//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
	}

	// substract 
	if (buttonDOWN && *userPx > minValue)
	{
		*userPx = *userPx - 1;
		delay(__delay);
		//startedWaiting = millis();  //reset meniu delay counter // LAG When  push SET Button
	}

}



bool meniuInterface() {

	

	if (buttonSET || boolSetButton) {
		boolSetButton = true;

		// Set menu for more options
		/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
		if (buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
			// set loop from boolean


			// exit from tight loops!
			// event trigger if after some time will react to button set
			// reset whole set loop
			//                                              Extra timer for not hit a first option
			if ((millis() > (long)(startedWaiting + 5000UL + 1550UL)) && buttonSET) {

				// After meniu option was selected 
				if (meniuOptionIsSelected) {
					//userSetValuesToMemory();
				}
				//  if meniu option selected and need change a values to EEPROM
				meniuOptionIsSelected = !meniuOptionIsSelected;

				// Exit from progra  and main boolSetButton loop...if meniu not selected
				if (meniuOptionSelectFun() == meniuOptionsLenght) { // Exit

					boolSetButton = false; // exit from boolSetButton statement
					boolSetMenu = false; // exit from boolSetMenu statement
					meniuOptionSelected = 1; //reset meniu position to P0
					meniuOptionIsSelected = false; // when user exit from meniu set to default 
				}
				//userSetValuesToMemory();
				delay(111);
			}
			else
			{
				// set whole set loop by default if no buttons event accures
				boolSetMenu = true;
				boolSetButton = true;

			}

			//progra-------------------------------------------------------
			if ((buttonSET || boolSetMenu)) {

				//display.clear();

				display.setCursor(0, 0);
				display.set1X();
				if (meniuOptionSelectFun() < meniuOptionsLenght) // only display meniu option values exept on 'EXIT'
					print("[P" + String(meniuOptionSelectFun() - 1) + "]");

				// if meniu option not selected print about meniu abbreviation
					
				if (meniuOptionWhenSelected) {
					//print("-");
					//print("");
				}
				// if not meniu option selected and meniu are not at EXIT point
				if (!meniuOptionIsSelected && meniuOptionSelectFun() != meniuOptionsLenght) { 
					meniuOptionWhenSelected = true;
				
					
					print("      MENIU");//display.println("MENIU"); 
					
				}
				else {
					
					meniuOptionWhenSelected = false;
				}

				

				if (meniuOptionSelectFun() == 1) { // P0 C-H
					//display.println("[P0]");
					meniuDescribeOptionDisplay("<Mode:" + Cool + "," + Heat);

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP0, 1, 0);

						if (0/*userP0 == 0*/) {
							// -C-
							//txtBigNumber("C", "ooling");
							//boolUserOffsetTemperatureRange = false; // reset after chaning performance mode to work first time
						}
						else {
							// -H-
							txtBigNumber("H", "eating");
							//boolUserOffsetTemperatureRange = false; // reset after chaning performance mode to work first time
						}

					}

				}
				else if (meniuOptionSelectFun() == 2) { // P1 Temp Logic offset 0.5
					//display.println("[P1]");
					meniuDescribeOptionDisplay("<Logic Temp. Offset>");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP1);
						// Display
						//txtBigNumber(userP2FixLogicRangeOffse(), "C");
					}

				}
				else if (meniuOptionSelectFun() == 3) { // Not included
					//display.println("[P2]");
					meniuDescribeOptionDisplay("Frequent on timeout ");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP2, 10, 0);
						// Display
						//txtBigNumber(userP2, "min");
					}

				}

				else if (meniuOptionSelectFun() == 4) { // P3 Timer count while relay is on, untit set a limit are reached.
					//display.println("[P3]");
					meniuDescribeOptionDisplay("<Overworking timeout>");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP3);
						// Display
						//txtBigNumber(3, "min");
					}

				}

				else if (meniuOptionSelectFun() == 5) { // P4 Timer to off relay when are in cooldown mode
					//display.println("[P4]");
					meniuDescribeOptionDisplay("<Overworking " + Cool);

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						// Change values interface
						//userChangeMeniuValue(&userP4);
						// Display
						//txtBigNumber(4, "min");
					}

				}

				else if (meniuOptionSelectFun() == 6) { // Reset to default
					//display.println("[R5]");
					meniuDescribeOptionDisplay("<Reset to default>");
					// do EEPROM changes
					if (meniuOptionIsSelected) {
						display.println("Press up to reset");

						if (buttonUP)
						{
							//display.setTextSize(4);
							println(" Done");
							//display.display();
							// reset to default
							//userSetDefault();
							//userSetValuesToMemory();


							delay(2000);
							meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
						}
					}
				}

				else if (meniuOptionSelectFun() == 7) {
					/*
					//display.println("[R6]"); energyOffMin energyOffMin
					meniuDescribeOptionDisplay("<Reset energy wasted>");
					// do EEPROM changes
					if (meniuOptionIsSelected)
					{
						display.println("Press up to reset");

						if (buttonUP)
						{
							display.setTextSize(4);
							display.println(" Done");
							display.display();
							// reset to default
							userP6EnergyOffMin = 0;
							userP6EnergyOnMin = 0;

							writeEEPROM32(MEMORY_P6OFF,0);
							delay(1);
							writeEEPROM32(MEMORY_P6ON,0);

							delay(2000);
							meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
						}
					}
					*/

					// too much memory usage
					/*

							meniuDescribeOptionDisplay("Power Usage Per Day");
						//  power usage per day ratio
						if (meniuOptionIsSelected)
						{

							display.println(String( int(powerUsageMin * float(userP6EnergyOnMin)  / ((userP6EnergyOffMin / 60) / 24)) ) + "w");
						}
					*/


				}



				else if (meniuOptionSelectFun() == meniuOptionsLenght) { // Exit
				display.setCursor(0, 0);
					
				display.set2X();
					print("");
					print("  [EXIT]");
					
					if (meniuOptionIsSelected) {
						
						boolSetMenu = false;
						boolQuicklyChange = false;
						display.clear();
					}
				
					
				}

				

			}


		}





		// Quickly change option
		else if (buttonSET == LOW || boolQuicklyChange) {
			// set loop from boolean



			// exit from tight loops!
			// event trigger if after some time will react to button set
			if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 230UL)) && buttonSET || /*or timeout for 45s*/(millis() > (long)(startedWaiting + 45100UL))) {
				// reset whole set loop
				boolQuicklyChange = false;
				boolSetButton = false;
				//userSetValuesToMemory(); // write temperature changes to memory
				delay(100);
			}
			else
			{
				// set whole set loop
				boolQuicklyChange = true;
				boolSetButton = true;

			}

			//progra-------------------------------------------------------
			if (boolQuicklyChange) {
				//display.setTextColor(BLACK, WHITE); // Draw 'inverse' text

			//	display.clear();



				display.println("Keisti temperatura");

				// Change values interface
				//userChangeMeniuValue(&userP5, 100);



				if (toggleDisplay > 20) // work as text blinker)
				{
					txtBigNumber(" ", " ");
					toggleDisplay = 0; //reset display flashing counter
				}
				else
					// display temperature 
					//txtBigNumber(5, "C");

				toggleDisplay++;



				//delay(4000);


			}
		}

	}
	else
	{
		startedWaiting = millis(); //by default will register values until condition
		boolQuicklyChange = false; //activate condition ones until event is trigered
		boolSetMenu = false; // set default to meniu option
		boolSetMenu = false; //activate condition ones until event is trigered
		//display.clear(); // blinking...
	}


	return boolSetMenu || boolQuicklyChange;
}


//------------------------------------------------- Meniu Ends-----------------------------