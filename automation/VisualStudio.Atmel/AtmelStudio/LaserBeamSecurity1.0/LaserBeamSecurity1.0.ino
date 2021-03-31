
//	This program was made for ESP8266

/* 

Some of the GPIO pins are used while booting, so Pulling this pin HIGH or LOW can prevent NODEMCU from booting

GPIO0: It oscillates and stabilizes HIGH after ~100ms. Boot Failure if pulled LOW
GPIO1: LOW for ~50ms, then HIGH, Boot Failure if Pulled LOW.
GPIO2: It oscillates and stabilize HIGH after ~100ms, Boot Failure if Pulled LOW.
GPIO3: LOW for ~50ms, then HIGH.
GPIO9: Pin is HIGH at Boot.
GPIO10: Pin is HIGH at Boot.
GPIO15: LOW, Boot failure if Pulled HIGH
GPIO16: HIGH during Boot and Falls to ~1Volt. 

GPIO16: pin is high at BOOT
GPIO0: boot failure if pulled LOW
GPIO2: pin is high on BOOT, boot failure if pulled LOW
GPIO15: boot failure if pulled HIGH
GPIO3: pin is high at BOOT
GPIO1: pin is high at BOOT, boot failure if pulled LOW
GPIO10: pin is high at BOOT
GPIO9: pin is high at BOOT


/*
NodeMCU IO index vs ESP8266 pin
IO index  ESP8266 pin
    0     [*] GPIO16
    1     GPIO5 
    2     GPIO4 
    3     GPIO0
    4     GPIO2
    5     GPIO14  
    6     GPIO12
    7     GPIO13
    8     GPIO15
    9     GPIO3
    10    GPIO1
    11    GPIO9
    12    GPIO10
[*] D0(GPIO16) can only be used as gpio read/write. 
No support for open-drain/interrupt/pwm/i2c/ow.
https://nodemcu.readthedocs.io/en/master/en/modules/gpio/

 
void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}


*/


/*
#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;


static const uint8_t LED_BUILTIN = 16;
static const uint8_t BUILTIN_LED = 16;
*/

/*

Label  GPIO            Input             Output                  Notes
D0     GPIO16          no interrupt      no PWM or I2C          support HIGH at boot , used to wake up from deep sleep
D1     GPIO5           OK                OK                     often used as SCL (I2C)
D2     GPIO4           OK                OK                     often used as SDA (I2C)
D3     GPIO0           pulled up         OK                     connected to FLASH button, boot fails if pulled LOW
D4     GPIO2           pulled up         OK                     HIGH at boot , connected to on-board LED, boot fails if pulled LOW
D5     GPIO14          OK                OK                     SPI (SCLK)
D6     GPIO12          OK                OK                     SPI (MISO)
D7     GPIO13          OK                OK                     SPI (MOSI)
D8     GPIO15          pulled to GND     OK                     SPI (CS), Boot fails if pulled HIGH
RX     GPIO3           OK?!              RX pin                 HIGH at boot
TX     GPIO1           TX pin            OK?!                   HIGH at boot , debug output at boot, boot fails if pulled LOW
A0     ADC0            Analog Input      NO

*/


//Reasons
// [1]  When pull up, wont let upload a program , better use pull down 
// [2]  Always pull up by default internal resistor
// [3] Fail to boot up, wdt reset when pinMode (input)

//    Warning
// {!1} --Disable Serial.print , and become regular GPIO INPUT/OUTPUT that can work with I2C Displays
//                                       //INPUT VCC-->[5.1k]-|-[btn]--GND                     INPUT VCC-->[btn]-|-[5.1k]--GND
//INPUT or buttons                               PULLUP                                                PULLDOWN

