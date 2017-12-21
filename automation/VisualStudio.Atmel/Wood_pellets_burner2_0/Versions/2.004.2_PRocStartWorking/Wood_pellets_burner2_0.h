#include <stdlib.h>
#include <stddef.h>
#include "ThermoSencor.h"
#include "ManageReadWrite8bitEEPROM.h"
#include "menuLiquidCrystal.h"
#include "functions.h"


ThermoSencor Temperature(A3);

int8_t FANPIN = 10; // ~
int8_t PELLETPUSHERPIN = 11; // ~
int8_t TEMPSENSORPIN = A3;

int8_t OneSec = 10; // times in loop until reach one secunde

controls LCDLIGHT(0, 1);//address
						// Celcius
controls TEMPMIN(1, 17); // default 
controls TEMPMAX(2, 23); //address
						 // Fan
controls FANMINSPEED(3, 90); //address
controls FANMAXSPEED(4, 150); //address

controls FANSECONDSHOLD(5, 40); //Hold time after fast shift , that make to burn last cycle dropet pellets
int16_t FANHOLDTIMEOUT_ON = 0;
int16_t FANSPEEDRUN = 0;
bool FANHOLDENABLE_ON = false;
bool FANFIRESTARTTIMERENABLE = true;


int16_t FANFIRESTARTTIME;// = 100 * OneSec; //seconds 600
int16_t FANFIRESTARTTIMEOUT= FANSECONDSHOLD.getValue() * OneSec; //startup first time in rise fan 
float FANSPEEDRUNFLOAT;
float FANSPEEDRUNFLOATSUM;


// Pellets  



controls PELLETPUSHERMODE(7, 1); // Select Mode 1)Only Timer, 2)By Temp. Regulating Min or Max Power, 3)Power Regulating By Temp. Bitween Min or Max in Procentage Range

controls PELLETPUSHERMILLISECONDSON(8, 10); //address //  push time until sleep time + seconds
controls PELLETPUSHERMINSPEED(9, 20);
//

// Componets regulation
int16_t COMPONENTSTIMEOUT_ON = 0; //time counters
int16_t COMPONENTSTIMEOUT_OFF = 0; //time counters

							   // By Desision Stored Values 
int16_t COMPONENTSTDESISION_ON = 0; //time counters
int16_t COMPONENTSTDESISION_OFF = 0; //time counters


controls COMPONENTSMINSECONDS(10, 100); // Responsible for Pellet Pusher and Fan Working Time   
controls COMPONENTSMAXSECONDS(11, 140); // Responsible for Pellet Pusher and Fan Working Time

						 

int16_t LowHightProcetange_value = -3;
int16_t procRatio;
int16_t calculatedRatioProc = 0;
int16_t procRatioStaticCalculation = 0;
int16_t COMPONENTSTIMEOUT_OFF_Static = 0;

bool ScreenStatusDisplay = false;

void init_memory_defaults(bool conditiondefault = false) {

	LCDLIGHT.setDataDefault();


	// Temperature 
	TEMPMIN.setDataDefault();
	TEMPMAX.setDataDefault();


	//Fan
	FANSECONDSHOLD.setDataDefault(); // 

	FANMINSPEED.setDataDefault();
	FANMAXSPEED.setDataDefault();



	// Pellet              
	PELLETPUSHERMODE.setDataDefault();
	PELLETPUSHERMILLISECONDSON.setDataDefault();
	PELLETPUSHERMINSPEED.setDataDefault();

	// Control Components Like Fan And Pellet Pusher
	COMPONENTSMINSECONDS.setDataDefault();
	COMPONENTSMAXSECONDS.setDataDefault();

}





void dummyFunc() {};

// START menu functions
void funTEMP() {

	switch (PELLETPUSHERMODE.getValue())
	{
	case 1:
		//  printMenuFunc("Min temperature ",&TEMPMIN,"C*:"); //
		sey("Nera naudojama", "temperaturos");
		break;
	case 2:
		printMenuFunc("Max temperature ", &TEMPMAX, "C*:"); // 
		break;
	case 3:
		printMenuFunc("Max temperature", &TEMPMAX, "C* ");
		printMenuFunc("Min temperature ", &TEMPMIN, "C*:", &dummyFunc, false, "Less", TEMPMAX.getValue()-1); // 
		break;
	default:
		sey("Nera Nustatyta", "Veikimo Budo!");
		break;
	}


};
//////////////////

