#pragma once

// THIS LIBRARY NOT DONE
// const Varables

#define CONST_TOTALMENUELEMENTS  15 // maximum ammount of elemtents(functions) in meniu 

//Structure
#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //0x3F mine or 0x27 need to find out with :LINK:http://playground.arduino.cc/main/i2cscanner
#include<stdlib.h>
#include <stdio.h>
#include "EEPROM32.h"
using namespace std;



// 0X3C+SA0 - 0x3C or 0x3D
//0x3F mine or 0x27 need to find out with :LINK:http://playground.arduino.cc/Main/I2cScanner
#define I2C_ADDRESS 0x27

// Define proper RST_PIN if required.
#define RST_PIN -1


LiquidCrystal_I2C display(I2C_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //was 0x3F  Eldas: 0x27 ,A4 and A5 Are Used



//String SPACE = "                ";
void print(String txt) {
	display.print(txt);
//	display.println(SPACE);

}


void print(byte txt) {
	display.print(txt);
	//display.println(SPACE);

}


void println(byte txt) {
	display.print(txt);
	//display.println(SPACE);

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








//store sub functions and variables from each menu block
struct funcBuffer {

public:	 // Only works in public , private throw error 
	// stored function
	void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
	String __functionName;// function abroviation what are doing 
	byte *__functionValueAddress;// this variable Address are stored in here to change or read a value. Also use as direct data to save in EEPROM or save back to.
	String __functionValueAbbreviation;// are for describe a "functionValueAddress" point what that mean:celcius,fahrenheit etc...
	bool __printfunctionValue; // allow to print a numeric/rest value of  this function
	int __funcAddress; // address of a value that are addressed in to EEPROM 
	
	
					   
   // Execute Selected menu option and point to responsive function
	void  DrawFunction() { //Expect Selected menu function

		fnc_(); // How Call Pointed Function ;

	};


	void IncludeFunction(void (*functionPointer)(), byte &functionValueAddress , String functionName = "unknown" , String functionValueAbbreviation = "",  bool printfunctionValue = true ) { // where magic on
		fnc_ = functionPointer;
		__functionName = functionName;
		__functionValueAddress = &functionValueAddress;
		__printfunctionValue = printfunctionValue;
		__functionValueAbbreviation = functionValueAbbreviation;
	};


	//funcBuffer* next; // from iterators
	
};



//funcBuffer  func_store = funcBuffer();      // Zero initialize using default constructor
//funcBuffer func_store{};          // Latest versions accept this syntax.
//funcBuffer* func_store = new funcBuffer();  // Zero initialize a dynamically allocated object.




// Store a value to memory while also can be used as menu options for a specific task
class lib_meniuInterface16x2_LiquidCrystal_I2C
{

	// old ones

	bool boolSetButton = false;
	bool boolSetMenu = false;
	bool boolQuicklyChange = false;
	private:bool boolClearAfterSetBtn = false;	  // clear display after entering or exiting menu
	byte meniuOptionSelected = 1;
	
	private:bool meniuOptionIsPressing = false;// detect when button no longer is press
    private:bool meniuOptionIsSelected = false; // When eventually are at P0-P3 option then time to select what specificity are changing a values that are included in the statement.
	private:bool meniuOptionWhenSelected = false;// for clearing one time a menu option are change
	unsigned long startedWaiting;
	unsigned long startedWaitingmeniuOptionSelected;
	byte toggleDisplay = 0;
	
	// less important
	bool isDisplayCleared = false; // track if display was cleared when exiting from meniuOptionIsSelected

		
	//////
	bool *buttonUP;
	bool *buttonDOWN;
	bool *buttonSET;
	byte *rotarySpinedSIDE;
	bool isRoatry = false; // detect where is a rotary encoder

	// if default function exist then use in menu to set a new default
public:void (*defaultFunc)();
	bool defaultIsFunc = false;

	int_fast16_t  includeStartingPointMem = 10; // where start to populate values from menu options to a memory EEPROM
	int_fast16_t includedMenuCount = 2; // start generate by a count a slow access menu position in memory EEPROM
	int_fast16_t includeQuckAccessMenu = 1; // always first positioning

	//ADDRESS STORED Stored memory management
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
	lib_meniuInterface16x2_LiquidCrystal_I2C(bool &buttonUp , bool &buttonDown, bool &buttonSet) {
		buttonUP = &buttonUp; // store address of a buttons
		buttonDOWN = &buttonDown; // store address of a buttons
		buttonSET = &buttonSet; // store address of a buttons

		//func_store[5];
	}

	// if using rotary encoder " Not advanced propetly right now"
	lib_meniuInterface16x2_LiquidCrystal_I2C(byte &rotarySpinedSide, bool  & buttonSet) {
		rotarySpinedSIDE = &rotarySpinedSide;
		buttonSET = &buttonSet;
		isRoatry = true;

	}
	



	// access menu by long pressed a Set button
public:void IncludeFunction(void (*functionPointer)(), byte& functionValue, String functionName = "Uknown"  , String functionValueAbbreviation = "", bool printfunctionValue = true)
	{ // where magic on
		func_stored[includedMenuCount ].IncludeFunction( (functionPointer), functionValue,  functionName , functionValueAbbreviation, printfunctionValue);

		includedMenuCount++;
	}


	  // access menu by long pressed a Set button
	  // specific function to set a default
public:void IncludeFunctionSetDefault(void (*functionPointer)() )
{ 

	defaultFunc = functionPointer;
	//includedMenuCount++;
	defaultIsFunc = true;
}

	// access menu by  short pressed a Set button
public:void IncludeQuckAccessFunction( void (*functionPointer)(), byte & functionValue , String functionName = "unknown"   , String functionValueAbbreviation = "", bool printfunctionValue = true)
	{
		func_stored[includeQuckAccessMenu].IncludeFunction((functionPointer), functionValue, functionName, functionValueAbbreviation,   printfunctionValue );
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





	  void initiateDisplay() {

		  Wire.begin();
		  Wire.setClock(400000L); // 800000L or 400000L
		  delay(500);
		  display.begin(16, 2);
		  delay(1000);
		  display.begin(16, 2);
		  delay(500);
		  display.setBacklight(HIGH);
		  //	  
		  userGetValues();// load values from EEPROM memory!
	  }



	//------------------------------------------------------------------
private:void meniuDescribeOptionDisplay(String txt) {
		display.setCursor(0, 1); //(0,9)
		
		//display.setTextSize(1);
		//display.println(txt);
		

		print(txt);
	}

	   // only for a buttons/ badly on rotary encoder
private:byte meniuOptionSelectFun() {


		// if pressing a button and menu where are selected and not are to big then...
		if ((*buttonUP && meniuOptionSelected < includedMenuCount /*+ 1 <Error out of menu bounderies*/) && meniuOptionIsPressing && !meniuOptionIsSelected) {

			++meniuOptionSelected;
			startedWaitingmeniuOptionSelected = millis();// time out start point
			display.clear();

		}
		// if pressing a button and menu where are selected and are not  smaller then...
		if ((*buttonDOWN && meniuOptionSelected > 1) && meniuOptionIsPressing && !meniuOptionIsSelected) {

			--meniuOptionSelected;
			startedWaitingmeniuOptionSelected = millis();// time out start point
			display.clear();

		}

		// Give new timeout and starting point for menu functions to work
		if (*buttonSET && !meniuOptionIsSelected) {
			//meniuOptionIsPressing = true;
			startedWaitingmeniuOptionSelected = millis();// time out start point
		}

		// if user do not do for a while , occurs a time out.
		if ((millis() > (long)(startedWaitingmeniuOptionSelected + 45000UL)) && !meniuOptionIsSelected)
		{
			boolSetButton = false; // exit from boolSetButton statement
			boolSetMenu = false; // exit from boolSetMenu statement
			meniuOptionSelected = 1; //reset menu position to P0
			delay(10);
			display.clear(); // works as timeout display cleaner
			delay(10);
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



	// SPECIAL BUGGY FUNCTION! 
	// user when changing something in meniu options, doest trow here 
private:void userChangeMeniuValue(byte* userPx, byte __delay = 10, byte minValue = 0, byte maxValue = 255) {

		// increment
		if (*buttonUP && *userPx < maxValue)
		{
			*userPx = *userPx + 1;

			delay(__delay);
			//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button // Start to  wait from begining
		}

		// subtract 
		if (*buttonDOWN && *userPx > minValue)
		{
			*userPx = *userPx - 1;
			delay(__delay);
			//startedWaiting = millis();  //reset meniu delay counter // LAG When  push SET Button // Start to  wait from begining
		}

	}


/*
private:void userChangeMeniuValue(bool* userPx, byte __delay = 10) {

		// increment
		if ((*buttonDOWN ||* buttonUP) && *userPx)
		{
			*userPx != *userPx;

			delay(__delay);
			//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
		}



	}
	*/

//clear display one time before menu and after 
private:void funClearAfterSetButton (bool statement/*of how to react*/, bool saveCondition/*how to save after codition for not reapeting inself*/) {
	
	if (boolClearAfterSetBtn == statement)
	{
		
		boolClearAfterSetBtn = saveCondition;
		Serial.print("boolClearAfterSetBtn:" + String (boolClearAfterSetBtn) + "\n");
		display.clear();
		delay(10);
		
	}
	
}

private:void menuSelectedPrint() {

	byte spacer;
	
	funClearAfterSetButton (true,false);
	
// when menu option P1-P3 is  selected and avoid overlapping with functionvalue name and menu index value in square brackets (( [P3] with var3 ( [vP3a]r3 ) ))
	
	/*
	if (meniuOptionSelectFun() < 10)
		spacer = 7;
	else
	{
		spacer = 4;
	}
*/
	
	display.setCursor(spacer, 0);

	// print something specific  about a this function out frtom this scope 
	func_stored[meniuOptionSelectFun() + 1].fnc_();

	

	// if allowed to print function
	//if (func_stored[includedMenuCount].printfunctionValue)

	if (func_stored[meniuOptionSelectFun() + 1].__printfunctionValue) {

		display.setCursor(0, 1);
		//print stored values from a memory 
		print(func_stored[meniuOptionSelectFun() + 1].__functionValueAbbreviation +":"+ *func_stored[meniuOptionSelectFun() + 1].__functionValueAddress  + " ");

		//		display.set1X();
		//print(" " + func_stored[meniuOptionSelectFun() + 1].__functionValueAbbreviation);

		//display.setCursor(0, 30);
	}
	// change values by spinint/pushing up or down buttons 
	userChangeMeniuValue(func_stored[meniuOptionSelectFun() + 1].__functionValueAddress);

}
private:void menuQuckAccesPrint() {

	//display.set2X();
	
	funClearAfterSetButton (true,false);
	
	display.setCursor(0, 0);

	// print name of the function 
	print("[-]" +func_stored[includeQuckAccessMenu].__functionName);
	// print something specific  about a this function out from this scope 
	// specific function to call outside this class
	func_stored[includeQuckAccessMenu].fnc_();





	// if allowed to print function
	if (func_stored[includeQuckAccessMenu].__printfunctionValue) {
		//print stored values from a memory 

//		display.set2X(); 
		display.setCursor(0, 1);

		print(func_stored[includeQuckAccessMenu].__functionValueAbbreviation + ":" + *func_stored[includeQuckAccessMenu].__functionValueAddress + " ");

	}

	//change a value 
	userChangeMeniuValue(func_stored[includeQuckAccessMenu].__functionValueAddress);

}



public:bool InterfaceDinamic() {
		
	// Slow access options
		if (*buttonSET || boolSetButton) {
									 
						
					 boolSetButton = true;
				
				
			// Set menu for more options
			/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
			if (*buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
				// set loop from boolean
				// BEFORE menu option was selected and exiting out

				if (!boolSetMenu && !boolQuicklyChange) // clear text noise from main program
					display.clear();

				// exit from tight loops!
				// event trigger if after some time will react to button set
				// reset whole set loop
				//                                              Extra timer for not hit a first option
				if ((millis() > (long)(startedWaiting + 5000UL + 1500UL)) && *buttonSET) {
					
					// BEFORE menu option was selected and exiting out
					//if (!isDisplayCleared) { display.clear(); }; // clear
					//isDisplayCleared = true;

					// After menu option was selected and exiting out
					if (meniuOptionIsSelected) {
						userSetValuesToMemory(meniuOptionSelectFun());
							
						display.clear();

						delay(60); //60
					}
					else
						isDisplayCleared = false; //reset status about cleared display

					//  if menu option selected and need change a values to EEPROM
					meniuOptionIsSelected = !meniuOptionIsSelected;

					// Exit from progra  and main boolSetButton loop...if menu not selected
					if (meniuOptionSelectFun() == includedMenuCount) { // Exit

						boolSetButton = false; // exit from boolSetButton statement
						boolSetMenu = false; // exit from boolSetMenu statement
						meniuOptionSelected = 1; //reset menu position to P0
						meniuOptionIsSelected = false; // when user exit from menu set to default 
	//					display.set1X();
					}
					//userSetValuesToMemory();
					//while (*buttonSET == true)
						delay(230);// wait until set button not resealed
				}
				else
				{
					// set whole set loop by default if no buttons event occurs
					boolSetMenu = true;
					boolSetButton = true;

				}

				//progra-------------------------------------------------------
				if ((*buttonSET || boolSetMenu)) {

					

					display.setCursor(0, 0);
//					display.set1X();

					if (meniuOptionSelectFun()  < includedMenuCount ) // only display menu option values except on 'EXIT'
						
						if (meniuOptionWhenSelected)
							print("P" + String(meniuOptionSelectFun()) + "]");
						//else
							//print( String(meniuOptionSelectFun()) + ">");



					// if menu option not selected print about menu abbreviation

					if (meniuOptionWhenSelected) {
						//print("-");
						//print("");
					}
					// if not menu option selected and menu are not at EXIT point
					if (!meniuOptionIsSelected && meniuOptionSelectFun()   < includedMenuCount) {
						meniuOptionWhenSelected = true;

						//print("      MENIU");//display.println("MENIU"); 

					}
					else {

						meniuOptionWhenSelected = false;
					}



					//Set to Default
					//check if option is available
					if (defaultIsFunc && meniuOptionSelectFun()  == includedMenuCount - 1) { // set to default 
						display.setCursor(0, 1);
						print("Reset to default");
						

						if (meniuOptionIsSelected) {
							display.setCursor(0, 0);
							print("Press up to reset");

							if (*buttonUP)
							{
	//							display.set2X();
								//display.setCursor(0, 1);
								print(" Done");
	//							display.set1X();
								// reset to default
								defaultFunc();

								delay(2000);
								meniuOptionIsSelected = !meniuOptionIsSelected; // back to menu options
								display.clear();
							}
						}
					}
					else {
						// ----------------------do rest of options------------------------------------

						// print about  menu  what it's doing
						print(func_stored[meniuOptionSelectFun() + 1].__functionName); // + 1 to avoid to stumble a Quick access menu option

						// if user select a desire function, then change his value
						if (meniuOptionIsSelected && boolSetMenu) {
							menuSelectedPrint();
						}


					}


					
					//  EXIT-----------------------------------------
					 if (meniuOptionSelectFun()  >=/*if '==' then crashes*/ includedMenuCount  ) { // Exit

						print("    [EXIT] ");

						if (meniuOptionIsSelected) {

							//boolSetMenu = false;
							//boolQuicklyChange = false;
							delay(10);
							display.clear(); // somewhere crashes
							delay(10);
						}
					


					}

					

					 // clear [EXIT] stuff left on screen
					 if (meniuOptionSelectFun() == includedMenuCount - 1)
					 {
						 if (!isDisplayCleared) {

							// print(SPACE);
							// print(SPACE);
							// print("");
							// delay(120);
							 display.clear(); // somewhere crashes
							
						 }

						 isDisplayCleared = true; // display was cleared 
					 }
					 else
						 isDisplayCleared = false; //reset status about cleared display


	//				 display.set1X(); 
					 
				}


			}





//-----------------// Quickly Access/change single option-----------------------------------------
			else if (*buttonSET == LOW || boolQuicklyChange) {
				// set loop from boolean



				// exit from tight loops!
				// event trigger if after some time will react to button set
				if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 230UL)) && *buttonSET || /*or timeout for 45s*/(millis() > (long)(startedWaiting + 45100UL))) {
					// reset whole set loop
					boolQuicklyChange = false;
					boolSetButton = false;
					userSetValuesToMemory(); // write temperature changes to memory
					display.clear();
					delay(200);
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


					menuQuckAccesPrint();


				}
			}

		}
		else
		{
			startedWaiting = millis(); //by default will register values until condition
			boolQuicklyChange = false; //activate condition ones until event is triggered
			boolSetMenu = false; // set default to menu option
			//display.clear(); // blinking...
			
			
			
			funClearAfterSetButton (false,true);
		}


		return boolSetMenu || boolQuicklyChange;
	}
	


	// address automatically by 1 byte each time 
public:void userSetValuesToMemory(int index = -1) { // plus quick access function
	if (index > -1) {
		for (int_fast16_t count = 1; count < (includedMenuCount); count++) {
			writeMemory((count + includeStartingPointMem), *func_stored[count].__functionValueAddress); //save all values to memory EEPROM
			//print(String (count)+ ", " + String (func_stored[count].__funcAddress) +", "+String(*func_stored[count].__functionValueAddress));
		}
	}
	else {
		writeMemory((index + includeStartingPointMem), *func_stored[index].__functionValueAddress); //save all values to memory EEPROM
	}
		//delay(1000);
	};

	
	// address automatically by 1 byte each time 
public:void userGetValues() {
								// plus quick access function
		for (int count = 1; count < (includedMenuCount)  ; count++) {
			
			byte val = readMemoryByte(count + includeStartingPointMem); //save all values to memory EEPROM
			
			*func_stored[count].__functionValueAddress = val;

			//print(String(*func_stored[count].__functionValueAddress));
		}
		//delay(5000);
	};


	void displayButtonsValue() {
	String btn="btn";
		print(btn+"SET:" + String(*buttonSET)); // getting value of the pointer
		print(btn+"UP:" + String(*buttonUP)); // value of the pointer
		print(btn+"DOWN:" + String(*buttonDOWN)); // value of the pointer
	};
	
	
	void displayRotaryValues() {
		print("btnSET:" + String(*buttonSET)); // value of the pointer
		print("rotarySIDE:" + String(*rotarySpinedSIDE)); // value of the pointer
	}
};



/*
Example 
lib_meniuInterface128x64OLEDSSD1306AsciiWire menu(buttonUP, buttonDOWN, buttonSET);
// 
setup() {

// add function to menu and varables to save in EEPROM memory 
	// function are included to a system

	menu.IncludeFunction(&func1, var_Water_Preasure_Minimum, "Water MInimum","psi");
	menu.IncludeFunction(&func2, var_Water_Preasure_Maximum, "Water maXimum","psi");
	menu.IncludeFunction(&func3, var_Water_Flow_Sensor_Minimum, "Flow minimum","l/min");
	menu.IncludeFunction(&func4,var_Allow_External_Button, "Sensor Grab","val",false);
	menu.IncludeFunction(&func4, var_Allow_External_Button, "Farger Potato", "val", true);
	
	//separate functions to include more specific menu options
	menu.IncludeQuckAccessFunction(&func7, var_manualMode, "Quic Access","psi",true);
	menu.IncludeFunctionSetDefault(&func6Default);

	menu.initiateDisplay();

	
	// pinModes



	// set up memory default
	if (readMemoryByte(0)!= true ) {

		print("MEMORY SET!");
		writeMemory(0, true);
		menu.defaultFunc();// call a default function to set a new values
		menu.userSetValuesToMemory();
	};

	menu.userGetValues();

}// setup end

void loop () {
	



	display.setCursor(0, 0);

	 buttonSET = digitalRead(BUTTON_SET);
	 buttonDOWN = digitalRead(BUTTON_DOWN);
	 buttonUP = digitalRead (BUTTON_UP);

	 rotory = rotaryEncoderDirection(&buttonUP, &buttonDOWN);


	//print(meniuOption[meniuIndex]);

	if (!menu.InterfaceDinamic()) {
		//menu.displayButtonsValue();

		print(menu.func_stored[1].__functionName); // access a unit from menu memory 

		print("PRogram Are Here");


	}

}


// external function 
void userSetDefault() {
	var_Water_Preasure_Minimum = 5;  // minimum of water preasure to turn on a water source unit
	var_Water_Preasure_Maximum = 5;  // Maximum of water preasure to turn on a water source unit
	var_Water_Flow_Sensor_Minimum = 5; //  minimum of water flow to turn on a water source unit
	var_Allow_External_Button = 5; // react to external button
	var_Allow_Exeption_Source_Vin = 5; // react to when  heater  is on to turn on a water source unit
	var_Source_Unit_Timeout = 5;	// Turn on a motor/solenoid(water source available) for some time to equalize a fliquating sensors inputs
	var_manualMode = 4;
	menu.userSetValuesToMemory();

};
*/






