static const uint8_t DTX  = 1;  //                   {!1}                                                 {!1}
static const uint8_t DRX  = 3;   //                  {!1}                                                 {!1}
static const uint8_t D0   = 16;  //               Yes                                                      Yes  but can't be use as interupts
static const uint8_t D1   = 5;   //I2C            Yes                                                      Yes
static const uint8_t D2   = 4;   //I2C            Yes                                                      Yes
static const uint8_t D3   = 0;   //               Yes                                                      NO, Reason [2]
static const uint8_t D4   = 2;   //               Yes                                                      Yes
static const uint8_t D5   = 14;  //               Yes                                                      Yes
static const uint8_t D6   = 12;  //               Yes                                                      Yes
static const uint8_t D7   = 13;  //               Yes                                                      Yes
static const uint8_t D8   = 15;  //               NO,   Reason[1]                                          Yes


/*
static const uint8_t D9   = 10;   // SDD3         Must Be disconected wires from internals
static const uint8_t D10   = 9;   //SDD2          Must Be disconected wires from internals
static const uint8_t D11  = 8; //SDD1             NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D12  = 11; //SDCMD           NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D13   = 7; //SDD0            NO,   Reason[3]                                           NO,   Reason[3] 
static const uint8_t D14 = 6; //SDCLK             NO,   Reason[3]                                            NO,   Reason[3] 
*/

/*

 pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);

*/



/*
 Name:		lib_meniuInterface128x64OLEDSSD1306AsciiWire.ino
 Created:	2/8/2020 8:43:53 PM
 Author:	zick
*/

#include "lib_meniuInterface128x64OLEDBased(SSD1306AsciiWire).h"
#include "buttons.h"
void oneSecTimer ();
void oneMinTimer ();
// buttons
 byte BUTTON_SET =  D0;  // pull up
 byte BUTTON_DOWN = D3; // pull up
 byte BUTTON_UP = D4;   // pull up
 byte OUTPUT_ALARMSOUND = D5; //pull down output positive when true
 byte INPUT_LASERBEAM = D6;   //pull down if some one cut laser beam with body, give positive signal for 2sec
 byte INPUT_LASERWCASE = D7;  //pull down if case are removed , not return signal meaning, some try to remove it
 byte OUTPUT_RELAY_LAMP = D8; // pull down
 
//variables
byte var_one; 
byte var_two;  
byte var_three;
byte var_four; 
byte var_five; 
byte var_six;	

bool  bool_pswdPassed = false;	

byte alarm_totalTime = 60; // 1s*60 = 60s 
byte alarm_strenght;                   // pulse width modulated strength of alarm sound
byte alarm_strenghtWarning	= 50	;  // when password doesn't inputed correctly
byte alarm_strenghFullBlast	= 255	;  // when someone passed throw laser or password was incorrect to many times
byte alarm_delayTime ;				   // is it a timer for delaying alarm when laser was disturbed . To avoid recognition of security position
byte alarm_delayTotal = 10;			   // delay to alarm to turn on with laser at the same time
byte alarm_timer; // 				   // this timer depends only from input password buttons and works as timeout to turn on alarm
byte alarm_timerLaser;				   // react only laser input and are to store time how much should be on a alarm
byte alarm_countLaser;				   // count laser interrupts
byte alarm_incorrectScoreChanged;      // =0 // for no repeating multiple times
byte alarm_arm_disarm_timer;           // for representative's that user are armed a system with alarm short pulse alarm sound 
bool alarm_armed_disarmed_system=0;	   // enable or disable armed or disarmed to work with laser

byte relay_lamp_totalTime = 80; // store value how much time should work a relay
byte relay_lamp_timer;         // timer for turn on relay for a lamps

byte laser_beam;				// if some one cut laser beam with body, give positive signal for 2sec
byte laser_beamChanged;			// not react multiple times if laser was passed
byte laser_case;				// if case are removed , not return signal meaning, some try to remove it
byte laser_caseChanged;			// not react multiple times if case was removed

byte user_turnOnBeforeSystemTime;      // if password is correct and then was armed with buttons, first time , give time to ignore laser friendly fire on owner about 30sec to avoid instantly working sound
byte user_turnOnBeforeSystemTotal = 30;

bool short_signal = false;

//Clock variables
unsigned long clock_1min = 0;
unsigned long clock_1sec = 0;
unsigned long clock_01sec = 0; // .1 sec
unsigned long clock_btnPswTimeout = 0;  