void __FANMINSPEEDMIN() { analogWrite(FANPIN, FANMINSPEED.getValue()); };
void __FANMINSPEEDMAX() { analogWrite(FANPIN, FANMAXSPEED.getValue()); };
void funFAN() {
	analogWrite(PELLETPUSHERPIN, 0); //Disable Pellet Pusher 

	printMenuFunc("Max. Fan RPM", &FANMAXSPEED, "RPM:", &dummyFunc);
	printMenuFunc("Min. Fan RPM ", &FANMINSPEED, "RPM:", &__FANMINSPEEDMIN, false, "Less", FANMAXSPEED.getValue()-1); //
	printMenuFunc("Fan Hold/Rise", &FANSECONDSHOLD, "Sec:");
	sey();
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
void __PELLETPUSH() { analogWrite(PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); };
void funPelletModeOnlyTimer()
{
	analogWrite(PELLETPUSHERPIN, 0); // disable pellet pusher 
	printMenuFunc("Gran. Greitis", &PELLETPUSHERMINSPEED, "RPM:");
	printMenuFunc("Gran.Veik.mlsc", &PELLETPUSHERMILLISECONDSON, "millisec:");
	printMenuFunc("Gran.Neveik.Sec", &COMPONENTSMINSECONDS, "secundes:");


	PELLETPUSHERMODE.writeValue(1); // Set to state
	sey();

};

void funPelletModeTempMinOrMax()
{
	analogWrite(PELLETPUSHERPIN, 0); // disable pellet pusher 
	printMenuFunc("Gran. Greitis", &PELLETPUSHERMINSPEED, "RPM:");
	printMenuFunc("Gran.Veik.mlsc", &PELLETPUSHERMILLISECONDSON, "millisec:");
	printMenuFunc("Fan.Hold/Rise", &FANSECONDSHOLD, "sec:"); // Delay of Keep  Turn On Fan sum While

	printMenuFunc("Darbo laikas ", &COMPONENTSMAXSECONDS, "Maximum Sec:"); // 
	printMenuFunc("Darbo laikas ", &COMPONENTSMINSECONDS, "Minimum Sec:", &dummyFunc, false, "Less", COMPONENTSMAXSECONDS.getValue()-1); // 

	printMenuFunc("Fano Greitis", &FANMAXSPEED, "Maximum RPM:");
	printMenuFunc("Fano Greitis", &FANMINSPEED, "Minimum RPM:", &dummyFunc, false, "Less", FANMAXSPEED.getValue()-1);

	printMenuFunc("Temperatura", &TEMPMAX, "Maximum C*:");




	PELLETPUSHERMODE.writeValue(2);
	sey("Gran. Min or Max");
};

void funPelletModeTempBetweenMinMaxProcentage()

{
	analogWrite(PELLETPUSHERPIN, 0); // disable pellet pusher 

	printMenuFunc("Gran. Greitis", &PELLETPUSHERMINSPEED, "RPM:");
	printMenuFunc("Gran.Veik.mlsc", &PELLETPUSHERMILLISECONDSON, "millisec:");

	printMenuFunc("Darbo laikas ", &COMPONENTSMAXSECONDS, "Maximum Sec:"); // 
	printMenuFunc("Darbo laikas ", &COMPONENTSMINSECONDS, "Minimum Sec:", &dummyFunc, false, "Less", COMPONENTSMAXSECONDS.getValue()-1); // 

	printMenuFunc("Fano Greitis", &FANMAXSPEED, "Maximum RPM:");
	printMenuFunc("Fano Greitis", &FANMINSPEED, "Minimum RPM:", &dummyFunc, false, "Less", FANMAXSPEED.getValue()-1);

	printMenuFunc("Temperatura", &TEMPMAX, "Maximum C*:");
	printMenuFunc("Temperatura", &TEMPMIN, "Minimum C*:", &dummyFunc, false, "Less", TEMPMAX.getValue()-1);
	//printMenuFunc (String text , controls* EEPROM  ,String n="", void (*functionPointer)() = myF  , bool testmode = false , String valueGreaterOrLessCondition = "Less" ,int valueGreaterOrLess=-1) {

	PELLETPUSHERMODE.writeValue(3);
	sey("Gran. Procentage");
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

/////////////////
void __funLCDLIGHT() {    // Sub Function 
	analogWrite(PELLETPUSHERPIN, 0); // Disable Pellet Pusher
	if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW);
};

void funLCDLIGHT() {
	analogWrite(PELLETPUSHERPIN, 0); // Disable Pellet Pusher
	printMenuFunc("LCD Sviesa", &LCDLIGHT, "on/off:", __funLCDLIGHT);
};
//
//void funTestingComponents() { // temporery loaded value that not changed can be tested with commands
//	printMenuFunc("Gran. Greitis", &PELLETPUSHERMINSPEED, "TEST-RPM:", __PELLETPUSH, true);
//	//         printMenuFunc("Max. Fan RPM",&FANMINSPEED,"TEST-RPM:",__FANMINSPEED,true);
//}


void funSettoDefault() {
	analogWrite(PELLETPUSHERPIN, 0); // disable pellet pusher 

	if (ArgueAgree())
	{
		lcd.clear();
		lcd.setCursor(0, 1);
		lcd.print("Default was");
		lcd.setCursor(0, 0);
		for (int x = 0; x < 16; x++) //animation arrow
		{

			delay(100);
			lcd.print(">");
			//                      
		}
		lcd.setCursor(0, 1);
		lcd.print("Default was set");
		delay(200);
		init_memory_defaults(true);
		delay(2000);
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0, 1);
		delay(300);
		lcd.print("Ok");
		delay(800);
		lcd.setCursor(0, 0);
		lcd.print("Default Not Set");
		delay(1000);
	}

};

