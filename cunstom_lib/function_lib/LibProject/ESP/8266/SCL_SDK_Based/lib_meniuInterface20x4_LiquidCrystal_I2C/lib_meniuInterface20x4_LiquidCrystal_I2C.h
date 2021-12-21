#pragma once

// THIS LIBRARY NOT DONE
// const Varables

#define CONST_TOTALMENUELEMENTS  15 // maximum ammount of elemtents(functions) in meniu 

//Structure
//#include "Arduino.h"
//#include <Wire.h>
#include <LiquidCrystal_I2C.h> //0x3F mine or 0x27 need to find out with :LINK:http://playground.arduino.cc/main/i2cscanner
//#include<stdlib.h>
//#include <stdio.h>
#include "EEPROM32.h"
//using namespace std;

// To reduce hight write speed  to display
unsigned long timerPrint;
unsigned long timerPrintOffset = 1000UL;
byte 		  timePrintDelayOffset = 5; // after completly exited from menu , ignore for a while to set regular printing speed , to avoid slow transition bitween regular status print
bool timerPrintAvailable = true;



// 0X3C+SA0 - 0x3C or 0x3D
//0x3F mine or 0x27 need to find out with :LINK:http://playground.arduino.cc/Main/I2cScanner
#define I2C_ADDRESS 0x27

// Define proper RST_PIN if required.
#define RST_PIN -1


LiquidCrystal_I2C display(I2C_ADDRESS,20,4); // set the LCD address to 0x27 for a 16 chars and 2 line display or 20 chars and 4 collums



String SPACE = "                    ";
void print(String txt , byte line = 255 , byte column = 255) {


   if (timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    // Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

   // Serial.println (" " + txt);
    display.print(txt); 
  }
  

}


void print(byte txt , byte line = 255 , byte column = 255) {


  
  if (timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    //Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

    Serial.println (" " + txt);
    //display.print(txt);
  }
	//display.println(SPACE);

}


void println(byte txt , byte line = 255 , byte column = 255) {
 
  
 
  if (timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    //Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

    Serial.println (" " + txt);
    //display.print(txt);
  }
	//display.println(SPACE);

}