// buttons variables
bool buttonUP;
bool buttonSET;
bool buttonDOWN;

buttons btnUp   (buttonUP,BUTTON_UP , true);//(bool &BTN/*button variable that change at demand*/,byte &PinRead /*what kind of pin to read*/ , bool InvertDigitalRead = true/*Read input statement invertecly !*/,bool PressSingleTime = false /*Disable fast pulses and leave single press*/ ,unsigned long CLOCK_ADD = 0UL /*add time  away to slower the speed of pulses*/) {
buttons btnDown (buttonDOWN,BUTTON_DOWN , true);
lib_meniuInterface128x64OLEDSSD1306AsciiWire menu(buttonUP, buttonDOWN, buttonSET); // set a buttons to in.

#include "EEPROM32.h"
#include "functions.h"

// the setup function runs once when you press reset or power the board
void setup() {
	//Serial.begin (115200);
	menu.IncludeFunction(&func1, alarm_totalTime, "Alarm Sound Time", "sec");
	menu.IncludeFunction(&func2, alarm_strenghtWarning, "Alarm  Warning", "pwm%" );
	menu.IncludeFunction(&func3, alarm_strenghFullBlast, "Alarm Full Blast", "pwm%");
	menu.IncludeFunction(&func4, alarm_delayTotal, "Alarm Bugler Delay", "sec");
	menu.IncludeFunction(&func5, alarm_countLaser, "Alarm Laser Count", "count");
	menu.IncludeFunction(&func6, relay_lamp_totalTime, "Lamp Shine Time", "sec");
	menu.IncludeFunction(&func7, user_turnOnBeforeSystemTotal, "Laser non react time", "sec");
	
	menu.IncludeQuckAccessFunction(&funcQc, quickAccesModes, "Alarmas", "mode", false);
	// a default function are saved in here.
	menu.IncludeFunctionSetDefault(&userSetDefault);
	
	menu.initiate();

	 if (quickAccesModes == 0)
	 {
		// print("Isjungta");
		alarm_armed_disarmed_system = 0;
		
	 }
	 else if (quickAccesModes == 1)
	 {
		// print("Saugo");

		 alarm_armed_disarmed_system = 1;
	 }
	 else if (quickAccesModes == 2)
	 {
		 //print("Testavimas");
		 alarm_armed_disarmed_system = 0;
		 //
	 } 
	
	for  (int i = 0; i < 8; i++)
	{
		display.setCursor(i * 3 ,1);
		println (".");
		delay(100);
	}

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);
	
	pinMode(OUTPUT_ALARMSOUND,OUTPUT);
	
	pinMode(INPUT_LASERBEAM,INPUT);
	pinMode(INPUT_LASERWCASE,INPUT);
	
	pinMode( OUTPUT_RELAY_LAMP, OUTPUT);
	
	
	analogWriteFreq(1000);
	analogWriteRange(255);
	
	// when system turn on , give time to ignore laser booting and giving false signals
	user_turnOnBeforeSystemTime = user_turnOnBeforeSystemTotal;
	
	Serial.print("Complete Loading");
	delay(50);
	//print("Laser Security Ready.");
	//delay(2500);
	display.clear();
	display.setCursor(1,1);
}






// odd even boolean
bool odd_even_01=false;;


