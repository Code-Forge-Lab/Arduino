#pragma once


// const Varables

#define CONST_TOTALMENUELEMENTS  10 // maximum ammount of elemtents(functions) in meniu 

//Structure
#include<stdlib.h>
#include <stdio.h>
#include "EEPROM32.h"
using namespace std;



// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1


#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire display;


void initiateDisplay() {

	Wire.begin();
	Wire.setClock(800000L); // 800000L or 400000L

#if RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
	display.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

	display.setFont(Adafruit5x7);
}


String SPACE = "                        ";
void print(String txt) {
	display.print(txt);
	display.println(SPACE);

}


void print(byte txt) {
	display.print(txt);
	display.println(SPACE);

}
















// store a function in here 
struct funcBuffer {

public:	 // Only works in public , private throw error 
	// stored function
	void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
	String __functionName;// function abroviation what are doing 
	byte *__functionValueAddress;// function value that are stored value to EEPROM memory
	bool __printfunctionValue; // allow to print a numeric value of  this function
	int __funcAddress; // address of avalue that are addresed in to EEPROM 
	// Execute Selected menu option and point to responsive function



	void  DrawFunction() { //Expect Selected menu function

		fnc_(); // How Call Pointed Function ;

	};


	void IncludeFunction(void (*functionPointer)(), byte &functionValueAddress , String functionName = "Uknown" ,  bool printfunctionValue = true ) { // where magic on
		fnc_ = functionPointer;
		__functionName = functionName;
		__functionValueAddress = &functionValueAddress;
		__printfunctionValue = printfunctionValue;
	};


	//funcBuffer* next; // from iterators
	
};



//funcBuffer  func_store = funcBuffer();      // Zero initialize using default constructor
//funcBuffer func_store{};          // Latest versions accept this syntax.
//funcBuffer* func_store = new funcBuffer();  // Zero initialize a dynamically allocated object.





class lib_meniuInterface128x64OLEDSSD1306AsciiWire
{

	// old ones

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

		
	//////
	bool *buttonUP;
	bool *buttonDOWN;
	bool *buttonSET;
	byte *rotarySpinedSIDE;
	bool isRoatry = false; // detect where is a rotary encoder
	byte includedMenuCount = 1;

	//ADDRESS STORED Stored memory managment
	//void (*userSetValuesToMemory)(); // 
	//void (*userSetDefault)();
	//void (*userSetValuesToMemory)();
	// example Struck init
	//C  c;            // members are random
	//C* c = new C;    // members are random (more officially undefined).
public:
	//const int allMEenuOptions = 5;
	
	
	 
	funcBuffer func_stored[CONST_TOTALMENUELEMENTS];

	//funcBuffer *func_store; to iterators
   
public:
	// constructor
		// using a simple buttons																		 Available menius in selection
	lib_meniuInterface128x64OLEDSSD1306AsciiWire(bool &buttonUp , bool &buttonDown, bool &buttonSet) {
		buttonUP = &buttonUp; // store address of a buttons
		buttonDOWN = &buttonDown; // store address of a buttons
		buttonSET = &buttonSet; // store address of a buttons

		//func_store[5];
	}

	// if using rotary encoder 
	lib_meniuInterface128x64OLEDSSD1306AsciiWire(byte &rotarySpinedSide, bool  & buttonSet) {
		rotarySpinedSIDE = &rotarySpinedSide;
		buttonSET = &buttonSet;
		isRoatry = true;

	}
	



	// access menu by long pressed a Set button
	void IncludeFunction(void (*functionPointer)(), byte& functionValue, String functionName = "Uknown"  , bool printfunctionValue = true)
	{ // where magic on
		func_stored[includedMenuCount].IncludeFunction( (functionPointer), functionValue,  functionName , printfunctionValue);

		includedMenuCount++;
	}

	// access menu by  short pressed a Set button
	void IncludeQuckAccessFunction( void (*functionPointer)(), byte & functionValue , String functionName = "Uknown"   , bool printfunctionValue = true)
	{
		func_stored[includedMenuCount + 1].IncludeFunction((functionPointer), functionValue, functionName,   printfunctionValue );
	}

	// using iterator :https://www.youtube.com/watch?v=cAZ8CyDY56s
	/*
	void addFunction(funcBuffer** pointerTofuncBuffer, void (*functionAddress)()) {
	
		// funcBuffer * func_store = new funcBuffer();

		funcBuffer* temp = (funcBuffer*)malloc(sizeof(struct funcBuffer));
		temp->fnc_ = functionAddress;
		temp->next = NULL;
		if (*pointerTofuncBuffer != NULL) temp->next = *pointerTofuncBuffer;
		*pointerTofuncBuffer = temp;
	};

	// test 
	void printFunctions() {
		print("List");
		while (func_store != NULL) {
			 func_store->fnc_();
			func_store = func_store->next;
		}
	
	}

	void printFuncNext() {
		//print("List");
		if (func_store != NULL) {
			func_store->fnc_();
			func_store = func_store->next;
		}

	}


	void printFuncPrev() {
		//print("List");
		if (func_store != NULL) {
			func_store->fnc_();
			func_store = func_store->prev;
		}

	}
	*/





