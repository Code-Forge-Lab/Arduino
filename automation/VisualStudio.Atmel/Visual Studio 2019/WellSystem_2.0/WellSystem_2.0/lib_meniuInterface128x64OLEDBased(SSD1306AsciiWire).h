#pragma once
// newest!.

// const Varables

#define CONST_TOTALMENUELEMENTS  15 // maximum ammount of elemtents(functions) in meniu 

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



// no control when menu exist
String SPACE = "                        ";
void print(String txt) {
	display.print(txt);
	display.println(SPACE);

}


void print(int txt) {
	display.print(txt);
	display.println(SPACE);

}


void println(int txt) {
	display.print(txt);
	//display.println(SPACE);Ftext

}


void println(String txt) {
	display.print(txt);
	//display.println(SPACE);

}

// clear start at where was last print end.
void clearinDisplay() {

	print("");
	print("");
	print("");
	print("");
	print("");
	print("");
	print("");
	print("");
	print("");
}





struct funcBuffer {

public:	 // Only works in public , private throw error 
	// stored function
	void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
	String __functionName;// function abroviation what are doing 
	byte* __functionValueAddress;// this variable Address are stored in here to change or read a value. Also use as direct data to save in EEPROM or save back to.
	String __functionValueAbbreviation;// are for desribe a "functionValueAddress" point what that mean:celcius,fahrenheit ect...
	bool __printfunctionValue; // allow to print a numeric/rest value of  this function
	int __funcAddress; // address of avalue that are addresed in to EEPROM 



   // Execute Selected menu option and point to responsive function
	void  DrawFunction() { //Expect Selected menu function

		fnc_(); // How Call Pointed Function ;

	};