void loop() {


	// read buttons values 
	buttonSET = !digitalRead(BUTTON_SET);
	
	
	
	
	


	

	
	
	one01SecTimer ();
	oneMinTimer ();
	oneSecTimer();
	
	btnUp.scaning();
	btnDown.scaning();
	
	
	

	

	// return condition about opened menu
	isMenuActive = !menu.InterfaceDinamic();
		if ( isMenuActive ) 
	{
			
		 if (quickAccesModes < 2)	// 3 == test mode 
			collectPasswordRuntime ();
		
   
	}
	
	
	 if (quickAccesModes < 2) // 3 = test mode
	 {
		 
	 
	
						if (psw_incorrectScore >= 25) // 5 incorrect times then alarm Base is 4
								{	
									alarm_timer = alarm_totalTime;
									psw_incorrectScore = 0; // reset for non repeating all the time
									alarm_strenght = alarm_strenghFullBlast;
									user_turnOnBeforeSystemTime=0; 
								}
								// first incorrect password , works as arm system and give time to exit from laser range
						else if ((alarm_arm_disarm_timer == 0 && psw_incorrectScore == pswdLenght/*=4*/  && alarm_incorrectScoreChanged != psw_incorrectScore)) // arm system
								{   
									Serial.println("shielded:" + String (alarm_incorrectScoreChanged) + " != " + String(psw_incorrectScore) );
				
									alarm_incorrectScoreChanged = psw_incorrectScore;
									//alarm_timer = 1; // 1sec alarm signal to alarm bugler from touching this buttons
									const_manualReapetEach01secVariable (5); // .5sec alarm signal to alarm bugler from touching this buttons
									alarm_strenght=alarm_strenghtWarning;
									
								if (!short_signal) // if not ever was armed , then give initiation's time for laser to react
								
									user_turnOnBeforeSystemTime=user_turnOnBeforeSystemTotal; // turn on timer for later laser start to react later
									
									alarm_armed_disarmed_system = true; // arm system
									
									short_signal = true;
			
								}
							
								// first incorrect password 
						else if (/*alarm_timer ==0*/ manualReapetEach01secVariable == 0 && psw_incorrectScore >= (pswdLenght+pswdLenght)/*=8*/  && alarm_incorrectScoreChanged != psw_incorrectScore)	// act as inputed incorrect password warning with alarm 1 second each
								{
									alarm_incorrectScoreChanged = psw_incorrectScore;
									//alarm_timer = 1; // 1sec alarm signal to alarm bugler from touching this buttons
									const_manualReapetEach01secVariable (5); // .5sec alarm signal to alarm bugler from touching this buttons
									alarm_strenght=alarm_strenghtWarning;
									alarm_armed_disarmed_system = true; // arm system
									Serial.println("password warning");
								}
		
	
						if (pswMatchingPulse) // user inputed correct password.
						{
								alarm_timer = 0; // if alarm is on, but later inputed correct to disable siren noise as soon as possible
								alarm_incorrectScoreChanged=0; 
								alarm_arm_disarm_timer = 4; //   alarm_arm_disarm_timer  / 2 , gives sound that represent correct password input
								alarm_strenght = alarm_strenghtWarning;
								alarm_armed_disarmed_system = false; // disarm system
								alarm_delayTime = 0;
				
						}
	

	// end of user password inputs with two buttons
	//**********************************************************************************************************************************************************
	
	
	
	
						if (alarm_armed_disarmed_system ) // if system is armed and ready to react with laser beam 100m sensor
						{
							laser_case = !digitalRead(INPUT_LASERWCASE); // should always give positive while wire connected
							laser_beam = digitalRead(INPUT_LASERBEAM);
		
		
		
		
		
		
		
									if (  manualReapetEach01sec && !pswisActivePrint)  // do each second and password menu is not opened
									{
											display.setCursor(0,0);
											menu.print__("Alarm: SAUGO\n\n");
									
										 if ( (alarm_delayTime != 0 || alarm_timer != 0)  ) 	//
										 {
											//menu.print__ ("laser_case:"+ String (laser_case));
											//menu.print__ ("laser_beam:"+ String (laser_beam));
										//	menu.print__ ("timer:"+ String (alarm_timer)+ " delay:"+String (alarm_delayTime));
										 }
									
									
									
									
									
									
									
									
											//*************************************************************** laser_beam	
											if (user_turnOnBeforeSystemTime == 0 && laser_beam !=laser_beamChanged )  // when laser interrupted and after user armed with buttons to wait for 10s 
											{
												laser_beamChanged = laser_beam; // not repeat multiple times
										
												if (laser_beam) // beam was disturbed
										
												{
											
														if (alarm_delayTime == 0 && alarm_timer == 0) // if both alarm timing is over then can be newly execute more siren 
														{
																// react a alarm from laser beam
																alarm_strenght = alarm_strenghFullBlast; // siren full strength
																alarm_delayTime = alarm_delayTotal; // delay 10s before a siren/alarm to confuse buglers
																//Serial.println("Saugo laser beam:"+String (alarm_arm_disarm_timer) + ", D:"+ String (alarm_delayTime)+ " T:"+ String (alarm_timer) );
													
														}
											
												}
											}
											
											
											
											
											
											
											//*************************************************************** laser_case
											if (/*need react immediately*//*user_turnOnBeforeSystemTime == 0 &&*/ laser_case !=laser_caseChanged )  // w// when laser interrupted and after user armed with buttons to wait for 10s 
											{
												laser_caseChanged = laser_case; // not repeat multiple times
										
												if (laser_case)
										
												{
											
														if (alarm_delayTime == 0 && alarm_timer == 0) // if both alarm timing is over then can be newly execute more siren
														{
															// react a alarm from laser beam
															alarm_strenght = alarm_strenghFullBlast; // siren full strength
															//alarm_delayTime = alarm_delayTotal; // delay 10s before a siren/alarm to confuse buglers
															
															alarm_timer = 230; /*alarm_totalTime*/; // immediately react at full time because this is serious 230= 3min,57sec
															relay_lamp_timer = 255; // turn on lamp for 255= 4min,15sec
															//alarm_delayTime = 2;//activate relay with short value in here 
															Serial.println("Saugo laser beam:"+String (alarm_arm_disarm_timer) + ", D:"+ String (alarm_delayTime)+ " T:"+ String (alarm_timer) );
														
														}
											
												}
											}	
										
										
										
										
										
										
										
										

									}
							}
							 else {
		
		
						//***************************************************************************** if system disarmed
							if (manualReapetEach1sec && !pswisActivePrint){
								display.setCursor(0,0);
								menu.print__("Alarm: ISJUNGTA");
							}
		
						}
						
						
						
						
		}
	    else 
		{
			
			
			laser_case = digitalRead(INPUT_LASERWCASE);
			laser_beam = digitalRead(INPUT_LASERBEAM);
			
			
				// printing
				menu.print__ ("Testivimas\n");
				menu.print__ ("laser_case:"+ String (laser_case));
				menu.print__ ("laser_beam:"+ String (laser_beam));
				
			
			
				if ( laser_beam !=laser_beamChanged )  // when laser interrupted
				{
						laser_beamChanged = laser_beam; // not repeat multiple times
					
								 if (laser_beam)	
						
												{
													
													alarm_arm_disarm_timer = 8;
													alarm_strenght = alarm_strenghtWarning;
													Serial.println("laser beam:"+String (alarm_arm_disarm_timer));
													
												}
				}
				
				if ( laser_case !=laser_caseChanged )  // when laser interrupted
				{
						laser_caseChanged = laser_case; // not repeat multiple times
								
								if (laser_case)
											
											{
												
												alarm_arm_disarm_timer = 4;
												alarm_strenght = alarm_strenghtWarning;
													
												Serial.println("laser case:"+String (alarm_arm_disarm_timer));
												
											}	
				}
			
		}
	
	
	
	manualReapetEach01sec = false;
	manualReapetEach1sec = false;
	btnUp.endScaning();
	btnDown.endScaning();
	
}