void println(String  txt , byte line = 255 , byte column = 255) {
 
  
 
  if (timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    //Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

    Serial.println (" " + txt);
    //display.print(txt);
  }
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




// clear start at where was last print end.
void clearinDisplay() {

	//print("");
	//print("");
	//print("");
	//print("");
	//print("");
	//print("");
	//print("");
	//print("");
	//print("");
	display.clear ();
}





struct funcBuffer {

public:	 // Only works in public , private throw error 
	// stored function
	void (*fnc_)(); // whiteout bracket not working , looks like try to use casting method
	String __functionName;// function abbreviation what are doing 
	byte* __functionValueAddress;// this variable Address are stored in here to change or read a value. Also use as direct data to save in EEPROM or save back to.
	String __functionValueAbbreviation;// are for describe a "functionValueAddress" point what that mean:celcius,Fahrenheit etc...
	bool __printfunctionValue; // allow to print a numeric/rest value of  this function
	int __funcAddress; // address of value that are addressed in to EEPROM 



   // Execute Selected menu option and point to responsive function
	void  DrawFunction() { //Expect Selected menu function

		fnc_(); // How Call Pointed Function ;

	};


	void IncludeFunction(void (*functionPointer)(), byte& functionValueAddress, String functionName = "unknown", String functionValueAbbreviation = "", bool printfunctionValue = true) { // where magic on
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
class lib_meniuInterface20x4_LiquidCrystal_I2C
{

	// old ones

	bool boolSetButton = false;
	bool boolSetMenu = false;
	bool boolQuicklyChange = false;
	byte meniuOptionSelected = 1;

	bool meniuOptionIsPressing = false;// detect when button no longer is press
	bool meniuOptionIsSelected = false; // When eventually are at P0-P3 option then time to select what specifically are changing a values that are included in the statement.
	bool meniuOptionWhenSelected = false;// for clearing one time a menu option are change
	unsigned long startedWaiting;
	int startedWaitingmeniuOptionSelected;
	byte toggleDisplay = 0;
	bool isMenuOpened = false; // know about menu when is opened and when is not. To avoid printing on menu with other uncontrolled 'Print' functions
	// less important
	//bool isDisplayCleared = false; // track if display was cleared when exitind from meniuOptionIsSelected

	//Work as delay to avoid open instanly after was closed 212
	byte delayOpenMenuQuickAccess = 0;
	byte delayOpenMenu = 0; // delay to open menu when was closed after  menu long press options
	bool delayOpenMenuFirstTime  = false; // when frist time opening 'long press' menu , give delay to avoid hitting P1 option in same time
		// default menu option
	byte delaySetToDefault = 0;
	byte delaySetToDefaultPressingButton = 0;

	bool basicToggle = false;
	byte basicToggleCount = 0; 

	unsigned long clock_1sec;
public:bool isclearedDisplayCommon = false; // common try use to clear once when enter in a option 
	  bool isCompletedChangeValueToExitFromSelectedOption = false; // more robust Set button press waiter

	  //////
	  bool* buttonUP;
	  bool* buttonDOWN;
	  bool* buttonSET;
	  byte* rotarySpinedSIDE;
	  bool isRoatry = false; // detect where is a rotary encoder

	  // if default function exist then use in menu to set a new default
public:void (*defaultFunc)();
	  bool defaultIsFunc = false;

	  int_fast16_t  includeStartingPointMem = 10; // where start to populate values from menu options to a memory EEPROM
	  int_fast16_t includeQuckAccessMenu = 1;  // //*not tested!*/ +includeStartingPointMem; // always first positioning
	  int_fast16_t includedMenuCount = 2; // start generate by a count a slow access menu position in memory EEPROM. Any of menu option start + 1 from includeQuckAccessMenu or 2


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
	lib_meniuInterface20x4_LiquidCrystal_I2C(bool& buttonUp, bool& buttonDown, bool& buttonSet) {
		buttonUP = &buttonUp; // store address of a buttons
		buttonDOWN = &buttonDown; // store address of a buttons
		buttonSET = &buttonSet; // store address of a buttons

		//func_store[5];
	}

	// if using rotary encoder " Not advanced properly right now"
	lib_meniuInterface20x4_LiquidCrystal_I2C(byte& rotarySpinedSide, bool& buttonSet) {
		rotarySpinedSIDE = &rotarySpinedSide;
		buttonSET = &buttonSet;
		isRoatry = true;

	}



public:void print__(String txt , byte line = 255 , byte column = 255) {


   if (!isMenuOpened && timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    // Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

   // Serial.println (" " + txt);
    display.print(txt); 
  }
  

}

// also works with bytes, no casting needed
public:void print__(int txt , byte line = 255 , byte column = 255) {


   if (!isMenuOpened && timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    // Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

   // Serial.println (" " + txt);
    display.print(txt); 
  }
  

}


public:void println__(int txt , byte line = 255 , byte column = 255) {


   if (!isMenuOpened && timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    // Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

   // Serial.println (" " + txt);
    display.print(txt); 
  }
  

}


public:void println__(String txt , byte line = 255 , byte column = 255) {


   if (!isMenuOpened && timerPrintAvailable)
  {
    if ( (line + column) != 510 ) {
    display.setCursor (column , line);
    // Serial.println ( "cl:" + String (column) + " ln:" + String (line ) + "=" + String (line + column));
    }

   // Serial.println (" " + txt);
    display.print(txt); 
  }
  

}

	// clear start at where was last print end.
public:void clearinDisplay__() {

	if (!isMenuOpened) {
		display.clear ();
		
	}
	}


	// access menu by long pressed a Set button
public:void IncludeFunction(void (*functionPointer)(), byte& functionValue, String functionName = "unknown", String functionValueAbbreviation = "", bool printfunctionValue = true)
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
public:void IncludeQuckAccessFunction(void (*functionPointer)(), byte& functionValue, String functionName = "unknown", String functionValueAbbreviation = "", bool printfunctionValue = true)
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
           EEPROM32_INIT();
		 // Wire.begin();
		 // Wire.setClock(800000L); // 800000L or 400000L

         display.init();
		 display.backlight();// HIGH
      	 display.setCursor(0,0);
		  
		  delay(100);
		  print ("Booting .",0,0);
     	  delay(700);
     	  print ("Booting . .",0,0);
          delay(700);
          print ("Booting . . .",0,0);
          delay (1000);
		
		
		  //
					// check if memory values not set as default
		  if (readMemoryByte(0) != true) {

			  print("MEMORY SET!",1,0);
			  EEPROM.write(0, true);
			  
			  defaultFunc();// call a default function to set a new values 
			  userSetValuesToMemory();
			  delay (4000);
		  };
		  // each startup fetch a stored values from the memory.
		  userGetValues();
	  }



	  //------------------------------------------------------------------
private:void meniuDescribeOptionDisplay(String txt) {
	display.setCursor(0, 0);

	//display.setTextSize(1);
	//display.println(txt);


	print(txt);
}


void fun_startedWaitingmeniuOptionSelected (int value = 300) 
{
	startedWaitingmeniuOptionSelected = value;
}

	   // only for a buttons/ badly on rotary encoder
private:byte meniuOptionSelectFun() {


	// if pressing a button and menu where are selected are not to big then...
	if (*buttonUP) {

		  if (meniuOptionSelected < includedMenuCount /*+ 1 <Error, out of menu bounderies*/ && meniuOptionIsPressing && !meniuOptionIsSelected)
			 ++meniuOptionSelected;
		

		fun_startedWaitingmeniuOptionSelected ();// time out start point

	}
	// if pressing a button and menu where are selected are not to small then...
	if (*buttonDOWN ) {
			
			if ( meniuOptionSelected > 1 && meniuOptionIsPressing && !meniuOptionIsSelected)
				--meniuOptionSelected;
		
		fun_startedWaitingmeniuOptionSelected ();// seconds time out start point
	}

	// Give new timeout and starting point for menu functions to work
	//if (*buttonSET /*&&  !meniuOptionIsSelected*/ ) {
		//meniuOptionIsPressing = true;
	//	fun_startedWaitingmeniuOptionSelected(); //seconds time out start point
	//}

	// if user do not do for a while , accrues a time out.								  
	if (/* !meniuOptionIsSelected && */ isMenuOpened &&  startedWaitingmeniuOptionSelected == 1 /*125seconds*/    )
	{
		// long Press Menu
		boolSetButton = false; // exit from boolSetButton statement
		boolSetMenu = false; // exit from boolSetMenu statement
		meniuOptionSelected = 1; //reset menu position to P0
		isclearedDisplayCommon = false;

		userSetValuesToMemory(); // write temperature changes to memory includeQuckAccessMenu
		display.clear();

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
	   // user when changing something in menu options, doest trow here 
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
	

	//display.set2X();
	// if allowed to print function
	//if (func_stored[includedMenuCount].printfunctionValue)

	if (func_stored[meniuOptionSelected + 1].__printfunctionValue) { // if not false to print by manualy in custom function
		//print stored values from a memory 
		//println(String (*func_stored[meniuOptionSelected + 1].__functionValueAddress) + " " ,1,0);

		//display.set1X();
		print(String (*func_stored[meniuOptionSelected + 1].__functionValueAddress) + " " + func_stored[meniuOptionSelected + 1].__functionValueAbbreviation + " ",2,0);

		//display.setCursor(0, 30);
	} // if not allowed to print 

	// print something specific  about a this function out frtom this scope 
	func_stored[meniuOptionSelected + 1].fnc_();

	// change values by spinint/pushing up or down buttons 
	userChangeMeniuValue(func_stored[meniuOptionSelected + 1].__functionValueAddress);
}

private:void menuQuckAccesPrint() {

	//display.set2X();

	//print("[-]",0,0);
	// print name of the function 
	print("[-]"+func_stored[includeQuckAccessMenu].__functionName,0,0);
	





	// if allowed to print function
	if (func_stored[includeQuckAccessMenu].__printfunctionValue) {
		//print stored values from a memory 

		//display.set2X();
		//println( String (*func_stored[includeQuckAccessMenu].__functionValueAddress) + " ",1,0);
		//display.set1X();
		print(String (*func_stored[includeQuckAccessMenu].__functionValueAddress) +" "+ func_stored[includeQuckAccessMenu].__functionValueAbbreviation,1,0);
		//var_manualMode

	} // if not allowed to print 

	// print something specific  about a this function out frtom this scope 
	// specific function to call outside this class
	func_stored[includeQuckAccessMenu].fnc_();

	//change a value 
	userChangeMeniuValue(func_stored[includeQuckAccessMenu].__functionValueAddress);

}



// Interpeter all alligment and print a manual value in custom function = .func
public: void menuPrintManuallyValue(int value , byte line = 1 , byte column = 0) {
	
	//print(meniuOptionSelected + 1);
	
	if (!func_stored[meniuOptionSelected + 1 ].__printfunctionValue) { // if allowed to print in config
		//display.set2X();
		//println(String(value) + " " ,1,5);

		//display.set1X();
		print(String(value) + " "+ func_stored[meniuOptionSelected + 1].__functionValueAbbreviation + " " ,line,column);
	}
}
	  
// Interpeter all alligment and print a manual value in custom function = .func only for quick access menu
public:void menuQuckAccesPrintManuallyValue(int value , byte line = 1 , byte column = 0) {
	
	//print(meniuOptionSelected + 1);

	if (!func_stored[includeQuckAccessMenu].__printfunctionValue) { // if allowed to print in config
		//display.set2X();
		//println(String (value) + " " +func_stored[includeQuckAccessMenu].__functionValueAbbreviation,1,0);

		print(String (value) + " " +func_stored[includeQuckAccessMenu].__functionValueAbbreviation + " " ,line,column);	

		//display.set1X();
		//print( func_stored[includeQuckAccessMenu].__functionValueAbbreviation);
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


// allows printing each second on display
public:void updates (){
		timerPrintAvailable = false;  

  if (millis() > (long)(clock_1sec + 1000) )
  {
        clock_1sec = millis();

        if (startedWaitingmeniuOptionSelected > 0)
        	--startedWaitingmeniuOptionSelected;

  }

  if (millis() > (long)(timerPrint + timerPrintOffset) )
    {
        timerPrint = millis();
        timerPrintAvailable = true;


        if (basicToggleCount == 0)
        {
	        basicToggle = !basicToggle;
	    	basicToggleCount = 9;
    	}
    		basicToggleCount--;



        if ( timePrintDelayOffset > 0) // after menu is closed , wait to print regual status screen 
        	--timePrintDelayOffset;
        else if (!isMenuOpened /*display value correctly*/)
        	 timerPrintOffset = 995UL; // and set to regular speed from 100-200UL



        if (delaySetToDefault > 0)
        	--delaySetToDefault ;
       


        if (delayOpenMenuQuickAccess > 0)
         	--delayOpenMenuQuickAccess;

		if (delayOpenMenu > 0) 
		    --delayOpenMenu; // delay to open menu when was closed after  menu long press options

		

        Serial.println (" time frame " +String (timerPrintOffset) + ",[ delayOpenMenu:" + String (delayOpenMenu) + " QuickAccess:"+ String (delayOpenMenuQuickAccess) + "] boolSetMenu:" + String (boolSetMenu) + " delayOpenMenuFirstTime:"+ String (delayOpenMenuFirstTime) +
         " meniuOptionSelectFun():" + String (meniuOptionSelectFun()) + "~"+ String (includedMenuCount) + " menu close:" + String (startedWaitingmeniuOptionSelected) + " default button:" + String (delaySetToDefaultPressingButton)  );
    };
}

public:bool InterfaceDinamic() {
	// always set display at zero state.	
	//display.setCursor(0, 0);



	if (*buttonSET)
	{
		
		if (delayOpenMenuQuickAccess > 0) // avoid exiting from quickAccesMenu while button is pressing 
		{
			delayOpenMenuQuickAccess = 3;
			timePrintDelayOffset = 3;
		}
	

		   fun_startedWaitingmeniuOptionSelected ();
	}

	if ((*buttonSET || boolSetButton) && delayOpenMenuQuickAccess == 0  /*<- work as delay to avoid open instanly after menu was closed */) {
		boolSetButton = true;
		
	 	 



		// when in menu selected option and pressing a set button to exit to common parameters. 
		if (meniuOptionIsSelected && *buttonSET) {
			isclearedDisplayCommon = false;
		}

		// Set menu for more options
		/*Is on timer when boolSetButton was passed and user must pressing a button for a 5 seconds*/
		if (*buttonSET && (millis() > (long)(startedWaiting + 5000UL) && !boolQuicklyChange) || boolSetMenu ) {
			// set loop from boolean
			if (!meniuOptionIsSelected)
				clearMenuDisplay();

			// exit from tight loops!
			// event trigger if after some time will react to button set
			// reset whole set loop
			//                                              Extra timer for not hit a first option or to work whole block!
			if ( (millis() > (long)(startedWaiting + 5000UL + 100UL)) && *buttonSET  ) 
				{

						// After meniu option was selected 
						if (meniuOptionIsSelected && /*default function fix to dont crash */ !(defaultIsFunc && meniuOptionSelectFun() == includedMenuCount - 1)) {
							userSetValuesToMemory(meniuOptionSelectFun() + 1);// start common menu options addresing in memory from 2, skipping 1 place.


							//display.set1X();
							if (meniuOptionSelectFun() < includedMenuCount) // only display meniu option values exept on 'EXIT'
								print("?P" + String(meniuOptionSelectFun()) + "?",0,0); // while pressing 



							//delay(60);
						} 

						//if ( *buttonSET && !isCompletedChangeValueToExitFromSelectedOption )  // entering menu
						//{
						//	isCompletedChangeValueToExitFromSelectedOption = true; 
						//}

						


						// Works as delay to  whole loop of loong press menu
						
						// When set button is while pressing , execute this branch
						if ( delayOpenMenu == 0  && delayOpenMenuFirstTime ) // wait until user releas Set button . Countdown until reach a 0 and before that 1 gives unique delay after set button was realesed 5 point before 
						{// 
							meniuOptionIsSelected = !meniuOptionIsSelected; // enter to menu option
						}

						delayOpenMenu = 3 ;// contantly eveluating 3 until button is realese , works as timer or timeout to user be available to press
						delayOpenMenuFirstTime = true; // when user entered in long press menu and hit here for a first time 
						delaySetToDefaultPressingButton = 0; // default timing to work proprietary

						// EXIT----------------------------------------------------------------
						// Exit from progra  and main boolSetButton loop...if meniu not selected
						if (meniuOptionSelectFun() == includedMenuCount) { // Exit

							boolSetButton = false; // exit from boolSetButton statement
							boolSetMenu = false; // exit from boolSetMenu statement
							meniuOptionSelected = 1; //reset meniu position to P0
							meniuOptionIsSelected = false; // when user exit from meniu set to default 
							isclearedDisplayCommon = false;
							
							timePrintDelayOffset = 4;
							delayOpenMenuQuickAccess  = 7;
							boolSetMenu = false;
							boolQuicklyChange = false;
							isclearedDisplayCommon = false; // reset a clearing system
							
							display.clear();
							display.setCursor(0, 0);
							//timerPrintOffset = 998UL;
						
						}



				//delay(130);// wait until set button not realesed


			}
			else
			{
				// set whole set loop by default if no buttons event accures
				boolSetMenu = true;
				boolSetButton = true;
				// now working , always on delayOpenMenu = 5; // delay to open menu when was closed after  menu long press options

			}

			//progra-------------------------------------------------------
			if ((*buttonSET || boolSetMenu) ) {

				/*if (*buttonSET && delayOpenMenuCompleted)
				{
					meniuOptionIsSelected = false;
					delayOpenMenuCompleted = false;
					delay (200);
				}

				if (!*buttonSET && delayOpenMenuCompleted)
				{
						//delayOpenMenuCompleted = false;
						//meniuOptionIsSelected = false;

				}

			*/

				//display.setCursor(0, 0);
				//display.set1X();
				if (meniuOptionSelectFun() < includedMenuCount) // only display meniu option values exept on 'EXIT'
					
					

					if (meniuOptionIsSelected && /*print fix not flicker*/ !(defaultIsFunc && meniuOptionSelectFun() == includedMenuCount - 1))
						if (basicToggle) {
								if (basicToggleCount < 6) // to see ?P1?
								print(" P" + String(meniuOptionSelectFun()) + " ",0,0);
						}
						else
							print(">P" + String(meniuOptionSelectFun()) + "<",0,0);
					else
							print("[P" + String(meniuOptionSelectFun()) + "]",0,0);


				// if meniu option not selected print about meniu abbreviation

				if (!meniuOptionWhenSelected) {
					//print("-");
					//if ( !(defaultIsFunc && meniuOptionSelectFun() == includedMenuCount - 1) )
					//	print(" to default " ,0 , 5);



				}
				// if not meniu option selected and meniu are not at EXIT point
				if (!meniuOptionIsSelected && meniuOptionSelectFun() < includedMenuCount) {
					meniuOptionWhenSelected = true;

					//print(" MENU" ,0 ,5);//display.println("MENIU"); 
					

				}
				else {

					meniuOptionWhenSelected = false;
				}



				//Set to Default
				//check if option is available
				if (defaultIsFunc && meniuOptionSelectFun() == includedMenuCount - 1 )  // set to default 
				{ 
					
					print("Reset to default" , 0 ,0);

					 

					 if (meniuOptionIsSelected) 
					 {
					 	
					 	 

								 
					 			switch(delaySetToDefault) 
					 	{
						  case 0:
							   		   print("Press UP"  ,1,0); // default print while at zero 
							    		break;
						  case 1:
						  				print(SPACE ,1,0); // clear "Done"
								 		meniuOptionIsSelected = !meniuOptionIsSelected; // back to meniu options
								 		delaySetToDefaultPressingButton = 0;
						    			break;

						    case 2 ... 38: 
						    			 
								 		 defaultFunc();
										 userSetValuesToMemory();
						    			 print(" Done           "  ,1,0);
						   				 break;	 

						}



								    if (delaySetToDefaultPressingButton > 0 && delaySetToDefaultPressingButton < 37)
								    
											 	print (String (map(delaySetToDefaultPressingButton, 0, 38, 0, 100)) + "%" , 1 ,9 );


								 	if (*buttonUP && delaySetToDefault == 0 )
								 	{
										
									 

										 		if ( delaySetToDefaultPressingButton > 40)
										 		 	{
														delaySetToDefault = 40;	
										 		    }
										 		else 
										 			{	
								 	 					delaySetToDefaultPressingButton++;

										 			}

									 				

									  			//if (delaySetToDefaultPressingButton <= 40)
								 	 			//		 delaySetToDefaultPressingButton++;

								 	} 

								 	//if (delaySetToDefaultPressingButton > 0 && !*buttonSET)
										//--delaySetToDefaultPressingButton;
						
					   



					 }


					 

				}
				else {
					// ----------------------do rest of options------------------------------------




					// print about  menu  what it's doing
					print(func_stored[meniuOptionSelectFun() + 1].__functionName,0,5); // + 1 to avoid to stuble a Quck access menu option


					// if user select a desire function, then change his value
					if (meniuOptionIsSelected /*&& !isCompletedChangeValueToExitFromSelectedOption*/) {



						//print menu
						menuSelectedPrint();
					}



				}



				//  EXIT ONLY PRINT-------------------------------------
				if (meniuOptionSelectFun() >= includedMenuCount) { // Exit
					//display.setCursor(0, 0);

					//display.set2X();
					print("  [EXIT]      " ,0,0);
					print(SPACE ,1,0);
				}



				// clear [EXIT] stuff left on screen
			   // if (meniuOptionSelectFun() == includedMenuCount - 1)




				//display.set1X();
			}


		}





		//-----------------// Quickly Access/change single option-----------------------------------------
		else if ( *buttonSET == LOW || boolQuicklyChange) {
			// set loop from boolean



			// exit from tight loops!
			// event trigger if after some time will react to button set
			if ( /*Wait while user realesing a button*/(millis() > (long)(startedWaiting + 400UL))  && *buttonSET || startedWaitingmeniuOptionSelected  == 0 /* || or timeout for 45s(millis() > (long)(startedWaiting + 45100UL)) */ ) {
				// reset whole set loop
				boolQuicklyChange = false;
				boolSetButton = false;
				isclearedDisplayCommon = false;
				userSetValuesToMemory(); // write temperature changes to memory includeQuckAccessMenu
				display.clear();
				//timerPrintOffset = 997UL;
				timePrintDelayOffset = 5;
				delayOpenMenuQuickAccess = 5 ; 
			
				//delay(100);

				
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
		boolQuicklyChange = false; //activate condition ones until event is triggered
		boolSetMenu = false; //activate condition ones until event is triggered // set default to menu option
		//display.clear(); // blinking...
	}


	if (boolSetMenu || boolQuicklyChange) { // when exit from any option, reset a clearing display ones a  boolean.
		isMenuOpened = true;
		timerPrintOffset = 200UL;
	}
	else 
	{
		isMenuOpened = false;
		delayOpenMenuFirstTime = false;
		//timerPrintOffset = 1000UL;
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
			//Serial.println(String(count) + ",addr:" + String(func_stored[count].__funcAddress) + "val:, " + String(*func_stored[count].__functionValueAddress));
		}

	}
	//delay(11000);
};



	  // address automatically by 1 byte each time 
public:void userGetValues() {
	// plus quick access function
	for (int count = 1; count < (includedMenuCount); count++) {

		byte val = readMemoryByte(count + includeStartingPointMem); //save all values to memory EEPROM

		*func_stored[count].__functionValueAddress = val;

		Serial.println("userGetValues: adr ["+String (count + includeStartingPointMem)+"] val:"+String(*func_stored[count].__functionValueAddress));
	}
	//delay(5000);
};



	  // print bare bone information





	  void displayStoredMemoryValues() {
		  for (int count = 1; count < (includedMenuCount); count++) {

			  byte val = readMemoryByte(count + includeStartingPointMem); //save all values to memory EEPROM 

			  this->print__(func_stored[count].__functionName.substring(0, 8) + "," + func_stored[count].__printfunctionValue + ":" + String(*func_stored[count].__functionValueAddress) + " " + func_stored[count].__functionValueAbbreviation);
		  }
	  }

	  void displayButtonsValue() {

		  this->print__ ("btnSET:" + String(*buttonSET)+" UP:" + String(*buttonUP)+" DOWN:" + String(*buttonDOWN),0,0); // value of the pointer

		 // Serial.print(("buttonSET:" + String(*buttonSET)));
		 // Serial.print(("buttonUP:" + String(*buttonUP)));
		 // Serial.print(("buttonDOWN:" + String(*buttonDOWN)));
	  };


	  void displayRotaryValues() {
		  this->print__("buttonSET:" + String(*buttonSET),0,0); // value of the pointer
		  this->print__("rotarySpinedSIDE:" + String(*rotarySpinedSIDE),1,0); // value of the pointer
		  
		  
	  }
};





