	//------------------------------------------------------------------
	void meniuDescribeOptionDisplay(String txt) {
		display.setCursor(0, 9);
		
		//display.setTextSize(1);
		//display.println(txt);
		

		print(txt);
	}


	byte meniuOptionSelectFun() {


		// if pressing a button and meniu where are selected are not to big then...
		if ((*buttonUP && meniuOptionSelected < includedMenuCount) && meniuOptionIsPressing && !meniuOptionIsSelected) {

			++meniuOptionSelected;
			startedWaitingmeniuOptionSelected = millis();// time out start point


		}
		// if pressing a button and meniu where are selected are not to small then...
		if ((*buttonDOWN && meniuOptionSelected > 1) && meniuOptionIsPressing && !meniuOptionIsSelected) {

			--meniuOptionSelected;
			startedWaitingmeniuOptionSelected = millis();// time out start point

		}

		// Give new timeout and starting point for menu fuctions to work
		if (*buttonSET && !meniuOptionIsSelected) {
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
		if (*buttonUP || *buttonDOWN) {
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
		if (*buttonUP && *userPx < maxValue)
		{
			*userPx = *userPx + 1;

			delay(__delay);
			//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
		}

		// substract 
		if (*buttonDOWN && *userPx > minValue)
		{
			*userPx = *userPx - 1;
			delay(__delay);
			//startedWaiting = millis();  //reset meniu delay counter // LAG When  push SET Button
		}

	}



	void userChangeMeniuValue(bool* userPx, byte __delay = 10) {

		// incrament
		if ((*buttonDOWN ||* buttonUP) && *userPx)
		{
			*userPx != *userPx;

			delay(__delay);
			//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
		}



	}

	bool InterfaceDinamic() {
		

		if (*buttonSET || boolSetButton) {
			boolSetButton = true;

			// Set menu for more options
			/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
			if (*buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
				// set loop from boolean


				// exit from tight loops!
				// event trigger if after some time will react to button set
				// reset whole set loop
				//                                              Extra timer for not hit a first option
				if ((millis() > (long)(startedWaiting + 5000UL + 1550UL)) && *buttonSET) {

					// After meniu option was selected 
					if (meniuOptionIsSelected) {
						userSetValuesToMemory();
					}
					//  if meniu option selected and need change a values to EEPROM
					meniuOptionIsSelected = !meniuOptionIsSelected;

					// Exit from progra  and main boolSetButton loop...if meniu not selected
					if (meniuOptionSelectFun() == includedMenuCount) { // Exit

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
				if ((*buttonSET || boolSetMenu)) {

					//display.clear();

					display.setCursor(0, 0);
					display.set1X();
					if (meniuOptionSelectFun() < includedMenuCount  ) // only display meniu option values exept on 'EXIT'
						print("[P" + String(meniuOptionSelectFun() - 1) + "]");

					// if meniu option not selected print about meniu abbreviation

					if (meniuOptionWhenSelected) {
						//print("-");
						//print("");
					}
					// if not meniu option selected and meniu are not at EXIT point
					if (!meniuOptionIsSelected && meniuOptionSelectFun() != includedMenuCount) {
						meniuOptionWhenSelected = true;

						print("      MENIU");//display.println("MENIU"); 

					}
					else {

						meniuOptionWhenSelected = false;
					}


					// print about  menu  what it's doing
					print(func_stored[meniuOptionSelectFun()].__functionName);
					
					// if user select a desire function, then change his value
					if (meniuOptionIsSelected) {
						// print something specific  about a this function out frtom this scope 
						func_stored[meniuOptionSelectFun()].fnc_();
						// change values by spinint/pushing up or down buttons 
						userChangeMeniuValue(func_stored[meniuOptionSelectFun()].__functionValueAddress);

						// if allowed to print function
						//if (func_stored[includedMenuCount].printfunctionValue)
							//print stored values from a memory 
							print(*func_stored[meniuOptionSelectFun()].__functionValueAddress);
					}


					//  EXIT
					 if (meniuOptionSelectFun() == includedMenuCount  ) { // Exit
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
			else if (*buttonSET == LOW || boolQuicklyChange) {
				// set loop from boolean



				// exit from tight loops!
				// event trigger if after some time will react to button set
				if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 230UL)) && *buttonSET || /*or timeout for 45s*/(millis() > (long)(startedWaiting + 45100UL))) {
					// reset whole set loop
					boolQuicklyChange = false;
					boolSetButton = false;
					userSetValuesToMemory(); // write temperature changes to memory
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




						// print name of the function 
						print(func_stored[includedMenuCount+1].__functionName);
					
						// specific function to call outside this class
						func_stored[includedMenuCount+1].fnc_();
					
						//change a value 

						userChangeMeniuValue(func_stored[includedMenuCount + 1].__functionValueAddress);


						// if allowed to print function
						if (func_stored[includedMenuCount+1].__printfunctionValue)
							//print stored values from a memory 
							print(*func_stored[includedMenuCount+1].__functionValueAddress);

						//var_manualMode

					display.set1X();

					//userChangeMeniuValue(&var_manualMode);






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
	


	// address automaticly by 1 byte each time 
	void userSetValuesToMemory() { // plus quick access function
		for (int_fast16_t count = 1; count < includedMenuCount +1  ; count++) {
			writeMemory(count, *func_stored[count].__functionValueAddress); //save all values to memory EEPROM
			print(String (count)+ ", " + String (func_stored[count].__funcAddress) +", "+String(*func_stored[count].__functionValueAddress));
		}

		delay(1000);
	};

	
	// address automaticly by 1 byte each time 
	void userGetValues() {
								// plus quick access function
		for (int count = 1; count < includedMenuCount + 1  ; count++) {
			
			byte val = readMemoryByte(count); //save all values to memory EEPROM
			
			*func_stored[count].__functionValueAddress = val;

			print(String(*func_stored[count].__functionValueAddress));
		}
		delay(1000);
	};


	void displayButtonsValue() {

		print("buttonSET:" + String(*buttonSET)); // getting value of the pointer
		print("buttonUP:" + String(*buttonUP)); // value of the pointer
		print("buttonDOWN:" + String(*buttonDOWN)); // value of the pointer
	};
	
	
	void displayRotaryValues() {
		print("buttonSET:" + String(*buttonSET)); // value of the pointer
		print("rotarySpinedSIDE:" + String(*rotarySpinedSIDE)); // value of the pointer
	}
};






































 //My Old Source of this lib below
/*



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
	
	//display.setTextSize(1);
	//display.println(txt);
	

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
	println(valueType + String(value).length());
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


void userChangeMeniuValue(bool* userPx, byte __delay = 10) {

	// incrament
	if ((buttonDOWN || buttonUP) && *userPx)
	{
		*userPx != *userPx;

		delay(__delay);
		//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
	}



}




bool meniuInterface() {



	if (buttonSET || boolSetButton) {
		boolSetButton = true;

		// Set menu for more options
		//Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds
		if (buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
			// set loop from boolean


			// exit from tight loops!
			// event trigger if after some time will react to button set
			// reset whole set loop
			//                                              Extra timer for not hit a first option
			if ((millis() > (long)(startedWaiting + 5000UL + 1550UL)) && buttonSET) {

				// After meniu option was selected 
				if (meniuOptionIsSelected) {
					userSetValuesToMemory();
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
					meniuDescribeOptionDisplay("<Min Preasure Water");


					// do EEPROM changes
					if (meniuOptionIsSelected) {
						display.set2X();
						// Change values interface
						print(var_Water_Preasure_Minimum);

						userChangeMeniuValue(&var_Water_Preasure_Minimum);

					}

				}
				else if (meniuOptionSelectFun() == 2) { // P1 Temp Logic offset 0.5
					//display.println("[P1]");
					meniuDescribeOptionDisplay("<Max Preasure Water");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						display.set2X();
						// Change values interface
						print(var_Water_Preasure_Maximum);

						userChangeMeniuValue(&var_Water_Preasure_Maximum);

					}
				}
				else if (meniuOptionSelectFun() == 3) { // Not included
					//display.println("[P2]");
					meniuDescribeOptionDisplay("Water Flow Sensor Min");

					// do EEPROM changes
					if (meniuOptionIsSelected) {
						display.set2X();
						// Change values interface
						print(var_Water_Flow_Sensor_Minimum);

						userChangeMeniuValue(&var_Water_Flow_Sensor_Minimum);

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
							userSetDefault();
							userSetValuesToMemory();


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
			if ( Wait while user realesing a button(millis() > (long)(startedWaiting + 230UL)) && buttonSET || /*or timeout for 45s(millis() > (long)(startedWaiting + 45100UL))) {
				// reset whole set loop
				boolQuicklyChange = false;
				boolSetButton = false;
				userSetValuesToMemory(); // write temperature changes to memory
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



				print("Siurbimo Busena");

				// Change values interface


				display.set2X();
				display.setCursor(5, 5);

				if (var_manualMode)
					print("Auto");
				else
					print("Manual");

				display.set1X();

				userChangeMeniuValue(&var_manualMode);






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


//------------------------------------------------- Meniu Ends-------------

*/