void oneMinTimer () {
	
	/// CLOCK 1 min			60000UL  120000UL  x 2 clock speed
	if (((long)clock_1min + 60000UL) < millis()) // 120000UL a double clock speed by
	{
		clock_1min = millis(); // reset each  60 seconds  time
		display.clear();
		manualReapetEach1sec = true; // reset for fast print
	}
	
}

void oneSecTimer () {
	
	if (((long)clock_1sec + 1000UL) < millis())
	{
		clock_1sec = millis(); // reset each  60 seconds  time
		manualReapetEach1sec = true; // reset each second to print one time
		
		
		
		
		
		
	//*******************************************
		
		if (user_turnOnBeforeSystemTime > 0) // turn on timer for laser start to react
		{
			user_turnOnBeforeSystemTime--;
			
			display.setCursor(0,7);
			menu.print__ ("Systema reguos:"+ String (user_turnOnBeforeSystemTime)+ "      ");
			
			if (user_turnOnBeforeSystemTime == 0 && short_signal) // give little sound that says is over
				{
					
					alarm_arm_disarm_timer = 2; // short pulse to alarm
					short_signal = false;
					display.setCursor(0,7);
					//menu.print__ ("						");
					//Serial.println(("short_signal"));
					
				}
		}
		
		
		
		
		
		
	//***********************************	
		
		if (alarm_delayTime > 0 && user_turnOnBeforeSystemTime == 0) // delay alarm for a while not to work,  set confusion for burglar's, 
		{
			
			
			alarm_delayTime--;
			
			display.setCursor(0,7);
			display.print ("Alarm prasides po:"+ String (alarm_delayTime)+ "      ");
			
			if (alarm_delayTime == 0) // set alarm timer
			{
				alarm_timer = alarm_totalTime; // give continue to turn on a alarm
				relay_lamp_timer = relay_lamp_totalTime; // turn on lamp only in here 
			}
		}
		
		
		
		
		
		
		
		
  //**************************************	
		
		if (alarm_timer > 0)
		{
			analogWrite(OUTPUT_ALARMSOUND,alarm_strenght);
			//Serial.println("A alrm:" + String (alarm_timer)+ " pswIncorrect: "+String(psw_incorrectScore));
			
			alarm_timer--;
			
			
			display.setCursor(0,7);
			
				menu.print__ ("Alarm IJUNGTA:"+ String (alarm_timer)+ "      ");
			
			
			
				
			
			if (alarm_timer == 0)
				display.clear();
				
			} else {
			analogWrite(OUTPUT_ALARMSOUND,0);
			//Serial.println("B");
			
			}
			
			
			
			
			
	 
	 
	 //**************************************		
		 if (relay_lamp_timer > 0 )
		   {
				relay_lamp_timer--; 
					digitalWrite(OUTPUT_RELAY_LAMP , HIGH);
					display.setCursor(0,6);
					menu.print__ ("Lempa IJUNGTA:"+ String (relay_lamp_timer)+ "      ");
					
		   }else{
					digitalWrite(OUTPUT_RELAY_LAMP,LOW);
		   }
		
		
	}
	
	
	
	
	
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	
	
}