void funExit() {
	ScreenStatusDisplay = false;
};

// END menu functions


menuLiquidCrystal menu[8]; // alway give exact size of menu
menuLiquidCrystalNavigate navmenu;

// load into menu external functions
void initiate_menu_functions() {

	// include menu objects
	menu[0].IncludeFunction(&funTEMP, "Temperatura");
	menu[1].IncludeFunction(&funFAN, "Oro Put. Fenas");
	menu[2].IncludeFunction(&funPelletModeOnlyTimer, "1-Veikimo Budas", "Pagal Laika");
	menu[3].IncludeFunction(&funPelletModeTempMinOrMax, "2-Veikimo Budas", "Temp.Min - Max");
	menu[4].IncludeFunction(&funPelletModeTempBetweenMinMaxProcentage, "3-Veikimo Budas", "Temp.Min % Max");
	menu[5].IncludeFunction(&funLCDLIGHT, "Sviesa", "On or Off");
	//menu[6].IncludeFunction(&funTestingComponents, "Testavimas");

	menu[6].IncludeFunction(&funSettoDefault, "Set to Default", "Gamik.Parametrai");

	menu[7].IncludeFunction(&funExit, "Iseiti");

	//total menu available
	navmenu.setmenuLenght(sizeof(menu) / sizeof(menu[0])); // find out about size 
}

// when program loaded newly .
void initiate_updatePins(bool print = true) {
	if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW);

	


	//     if (print) 
	//      {
	//      Serial.println ("FANMINSPEED:" + String (FANMINSPEED.getValue()));
	//      Serial.println ("PELLETPUSHERMINSPEED:" + String (PELLETPUSHERMINSPEED.getValue()));
	//      Serial.println ("TEMPMAX:" + String (TEMPMAX.getValue()));
	//    }
}

// Update Pins 

void initControlPins() {
	//  analogWrite (FANPIN, FANMINSPEED.getValue()); 
	//////////analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); Negali buti naudojamas be logikos isikisimo
	__funLCDLIGHT(); // Update Background Light

}

const static  int pusher = 10;
const static  int wind = 11;

//b1(12) set, b2(13) down; b3(A0) up
//buttons
////////////////////
//Program Variables




void setup() {
	lcd.begin(16, 2);



	// put your setup code here, to run once:
	pinMode(pusher, OUTPUT);
	pinMode(wind, OUTPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_UP, INPUT);
	// Motors
	pinMode(FANPIN, OUTPUT);
	pinMode(PELLETPUSHERPIN, OUTPUT);
	// Temp. Sensor
	pinMode(TEMPSENSORPIN, INPUT);

	//   Serial.begin(9600);
	//   Serial.println ("Load Complete: " + String  ("Structs array") + sizeof (menu) + String (",Menu [0] Size") + sizeof (menu[0]));
	delay(500);

	initiate_menu_functions();
	initiate_updatePins();

	if (EEPROM.read(1000) >= 255) { // if first time are loded to chip when set sum defaults 
		init_memory_defaults();
		EEPROM.write(1000, 0);
	}
}