	void IncludeFunction(void (*functionPointer)(), byte& functionValueAddress, String functionName = "Uknown", String functionValueAbbreviation = "", bool printfunctionValue = true) { // where magic on
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
class lib_meniuInterface128x64OLEDSSD1306AsciiWire
{

	// old ones

	bool boolSetButton = false;
	bool boolSetMenu = false;
	bool boolQuicklyChange = false;
	byte meniuOptionSelected = 1;

	bool meniuOptionIsPressing = false;// detect when button no longer is press
	bool meniuOptionIsSelected = false; // When eventualy are at P0-P3 option then time to select what specificly are changing a values that are included in the statement.
	bool meniuOptionWhenSelected = false;// for clearing one time a meniu option are change
	unsigned long startedWaiting;
	unsigned long startedWaitingmeniuOptionSelected;
	byte toggleDisplay = 0;
	bool isMenuOpened = false; // know about menu when is opened and when is not. To avoid printing on menu with other uncontroled 'Print' functions
	// less inportant
	//bool isDisplayCleared = false; // track if display was cleared when exitind from meniuOptionIsSelected
public:bool isclearedDisplayCommon = false; // comontry use to clear once when enter in a option 
	  bool isCompletedChangeValueToExitFromSelectedOption = false; // more robust Set button press waiter

	  //////
	  bool* buttonUP;
	  bool* buttonDOWN;
	  bool* buttonSET;
	  byte* rotarySpinedSIDE;
	  bool isRoatry = false; // detect where is a rotary encoder

	  // if default function exsist then use in menu to set a new default
public:void (*defaultFunc)();
	  bool defaultIsFunc = false;

	  int_fast16_t  includeStartingPointMem = 10; // where start to populate values from menu options to a memory EEPROM
	  int_fast16_t includeQuckAccessMenu = 1;  // //*not tested!*/ +includeStartingPointMem; // always first positioning
	  int_fast16_t includedMenuCount = 2; // start generate by a count a slow access menu position in memory EEPROM. Any of menu option start + 1 from includeQuckAccessMenu or 2


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
	lib_meniuInterface128x64OLEDSSD1306AsciiWire(bool& buttonUp, bool& buttonDown, bool& buttonSet) {
		buttonUP = &buttonUp; // store address of a buttons
		buttonDOWN = &buttonDown; // store address of a buttons
		buttonSET = &buttonSet; // store address of a buttons

		//func_store[5];
	}

	// if using rotary encoder " Not advanced propetly right now"
	lib_meniuInterface128x64OLEDSSD1306AsciiWire(byte& rotarySpinedSide, bool& buttonSet) {
		rotarySpinedSIDE = &rotarySpinedSide;
		buttonSET = &buttonSet;
		isRoatry = true;

	}



public:void print__(String txt) {
	if (!isMenuOpened) {
		display.print(txt);
		display.println(SPACE);
	}

}


public:void print__(int txt) { // also works with bytes, no casting needed

	if (!isMenuOpened) {
		display.print(txt);
		display.println(SPACE);
	}

}


public:void println__(int txt) {
	if (!isMenuOpened)
		display.print(txt);
	//display.println(SPACE);Ftext

}


public:void println__(String txt) {
	if (!isMenuOpened)
		display.print(txt);
	//display.println(SPACE);

}

	  // clear start at where was last print end.
public:void clearinDisplay__() {

	if (!isMenuOpened) {
		print("");
		print("");
		print("");
		print("");
		print("");
		print("");
		print("");
		print("");
		print("");
	}
}


	  // access menu by long pressed a Set button
public:void IncludeFunction(void (*functionPointer)(), byte& functionValue, String functionName = "Uknown", String functionValueAbbreviation = "", bool printfunctionValue = true)
{ // where magic on
	func_stored[includedMenuCount].IncludeFunction((functionPointer), functionValue, functionName, functionValueAbbreviation, printfunctionValue);

	includedMenuCount++;
}


	  // access menu by long pressed a Set button
	  // specific function to set a default
public:void IncludeFunctionSetDefault(void (*functionPointer)())
{

	defaultFunc = functionPointer;
	//includedMenuCount++;
	defaultIsFunc = true;
}

	  // access menu by  short pressed a Set button
public:void IncludeQuckAccessFunction(void (*functionPointer)(), byte& functionValue, String functionName = "Uknown", String functionValueAbbreviation = "", bool printfunctionValue = true)
{
	func_stored[includeQuckAccessMenu].IncludeFunction((functionPointer), functionValue, functionName, functionValueAbbreviation, printfunctionValue);

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




	  // initiate display and load memory settings to a program

	  void initiate() {

		  Wire.begin();
		  Wire.setClock(800000L); // 800000L or 400000L

#if RST_PIN >= 0
		  display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
		  display.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

		  display.setFont(Adafruit5x7);

		  //
					// check if memory values not set as default
		  if (readMemoryByte(0) != true) {

			  print("MEMORY SET!");
			  writeMemory(0, true);
			  defaultFunc();// call a default function to set a new values 
			  userSetValuesToMemory();
		  };
		  // each startup fech a stored values from the memrory.
		  userGetValues();
	  }



	  //------------------------------------------------------------------
private:void meniuDescribeOptionDisplay(String txt) {
	display.setCursor(0, 9);

	//display.setTextSize(1);
	//display.println(txt);


	print(txt);
}

	   // only for a buttos/ badly on rotary encoder
private:byte meniuOptionSelectFun() {


	// if pressing a button and meniu where are selected are not to big then...
	if ((*buttonUP && meniuOptionSelected < includedMenuCount + 1) && meniuOptionIsPressing && !meniuOptionIsSelected) {

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
		isclearedDisplayCommon = false;

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
private:void userChangeMeniuValue(byte* userPx, byte __delay = 10, byte minValue = 0, byte maxValue = 255) {

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



private:void userChangeMeniuValue(bool* userPx, byte __delay = 10) {

	// incrament
	if ((*buttonDOWN || *buttonUP) && *userPx)
	{
		*userPx != *userPx;

		delay(__delay);
		//startedWaiting = millis(); //reset meniu delay counter // LAG When  push SET Button
	}



}

private:void menuSelectedPrint(int value = 0, bool value_print = false) {
	// print something specific  about a this function out frtom this scope 
	func_stored[meniuOptionSelected + 1].fnc_();

	display.set2X();
	// if allowed to print function
	//if (func_stored[includedMenuCount].printfunctionValue)

	if (func_stored[meniuOptionSelected + 1].__printfunctionValue) {
		//print stored values from a memory 
		println(String(*func_stored[meniuOptionSelected + 1].__functionValueAddress) + " ");

		display.set1X();
		print(func_stored[meniuOptionSelected + 1].__functionValueAbbreviation);

		//display.setCursor(0, 30);
	} // if not allowed to print 

	// change values by spinint/pushing up or down buttons 
	userChangeMeniuValue(func_stored[meniuOptionSelected + 1].__functionValueAddress);
}

private:void menuQuckAccesPrint() {

	//display.set2X();

	print("[-]");
	// print name of the function 
	print(func_stored[includeQuckAccessMenu].__functionName);
	// print something specific  about a this function out frtom this scope 
	// specific function to call outside this class
	func_stored[includeQuckAccessMenu].fnc_();





	// if allowed to print function
	if (func_stored[includeQuckAccessMenu].__printfunctionValue) {
		//print stored values from a memory 

		display.set2X();
		println(String(*func_stored[includeQuckAccessMenu].__functionValueAddress) + " ");

		display.set1X();
		print(func_stored[includeQuckAccessMenu].__functionValueAbbreviation);
		//var_manualMode

	} // if not allowed to print 


	//change a value 
	userChangeMeniuValue(func_stored[includeQuckAccessMenu].__functionValueAddress);

}



	   // Interpeter all alligment and print a manual value in custom function = .func
public: void menuPrintManuallyValue(int value) {

	//print(meniuOptionSelected + 1);

	if (!func_stored[meniuOptionSelected + 1].__printfunctionValue) { // if allowed to print in config
		display.set2X();
		println(String(value) + " ");

		display.set1X();
		print(func_stored[meniuOptionSelected + 1].__functionValueAbbreviation);
	}
}

	  // Interpeter all alligment and print a manual value in custom function = .func only for quick access menu
public:void menuQuckAccesPrintManuallyValue(int value) {

	//print(meniuOptionSelected + 1);

	if (!func_stored[includeQuckAccessMenu].__printfunctionValue) { // if allowed to print in config
		display.set2X();
		println(String(value) + " ");

		display.set1X();
		print(func_stored[includeQuckAccessMenu].__functionValueAbbreviation);
	}

}

	  // clear display just at the moment needed
private: void clearMenuDisplay() {
	if (!isclearedDisplayCommon) // clear display
	{
		isclearedDisplayCommon = true;
		display.clear();
	}
}

public:bool InterfaceDinamic() {
	// always set display at zero state.	
	display.setCursor(0, 0);

	if (!*buttonSET) {
		if (isCompletedChangeValueToExitFromSelectedOption && !*buttonSET) {
			isCompletedChangeValueToExitFromSelectedOption = false; // help for a pressing to short to press while it needet
		}
	}
	// Slow access options
	if ((*buttonSET || boolSetButton)) {
		boolSetButton = true;

		// when in menu selected option and pressing a set button to exit to common parameters. 
		if (meniuOptionIsSelected && *buttonSET) {
			isclearedDisplayCommon = false;
		}

		// Set menu for more options
		/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
		if (*buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu) {
			// set loop from boolean
			clearMenuDisplay();

			// exit from tight loops!
			// event trigger if after some time will react to button set
			// reset whole set loop
			//                                              Extra timer for not hit a first option
			if ((millis() > (long)(startedWaiting + 5000UL + 1550UL)) && *buttonSET) {

				// After meniu option was selected 
				if (meniuOptionIsSelected) {
					userSetValuesToMemory(meniuOptionSelectFun() + 1);// start common menu options addresing in memory from 2, skipping 1 place.


					display.set1X();
					if (meniuOptionSelectFun() < includedMenuCount) // only display meniu option values exept on 'EXIT'
						print("[P" + String(meniuOptionSelectFun()) + "]");



					//delay(60);
				}


				if (!isCompletedChangeValueToExitFromSelectedOption) // wait until user releas Set button 
				{//  if meniu option selected and need change a values to EEPROM
					meniuOptionIsSelected = !meniuOptionIsSelected;
					isCompletedChangeValueToExitFromSelectedOption = true;
				}

				// Exit from progra  and main boolSetButton loop...if meniu not selected
				if (meniuOptionSelectFun() == includedMenuCount) { // Exit

					boolSetButton = false; // exit from boolSetButton statement
					boolSetMenu = false; // exit from boolSetMenu statement
					meniuOptionSelected = 1; //reset meniu position to P0
					meniuOptionIsSelected = false; // when user exit from meniu set to default 
					isclearedDisplayCommon = false;
					display.set1X();
				}



				delay(130);// wait until set button not realesed


			}
			else
			{
				// set whole set loop by default if no buttons event accures
				boolSetMenu = true;
				boolSetButton = true;

			}

			//progra-------------------------------------------------------
			if ((*buttonSET || boolSetMenu)) {



				display.setCursor(0, 0);
				display.set1X();
				if (meniuOptionSelectFun() < includedMenuCount) // only display meniu option values exept on 'EXIT'
					print("[P" + String(meniuOptionSelectFun()) + "]");

				// if meniu option not selected print about meniu abbreviation

				if (!meniuOptionWhenSelected) {
					//print("-");
					//print("");



				}
				// if not meniu option selected and meniu are not at EXIT point
				if (!meniuOptionIsSelected && meniuOptionSelectFun() < includedMenuCount) {
					meniuOptionWhenSelected = true;

					print("      MENU");//display.println("MENIU"); 


				}
				else {

					meniuOptionWhenSelected = false;
				}



				//Set to Default
				//check if option is available
				if (defaultIsFunc && meniuOptionSelectFun() == includedMenuCount - 1) { // set to default 

					print("<Reset to default>");
					print("");

					if (meniuOptionIsSelected) {
						print("Press up to reset");

						if (*buttonUP)
						{
							display.set2X();
							print(" Done");
							display.set1X();
							// reset to default
							defaultFunc();
							//userSetValuesToMemory();
							delay(2000);
							meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
							display.clear();
						}
					}
				}
				else {
					// ----------------------do rest of options------------------------------------




					// print about  menu  what it's doing
					print(func_stored[meniuOptionSelectFun() + 1].__functionName); // + 1 to avoid to stuble a Quck access menu option


					// if user select a desire function, then change his value
					if (meniuOptionIsSelected && !isCompletedChangeValueToExitFromSelectedOption) {



						//print menu
						menuSelectedPrint();
					}



				}



				//  EXIT-----------------------------------------
				if (meniuOptionSelectFun() >= includedMenuCount) { // Exit
					display.setCursor(0, 0);

					display.set2X();
					print("");
					print("  [EXIT]");

					if (meniuOptionIsSelected) {

						boolSetMenu = false;
						boolQuicklyChange = false;
						isclearedDisplayCommon = false; // reset a clearing system
						display.clear();
						display.setCursor(0, 0);

					}



				}



				// clear [EXIT] stuff left on screen
			   // if (meniuOptionSelectFun() == includedMenuCount - 1)




				display.set1X();
			}


		}





		//-----------------// Quickly Access/change single option-----------------------------------------
		else if (*buttonSET == LOW || boolQuicklyChange) {
			// set loop from boolean



			// exit from tight loops!
			// event trigger if after some time will react to button set
			if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 730UL)) && *buttonSET || /*or timeout for 45s*/(millis() > (long)(startedWaiting + 45100UL))) {
				// reset whole set loop
				boolQuicklyChange = false;
				boolSetButton = false;
				isclearedDisplayCommon = false;
				userSetValuesToMemory(); // write temperature changes to memory includeQuckAccessMenu
				display.clear();
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

				//display.clear();

				//if (isDisplayCleared)

				clearMenuDisplay();

				menuQuckAccesPrint();


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


	if (boolSetMenu || boolQuicklyChange) { // when exit from any option, reset a clearing display ones a  boolean.
		isMenuOpened = true;
	}
	else
	{
		isMenuOpened = false;
	};

	return boolSetMenu || boolQuicklyChange; // if any selected , give true.
}



	  // address automaticly by 1 byte each time 
public:void userSetValuesToMemory(int index = -1) { // plus quick access function
	if (index > -1) {
		// save single value to memory
		writeMemory((index + includeStartingPointMem), *func_stored[index].__functionValueAddress);

		//display.clear(); print("Single point debug");	print(String(index) + ",addr: " + String(func_stored[index].__funcAddress) + ",val:" + String(*func_stored[index].__functionValueAddress));

	}
	else {
		//save all values to memory EEPROM
		//display.clear(); print("Multyple point debug");

		for (int_fast16_t count = 1; count < (includedMenuCount); count++) {
			writeMemory((count + includeStartingPointMem), *func_stored[count].__functionValueAddress); //save all values to memory EEPROM
			//print(String(count) + ",addr:" + String(func_stored[count].__funcAddress) + "val:, " + String(*func_stored[count].__functionValueAddress));
		}

	}
	//delay(11000);
};


	  // address automaticly by 1 byte each time 
public:void userGetValues() {
	// plus quick access function
	for (int count = 1; count < (includedMenuCount); count++) {

		byte val = readMemoryByte(count + includeStartingPointMem); //save all values to memory EEPROM

		*func_stored[count].__functionValueAddress = val;

		//print(String(*func_stored[count].__functionValueAddress));
	}
	//delay(5000);
};



	  // print barebone information





	  void displayStoredMemoryValues() {
		  for (int count = 1; count < (includedMenuCount); count++) {

			  byte val = readMemoryByte(count + includeStartingPointMem); //save all values to memory EEPROM 

			  this->print__(func_stored[count].__functionName.substring(0, 8) + "," + func_stored[count].__printfunctionValue + ":" + String(*func_stored[count].__functionValueAddress) + " " + func_stored[count].__functionValueAbbreviation);
		  }
	  }

	  void displayButtonsValue() {

		  this->print__("buttonSET:" + String(*buttonSET)); // getting value of the pointer
		  this->print__("buttonUP:" + String(*buttonUP)); // value of the pointer
		  this->print__("buttonDOWN:" + String(*buttonDOWN)); // value of the pointer
	  };


	  void displayRotaryValues() {
		  this->print__("buttonSET:" + String(*buttonSET)); // value of the pointer
		  this->print__("rotarySpinedSIDE:" + String(*rotarySpinedSIDE)); // value of the pointer


	  }
};





