void one01SecTimer () {
	
	if (((long)clock_01sec + 100UL) < millis())
	{
		
		clock_01sec = millis(); // reset each  60 seconds  time
		
		
		if (manualReapetEach01secVariable > 0) // hold while have values
		{
			manualReapetEach01secVariable--;
			
			analogWrite(OUTPUT_ALARMSOUND,alarm_strenght);
			//Serial.print( "manualReapetEach01secVariable[" +String(manualReapetEach01secVariable)+ "] on,"+ String (alarm_strenght));
			
			if (manualReapetEach01secVariable == 0 )
			{
				analogWrite(OUTPUT_ALARMSOUND,LOW);
				//Serial.println( " *******off");
			}
			
		}
		
		
		
		
		if (alarm_arm_disarm_timer > 0 ) // only use for short sound
		{
			
			
			//Serial.println( "alarm_arm_disarm_timer:"+String(relay_lamp_totalTime));
			
			odd_even_01=!odd_even_01;
			
			if ( odd_even_01 )  //odd or even toggle and works like on and off
			{
				analogWrite(OUTPUT_ALARMSOUND,alarm_strenght);
				//Serial.print( "alarm_arm_disarm_timer on,"+ String (alarm_strenght));
				
				
			}
			else {
				digitalWrite(OUTPUT_ALARMSOUND,LOW);
				//Serial.println( " off");
			}
			
			--alarm_arm_disarm_timer;
		}else
		{
			odd_even_01 =false;
		}
		
		
		manualReapetEach01sec = true;
	}
	
}

void fun_laser_case () 
{
  
  	
	laser_case = digitalRead(INPUT_LASERWCASE);
	
	
}
	
void fun_laser_beam ()	
{
	laser_beam = digitalRead(INPUT_LASERBEAM);
	
	//if (laser_beam)
	
}	