void printmenu() {
	int8_t menuselected = navmenu.getMenuSelected();
	lcd.clear();
	lcd.print(String(menuselected + 1) + String(")") + menu[menuselected].functionName);
	lcd.setCursor(0, 1);
	if (menu[navmenu.getMenuSelected()].isEmptyFunctionValue()) // ignore unknown value , no dinamic update support :<
		lcd.print(menu[navmenu.getMenuSelected()].functionValue);
};


int8_t printstatuscounter = 0;
int8_t printstatustimer = -1;
int c = 0;
float val;
void printstatus(bool print = false) {



	//       c--;
	//    if (c <=0) {
	//////        val = analogRead(TEMPSENSORPIN)/25.7;
	//        val=Temperature.RawValueUpdate () /25.7;
	//        delay(20);
	//        c=10;
	//      }
	lcd.clear();

	switch (printstatuscounter)
	{
	case 1:
		lcd.print("Gran.stumiklis");
		lcd.setCursor(0, 1);

		if (PELLETPUSHERMODE.getValue() == 1) // by Timer
		{
			if (COMPONENTSTIMEOUT_ON != -1)
				lcd.print("Clock>on:" + String(COMPONENTSTIMEOUT_ON));
			else
				lcd.print("Clock>off:" + String(COMPONENTSTIMEOUT_OFF));
		}
		else if (PELLETPUSHERMODE.getValue() == 2) //Bitween Hight or Low , Controled by Temperature
		{

			if (COMPONENTSTIMEOUT_ON != -1)

				if (LowHightProcetange_value == -1) // Hight = -1, Low = -2;
					lcd.print("-HIGHT>on:" + String(COMPONENTSTIMEOUT_ON));
				else
					lcd.print("LOW->on:" + String(COMPONENTSTIMEOUT_ON));
			else

				if (LowHightProcetange_value == -1) // Hight = -1, Low = -2;
					lcd.print("-HIGHT>off:" + String(COMPONENTSTIMEOUT_OFF));
				else
					lcd.print("LOW->off:" + String(COMPONENTSTIMEOUT_OFF));

		}

		else if (PELLETPUSHERMODE.getValue() == 3)// Bitween  Hight or Low using Sum of Procentage Distage , Controled by Temperature
		{
			//int valProc = 100 - ((COMPONENTSTIMEOUT_OFF_Static * 10) / COMPONENTSMAXSECONDS.getValue());

			if (COMPONENTSTIMEOUT_ON != -1)
				lcd.print("-" + String(calculatedRatioProc) + "%>on:" +  String(COMPONENTSTIMEOUT_ON));
			else
				lcd.print("-" + String(calculatedRatioProc) + "%>off:" + String(COMPONENTSTIMEOUT_OFF));

		}
		else
		{
			lcd.print("Isjungta.");
		}




		break;
	case 2:
		lcd.print("Oro Put. Fenas");////////////////////////////////////////////
		lcd.setCursor(0, 1);
		switch (PELLETPUSHERMODE.getValue())
		{
		case 1:
			lcd.print("RPM:" + String(FANSPEEDRUN));
			break;
		case 2:

			
			
			;

			     if (FANFIRESTARTTIMEOUT > 0)
					 lcd.print("RPM:" + String(FANSPEEDRUN) + " s:"+String(FANFIRESTARTTIMEOUT));
			else if (FANSPEEDRUN == FANMAXSPEED.getValue())
					lcd.print("max RPM:" + String(FANSPEEDRUN) );
			else
					lcd.print("min RPM:" + String(FANSPEEDRUN) );


		

			break;
		case 3:
			int valProc = (FANSPEEDRUN * 100) / FANMAXSPEED.getValue();
			//                              100 - ((COMPONENTSTIMEOUT_OFF_Static * 10 ) / COMPONENTSMAXSECONDS.getValue())
			//                             lcd.print(String( calculatedRatioProc )+"%RPM:"+ String (FANSPEEDRUN));
			//                           lcd.print(String( valProc )+"%"+String(calculatedRatioProc)+"%?RPM:"+ String (FANSPEEDRUN));
			lcd.print(String(valProc) + "%RPM:" + String(FANSPEEDRUN));

			break;
		}


		if (FANHOLDTIMEOUT_ON > 0)
			lcd.print(",H:" + String(FANHOLDTIMEOUT_ON));
		break;
	case 3:
		lcd.print("Temp. C*:" + String(Temperature.Temperature));
		lcd.setCursor(0, 1);
		switch (PELLETPUSHERMODE.getValue())
		{
		case 1:
			lcd.print("---");
			break;
		case 2:
			lcd.print("C* max:" + String(TEMPMAX.getValue()));    // Temperature.Temperature
			break;
		case 3:
			lcd.print("C* min:" + String(TEMPMIN.getValue()) + " max:" + String(TEMPMAX.getValue()));    // Temperature.Temperature
			break;
		}

		break;
	case 4:
		//
		//                   1-Veikimo Budas","Pagal Laika");
		//   menu[3].IncludeFunction(&funPelletModeTempMinOrMax,"2-Veikimo Budas","Temp.Min - Max");
		//   menu[4].IncludeFunction(&funPelletModeTempBetweenMinMaxProcentage,"3-Veikimo Budas","Temp.Min % Max") ;

		//                      lcd.print("WTFACK");
		//                      lcd.setCursor(0,1);
		//                      lcd.print("WTFACKdick");





		switch (PELLETPUSHERMODE.getValue())
		{
		case 1:
			lcd.print("1-Veikimo Budas");
			lcd.setCursor(0, 1);
			lcd.print("Pagal Laika");
			break;

		case 2:
			lcd.print("2-Veikimo Budas");
			lcd.setCursor(0, 1);
			lcd.print("Temp.Min - Max");
			break;

		case 3:
			lcd.print("3-Veikimo Budas");
			lcd.setCursor(0, 1);
			lcd.print("Temp.Min % Max");
			break;
		default:
			lcd.print("Veikimo Budas");
			lcd.print("Nust.Nepavyko");
			break;
		}
		break;
	case 5:
		switch (PELLETPUSHERMODE.getValue())
		{
		case 1:
			lcd.print("Galingumas");
			lcd.setCursor(0, 1);
			lcd.print("Pagal Laika");
			break;

		case 2:
			lcd.print("Galingumas");
			lcd.setCursor(0, 1);
			if (LowHightProcetange_value == -1) // Hight = -1, Low = -2;
				lcd.print("H.Daugiau Kuro.");
			else
				lcd.print("L.Maziau Kuro.");

			break;

		case 3:

			lcd.print("Naudojama");
			lcd.setCursor(0, 1);
			lcd.print("-"+String(calculatedRatioProc) + "% kuro.");//raw:"+String(procRatio)+"%");




			break;
		default:
			lcd.print("Veikimo Budas");
			lcd.print("Nust.Nepavyko");
			break;
		}
		break;

	}




	if (printstatuscounter > 5) //how much status blocks in switch case 
		printstatuscounter = 1;

	// Timer on
	if (printstatustimer == -1 || print) {
		printstatustimer = 70; // 7 sec
		printstatuscounter++;
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int exitmenutime = 30 * OneSec;//ms
int exitmenutimer = exitmenutime;
int c1Sec = 0;
void loop() {

	//  c1Sec--;
	//  if (c1Sec <= 0 )//delay shift between temp function and just time hold function
	//  { 
	//    c1Sec = 10; 
	//     Temperature.RawValueUpdate (); // Measure Temperature 100 ms Hold
	//  }else
	//  {
	//    delay (100);
	//  }

	Temperature.InCustomTimeAverageUpdate(600);

	delay(90);

	initControlPins(); // update output pins


					   //delay (90);
	if (COMPONENTSTIMEOUT_ON > -1)COMPONENTSTIMEOUT_ON--;
	if (COMPONENTSTIMEOUT_OFF > -1)COMPONENTSTIMEOUT_OFF--;
	if (FANHOLDTIMEOUT_ON > -1) FANHOLDTIMEOUT_ON--;

	if (printstatustimer > -1) printstatustimer--;
	if (FANFIRESTARTTIMEOUT > -1) { FANFIRESTARTTIMEOUT--; };






	int8_t __set = digitalRead(BUTTON_SET);
	int8_t __up = digitalRead(BUTTON_UP);
	int8_t __down = digitalRead(BUTTON_DOWN);


	//     Serial.println("__set:"+ String(__set) + ",__up:"+ String(__up) + ",__down:" + String(__down)  );



	//PELLETPUSHER


	switch (PELLETPUSHERMODE.getValue())
	{


	case 1:



		COMPONENTSTDESISION_OFF = COMPONENTSMINSECONDS.getValue() * OneSec; // min = 60 seconds  + custom seconds           
		FANSPEEDRUN = FANMINSPEED.getValue(); // default
		break;//////////////////////////////////////////////////////////////////////////////////////////////////////
	case 2: // Temp.Low-Hight" // use temperature to shift between low or hight power condition

		if (Temperature.Temperature < TEMPMAX.getValue()) {
			COMPONENTSTDESISION_OFF = COMPONENTSMINSECONDS.getValue()* OneSec; // if not worm enough when reduce Time



			if (FANHOLDENABLE_ON == false) // if fan was started from LOW mode then try increase speed very slowly
			{

				//FANFIRESTARTTIMERENABLE = false;
				FANFIRESTARTTIMEOUT = FANSECONDSHOLD.getValue() * OneSec; //FANFIRESTARTTIME; // react if burn cycle is coplete

				FANSPEEDRUNFLOATSUM = float(FANMAXSPEED.getValue() - FANMINSPEED.getValue()) / FANFIRESTARTTIMEOUT;

				FANSPEEDRUNFLOAT = FANMINSPEED.getValue(); // set to minimum , later increases fan speed 
			}

			if (FANFIRESTARTTIMEOUT > 0 )// if true to slow start fan
			{
				FANSPEEDRUNFLOAT = FANSPEEDRUNFLOAT + FANSPEEDRUNFLOATSUM; // Minimum Speed plus each bit divided from given time 
				FANSPEEDRUN = FANSPEEDRUNFLOAT;
			}
			else
			{
				FANSPEEDRUN = FANMAXSPEED.getValue(); // if time out slow ignite mode then use this 
				//FANFIRESTARTTIMERENABLE = false;
				
			}

			FANHOLDENABLE_ON = true;
			LowHightProcetange_value = -1; // Hight                    

		}
		else {
			COMPONENTSTDESISION_OFF = COMPONENTSMAXSECONDS.getValue()* OneSec; // if  enough warm when Increase Time

			if (FANHOLDENABLE_ON) { // turn on fan max speed 
				FANHOLDTIMEOUT_ON = FANSECONDSHOLD.getValue()* OneSec;
				FANHOLDENABLE_ON = false;
				 // set value FANFIRESTARTTIMEOUT to kick fan each time 
			}
			LowHightProcetange_value = -2; // Low show condition                   
			FANSPEEDRUN = FANMINSPEED.getValue();
		}

		break;//////////////////////////////////////////////////////////////////////////////////////////////////////
	case 3: //Temp.Low%Hight // control in low hight range with sum of procenatge from min max temperature range


		//if (Temperature.Temperature <= TEMPMIN.getValue()) // if current temp. is less then minimum  temp then set everything to 100% Heat power
			//procRatio = 100;

		//else //Temperature is more then minimum temp , can reduse power in procentage
			procRatio = (100 * ((Temperature.Temperature - TEMPMIN.getValue()) / (TEMPMAX.getValue() - TEMPMIN.getValue())));


		if (procRatio > 100) procRatio = 100; //Protection from wtf logic failure 
		
		if (Temperature.Temperature < TEMPMIN.getValue())
		{
			procRatio = 100;
		}
		else if (Temperature.Temperature > TEMPMAX.getValue())
		{
			procRatio = 0;
		}

		

		

		if (COMPONENTSTDESISION_OFF < (COMPONENTSMINSECONDS.getValue() * OneSec))  COMPONENTSTDESISION_OFF = COMPONENTSMINSECONDS.getValue() * OneSec; // Set to minimum
		if (COMPONENTSTIMEOUT_OFF_Static == 0) { // later use to show  procentage  // also set to default first time in the program
			COMPONENTSTIMEOUT_OFF_Static = COMPONENTSTDESISION_OFF;
			
			calculatedRatioProc = 100 - ((COMPONENTSTIMEOUT_OFF_Static * OneSec) / COMPONENTSMAXSECONDS.getValue());

			FANSPEEDRUN = (FANMAXSPEED.getValue()* procRatio) / 100;
			if (FANSPEEDRUN < FANMINSPEED.getValue()) // fan speed from procentage is lower then minimum posible value , do set minimum value 
				FANSPEEDRUN = FANMINSPEED.getValue();

		}


		COMPONENTSTDESISION_OFF = (float(COMPONENTSMAXSECONDS.getValue()* OneSec) / 100) * calculatedRatioProc; //set to dinamic procentage of power to work


		break;/////////////////////////////////////////////////////////////////////////////////////////////////////
	default:
		break;
	}

	COMPONENTSTDESISION_ON = PELLETPUSHERMILLISECONDSON.getValue(); // min = 60 seconds + custom seconds 



//OFF																	// Fan And Pellet Time Counter
	if (COMPONENTSTIMEOUT_ON == -1 && COMPONENTSTIMEOUT_OFF == -1) // before ON_TIMEOUT become -1 , zero give window to step up a turn of mode enable
	{

		switch (PELLETPUSHERMODE.getValue())
		{
		case 1:

			break;
		case 2:
			
			//FANFIRESTARTTIMERENABLE = true;
			break;
		case 3:
			COMPONENTSTIMEOUT_OFF_Static = COMPONENTSTDESISION_OFF;

			calculatedRatioProc = 100 - ((COMPONENTSTIMEOUT_OFF_Static * OneSec) / COMPONENTSMAXSECONDS.getValue());

			FANSPEEDRUN = (FANMAXSPEED.getValue()* procRatio) / 100;

			if (FANSPEEDRUN < FANMINSPEED.getValue()) // fan speed from procentage is lower then minimum posible value , do set minimum value 
				FANSPEEDRUN = FANMINSPEED.getValue();
			break;
		default:
			break;
		}




		COMPONENTSTIMEOUT_OFF = COMPONENTSTDESISION_OFF; // final setup
	}






	/////////////////////// ON 
	if (COMPONENTSTIMEOUT_ON == -1 && COMPONENTSTIMEOUT_OFF == 0) // give beggining and turn on pellet pusher
		COMPONENTSTIMEOUT_ON = COMPONENTSTDESISION_ON;

	// Pellet

	if (COMPONENTSTIMEOUT_ON > -1) // execute rutine   
								   //ON
		analogWrite(PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); //Give speed/power to motor
	else // OFF
		analogWrite(PELLETPUSHERPIN, 0); //Give speed/power to motor  

										 // Fan Delay Timer


	if (FANHOLDTIMEOUT_ON > 0)
		analogWrite(FANPIN, FANMAXSPEED.getValue());
	else
		analogWrite(FANPIN, FANSPEEDRUN);

	// Automatiskai iseinti pagal laika is option menu

	// to Those What to print 
	if (ScreenStatusDisplay) {
		printmenu();

		// exit timer 
		if (exitmenutimer > 0)
			exitmenutimer--;
		else
			funExit(); // automaticly exit after sum time from manual seletion menu

	}
	else {
		printstatus();
	}




	if (__up) {//  Serial.print ( "getMenuSelected:" + String (navmenu.getMenuSelected ()) + ",getMenuLenght:" + String (navmenu.getMenuLenght()) );
			   //        FANMINSPEED.addValue();
		navmenu.menuUp();
		printmenu();
		// Wait button realess
		buttonRelease(BUTTON_UP);
		exitmenutimer = exitmenutime; // update each time auto menu exit timer after pressed a button

		ScreenStatusDisplay = true;
	}



	if (__down) {
		//        FANMINSPEED.subValue();
		navmenu.menuDown();
		printmenu();
		// Wait button realess
		buttonRelease(BUTTON_DOWN);
		exitmenutimer = exitmenutime; // update each time auto menu exit timer after pressed a button

		ScreenStatusDisplay = true;
	}


	if (__set && ScreenStatusDisplay) {




		lcd.blink();
		menu[navmenu.getMenuSelected()].DrawFunction();
		lcd.noBlink();


		//                       if (  menu[navmenu.getMenuSelected()].isEmptyFunctionValue() ) // ignore unknown value
		//                          menu[navmenu.getMenuSelected()].functionValue  = 


		if (!ScreenStatusDisplay) {
			navmenu.menuReset(); // When user exit from menu , give ScreenStatusDisplay = false
		}
		//                      Serial.println ("After ScreenStatusDisplay:"+String (ScreenStatusDisplay)  );

		// fast swith status output

	}
	if (__set && !ScreenStatusDisplay)
	{
		printstatus(true);
	}





}










