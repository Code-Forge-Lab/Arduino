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
// [1]  When pullup, wont let upload a program , better use pulldown 
// [2]  Always pullup by default internal resistor
// [3] Fail to boot up, wdt reset when pinMode (input)

//    Warning
// {!1} --Disable Serial.print , and become regular GPIO INPUT/OUTPUT that can work with I2C Displays
// {!2} -- If using button , must use pullup RESISTOR 
//                                       //INPUT VCC-->[5.1k]-|-[btn]--GND      ser               INPUT VCC-->[btn]-|-[5.1k]--GND
//INPUT or buttons                               PULLUP                                                PULLDOWN

static const uint8_t DTX  = 1;  //                    {!1}                                                {!1}
static const uint8_t DRX  = 3;   //                   {!1}                                                {!1}
static const uint8_t D0   = 16;  //               Yes {!2}                                                 Yes  but can't be use as interupts
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
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

*/

unsigned long clock_0_1sec = 0;      // 0.2 seconds update
int clock_1secValue = 10;
int clock_3secValue = clock_1secValue * 3;
int clock_5secValue = clock_1secValue * 5;
unsigned long clock_1secCounter = clock_1secValue; // if x 10s then = 1 sec
unsigned long clock_3secCounter = clock_3secValue; // if x 30s then = 3 sec
unsigned long clock_5secCounter = clock_5secValue;     // if x 50s then = 3 sec
unsigned long clock_1sec = 0;
unsigned long clock_volatge = 0; // seconds update




// Function declaration
bool timer1sec();
bool timer3sec();
bool timer5sec();
String getText(String index, bool conditionNaming);
void funInv_On_then_Output220(String x = "off", bool silence = false);
void oneSecTimer();
void quarterSecondTimer();
String fun_CmdRead(String);
void setBatVltRange(uint8_t, uint8_t);
bool avoidBatVltOutOfRangeThenMemCommit(); // avoid   inputed voltage to be out of range;
void getmemBatVlt();
void funTurnInvAutomaticallyByVoltage();
bool reactionFromASensors(bool react = false);
void funmaxBatVltSustained(bool enableTimer = false);
void funTurnOffTimer(bool enableTimer = false);
void funDelay_Inv_Output220(bool enableTimer = false);
String fungetfromatedTime(signed int seconds);
void serialPrint1s(String txt);
void serialPrintln1s(String txt);
void serialPrint3s(String txt);
void serialPrint5s(String txt);
void serialPrintln3s(String txt);
void funResetTriggeredAction();
void funTimeClient();

    /*    // enable timeout only one time after a triggeredAction event
        if (triggeredAction  && triggeredTimeoutCnt == 0) {
            triggeredTimeoutCnt = triggeredTimeoutMax;
        }

        // count down a timeout where more could accures triggeredAction
        if (triggeredTimeoutCnt > 0)  triggeredTimeoutCnt--;

        //  time where triggered actioct will be tracked in 10 minutes period
        if (triggeredTimeoutCnt > 0  && triggeredAction){

                Serial.println ("Reached triggeredTracketEvents");
          // keep counting triggered actioct
          if (triggeredTracketEventsCnt < triggeredTracketEventsMax - 1) triggeredTracketEventsCnt ++;
          // reached maximum error AI level where will enable program waiting for a long time after many failures from a sensors or a program attempts to work propietly
          else {Serial.println ("Maxed out triggeredTracketEventsMax"); triggeredLongAITimeReached = true; };
        }

        // initiate long time timer
        if (triggeredLongAITimeReached && triggeredLongAITimeCnt == 0)
            triggeredLongAITimeCnt = triggeredLongAITimeMax;

        // keep holding a progrom to work about 6h
        if (triggeredLongAITimeCnt > 0) {

                triggeredLongAITimeCnt--;
            if (triggeredLongAITimeCnt == 2)
                desribtionsInTextAI = "" ;// clear text
            }

        else{
           triggeredLongAITimeReached = false; // reset AI
           triggeredTimeoutCnt = 0;
           }*/

    // AI main decision logic vars

    bool triggeredAction;                // comes from sensors , auto mode program actions as a flag that enables triggeredTimeoutCnt = triggeredTimeoutMax
int triggeredTimeoutMax = 800;           // seconds, time where triggered actioct will be tracked  in 10 minutes period
int triggeredTimeoutCnt = 0;             // counting down a time after a trigger event is established
int triggeredTracketEventsMax = 3;       // tolerate 3 triggeredTracketEventsCnt before crucial desition to stop working inverter
int triggeredTracketEventsCnt = 0;       // Track every triggeredAction where accures at sensors or in  auto mode program for later to deside a crucial desition to stop working inverter for a long time
bool triggeredLongAITimeReached = false; //  rememeber a triggered long time is established
int triggeredLongAITimeMax = 25300;      // 6h  maximum AI triggered long time where will set a triggeredLongAITimeCnt to turn of inverter for a many hours
int triggeredLongAITimeCnt = 0;          //  keep counting
String desribtionsInTextAI = "";         // hold information about failed conditions from a  sensors in AI frame 0 / 3


void funTriggeredLongTimeCounter ()
{

  

  // if (triggeredLongAITimeReached && triggeredLongAITimeCnt == 0)
  //   triggeredLongAITimeCnt = triggeredLongAITimeMax;

  // keep holding a progrom to work about 6h
  if (triggeredLongAITimeCnt > 0)
  {
    triggeredLongAITimeCnt--;
    // if (triggeredLongAITimeCnt == 2)
    //   desribtionsInTextAI = ""; // clear text
  }

}

// do invidual calculation per each object as AI tracking
  class timingIntervalsObj
  {
    bool __supportTriggeredAction;           // Sustain triggeredAction to be enable only one time 
    bool __triggeredAction;                 // comes from sensors , auto mode program actions as a flag that enables triggeredTimeoutCnt = triggeredTimeoutMax
    int __triggeredTimeoutMax = 800;
    int __triggeredTimeoutCnt = 0;       // counting down a time after a trigger event is established
    int __triggeredTracketEventsCnt = 0; // Track every triggeredAction where accures at sensors or in  auto mode program for later to deside a crucial desition to stop working inverter for a long time
    int __triggeredTracketEventsMax = 3;   // tolerate 3 triggeredTracketEventsCnt before crucial desition to stop working inverter
    bool __triggeredLongAITimeReached = false; //  rememeber a triggered long time is established
    int __triggeredLongAITimeMax = 25300;
    int __triggerActionCnt = 0; // after sensor is triggered , keep countting in small unit to give more time to confirm assignment
    bool __enableTriggerEvents = true; // keep reacting until triggered event has accure if this var is enable
    String name;

  public:
    timingIntervalsObj(String abbreviation, int maxAIEventProtectionCount = 3, int maxAITimer = 6 /*6h*/ , int triggerTimeout = 800) // contructor
    {
      name = abbreviation;
      __triggeredTracketEventsMax = maxAIEventProtectionCount;
      __triggeredLongAITimeMax = maxAITimer * 3600 + 100;
      __triggeredTimeoutMax = triggerTimeout;
    }

void InteractionCountinerGlobalUseOneTime () { // inportant function to be placed in raw base counting invorement

      // 0 keep discounting time from AI from 6h
      //  keep holding a progrom to work about 6h
      if (triggeredLongAITimeCnt > 0)
      {
        if (timer1sec())
          triggeredLongAITimeCnt--;
        // if (triggeredLongAITimeCnt == 2)
        //   desribtionsInTextAI = ""; // clear text
      }
}

 // main function
    bool InteractionTime(bool isSensorSensing, bool returnTrueTrigger  = false ,/* output not a pulse*/ int suspendTriggerMiliSeconds = 20)
    {
/////////////////// Long Time Waiting Logic
      if (timer1sec())
        
        {        
              


             

              // count down a timeout where more could accures triggeredAction 10min
              if (__triggeredTimeoutCnt > 0)  __triggeredTimeoutCnt--;
          
              else if (!triggeredLongAITimeReached) // reset inspection event counter if not reached __triggeredLongAITimeReached
              {
                
                __triggeredTracketEventsCnt = 0;
                desribtionsInTextAI = "";
              }

              if (triggeredLongAITimeCnt == 0 && __triggeredTimeoutCnt == 0)
              {
                    triggeredLongAITimeReached = false; // reset AI
                  __triggeredLongAITimeReached = false;
                  triggeredTimeoutCnt = 0;
                  // Serial.println(name + " Finished Long Time Waiting ");
                   resetErrors();
              }

                  // Long Time Waiting AI protection

             

             
        } // end timer1sec()


 /// Sensing category
        __triggeredAction = false; // keep reseting until condition accurace
        if (isSensorSensing)       // if sensor is triggered
        {

            if (__triggerActionCnt >= (/* clock_1secValue +  */suspendTriggerMiliSeconds))
            {
                      // __triggerActionCnt = clock_1secValue * 5;
                      // trigger by sensor event only one time

                      if (!__supportTriggeredAction) // trigger only single time a about reached  condition
                      {
                       // 1 trigger one time
                        __triggeredAction = true;
                        __supportTriggeredAction = true;

                        // 2 valuate 10min timer
                        // enable timeout only one time after a triggeredAction event
                        if (/* __triggerAction &&  */ __triggeredTimeoutCnt == 0)
                        {
                          __triggeredTimeoutCnt = __triggeredTimeoutMax /* * clock_1secValue */;
                        }
                       // 3 Count AI errors , after 10min reset all
                       // reached maximum error AI level where will enable program waiting for a long time after many failures from a sensors or a program attempts to work propietly
                        if (!getIsReachedTriggeredEventsMax() && __enableTriggerEvents) // while not reched maximum event count , keep counting
                          __triggeredTracketEventsCnt++;

                         if (getIsReachedTriggeredEventsMax()) // Maxed out triggeredTracketEventsMax
                         {
                          triggeredLongAITimeReached = true;
                          __triggeredLongAITimeReached = true;
                          // Serial.println ("REACHED AI!!");
                        }
                        // 4 set 6h AI time if getIsReachedTriggeredEventsMax reached

                        if (triggeredLongAITimeReached /*||  __triggeredLongAITimeReached */ && triggeredLongAITimeCnt == 0)
                         { 
                          triggeredLongAITimeCnt = __triggeredLongAITimeMax;
                         }

                          // print text
                         if (!getIsReachedTriggeredEventsMax()) // save  every happened condition into a string
                          desribtionsInTextAI += String(__triggeredTracketEventsCnt) + " " + name + " ";
                        
                           Serial.println(getStatementStr());
                      }
            }
            else
            {
                      __triggerActionCnt++; // count when exsepted a sensoring
                  //    Serial.println(name + " Counting Action: " + String(__triggerActionCnt));  // show interaction with a sencor
            }
      }
      else if (__triggerActionCnt > 0) // keep substracting triggered action
      {
          __triggerActionCnt--;

      //  Serial.println(name + " Remove Counted Action: " + String(__triggerActionCnt));  // show interaction with a sencor
      }
      else // reset triggered action if completed a cycle
      {

        if (__supportTriggeredAction)
     //  Serial.println(name + "------------------>>  Reset  triggered " + String (__triggeredAction) ); // print last time
        __supportTriggeredAction = false;                                                                 // when time nothing happening then reset boolean
      }

      if (returnTrueTrigger)
        return __supportTriggeredAction;
    // else 
        return __triggeredAction;
    };

    String getStatementStr () {
      return ("[ " + name + " ]" /* + getText("Triggered condition  ", __triggeredAction) */ + "  Events Cnt " + String(__triggeredTracketEventsCnt) + " / " + String(__triggeredTracketEventsMax) + " [10m counter]: " + String(__triggeredTimeoutCnt) + getText(" is Reached 6h protection local ", __triggeredLongAITimeReached) + getText(" global ", triggeredLongAITimeReached) + " time left: " + fungetfromatedTime (triggeredLongAITimeCnt) );
    }

    bool getIsReachedTriggeredEventsMax () {
      return __triggeredTracketEventsCnt >= __triggeredTracketEventsMax;
    }

    bool getTriggeredTracketEventsAny () {
      return __triggeredTracketEventsCnt > 0 || __triggeredTimeoutCnt > 0;
    }
    void setEnableTriggerEvents(bool cond) { __enableTriggerEvents  = cond;};
    void resetErrors()
    {
       __triggeredTimeoutCnt = 0;      
       __triggeredTracketEventsCnt = 0; 
       __triggeredLongAITimeReached = false;
       desribtionsInTextAI = "";
    }

    void resetErrorsAll()
    {
      triggeredTimeoutCnt = 0;
      triggeredLongAITimeCnt = 0;
      triggeredLongAITimeReached = false;
      triggeredTracketEventsCnt = 0;
      triggeredAction = false;
      resetErrors();
    }
     int getTriggerActionCnt () {return __triggerActionCnt;};
    String fungetfromatedTime(signed int seconds)
    {
      String txt;
      int h = (seconds % 86400L) / 3600;
      int m = (seconds % 3600) / 60;
      int s = (seconds % 60);

      if (h > 0) // hours
        txt += String(h) + "h ";

      if (m > 0 && h == 0) // min  // m == 0 to save space and only show hours
        txt += String(m) + "m ";

      if (s > 0 && h == 0 && m == 0) // seconds then minints and last will be a seconds
        txt += String(s) + "s";

      return txt;
    }

    bool getTriggeredAIReached() {return __triggeredLongAITimeReached;};
        String getAbbreviationName() { return name; };
    void cleardesribtionsInTextAI() { desribtionsInTextAI = ""; };
};

// reaction time where react to a sensor to a trigger triggeredAction
timingIntervalsObj ObjTriggerInv_Output220("Inv_Output220",5,10,1400);     // reaction time where react to a sensor
timingIntervalsObj ObjTriggerInv_ReadSignal("Inv_ReadSignal",4);   // reaction time where react to a sensor
timingIntervalsObj ObjTriggerPrg_StopInv("Prg_StopInv/Pause",5);   // reaction time where react to a sensor
timingIntervalsObj ObjTriggerPrg_StopInvTemp("Prg_StopInvTemp", 4); // reaction time where react to a sensor
timingIntervalsObj ObjSuddenVoltageChange("SuddenVoltageChange", 7, 16); // reaction time where react to a sensor

//AI   END ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// #include <ESP8266WiFi.h>

// #include <DNSServer.h> // not needy
// #include <ESP8266WebServer.h> // not needy

//B

#include <WiFiManager.h>  //         // https://github.com/tzapu/WiFiManager
#include "Voltmeter2.h"
#include "buttons.h"
#include "EEPROM32.h"
#include <NTPClient.h>
// char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeekLT[7][20] = {"Sek", "Pir", "Ant", "Tre", "Ket", "Pen", "Ses"};
String currentTimeHeader;
// Define NTP Client to get time
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 7200; // 10800 // time zone offset
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

/*  About program
Inspect where inv. is on and output strong 220vac
  GPIO16 (D0) = Inv.ReadAC+         ->testPoint_Inv,ReadAC
*/
const static uint8_t Inv_readAC = D0;
bool sensorDoInv_readAC = false;
/*
Relay Turn On Relay to 220v Output
Can control 220v relay and also read inv live, is completly on with "Feed_Separate"
  GPIO5 (D1) = Inv.Output220        ->Realy { 220v Out [r__/ __] }
*/

// int maxBatVltSustainedMax_sec = 60;
// int maxBatVltSustained_cnt = 0; //  keep count of maxBatVltSustained_sec
// int maxBatVltSustained_sec = maxBatVltSustainedMax_sec; //  if not interuption in 30 seconds have in reached by maximum voltage range then
// byte maxBatVltSustained_User; 

const static uint8_t Inv_Output220 = D1;

int delay_Inv_Output220Max_sec = 30; //max delay before turning on a relay to pass a power from inverter
int maxDelay_Inv_Output220_sec  = delay_Inv_Output220Max_sec; // align a values 
int delay_Inv_Output220_cnt;          //sec delay to pass power throw power relay from inverter
float delay_Inv_Output220_ReactRatio = 0.2; // esp start to react to inverter output after 20% time left ,  maxDelay_Inv_Output220_sec < (delay_Inv_Output220Max_sec * maxDelay_Inv_Output220_ReactRatio)
byte    delay_Inv_Output220_User; // user saved value from a EEPROM memory
bool    doInv_Output220 = false;      //sec delay to pass power throw power relay from inverter



/*
Relay Turn On Inverter
Turn on invite. by powering throw relay
  GPIO4 (D2) = Inv.On               ->Relay { Inv.On [r__/ __] }
*/
const static uint8_t Inv_On = D2;
uint8_t maxDelayAvoid_Inv_On = 24; //sec delay to turn to on inverter to fast, avoid toggle switching 
uint8_t delayAvoid_Inv_On = 0;    //sec delay to turn to on inverter to fast, avoid toggle switching 
bool    doAvoidInv_On = false;    //sec delay to turn to on inverter to fast, avoid toggle switching
/*
Turn on inv. prg by button
  GPIO0 (D3) = Prg.on.button
*/
const static byte Prg_on_button = D3;
bool buttonSET;
byte BUTTON_SET =  Prg_on_button;
// buttonTimeout btnPrg_on (buttonSET,BUTTON_SET , true ,3000);
bool sensorProg_on_button = false; // readDigital
bool doPrg_on_button = false; // 
byte delayPrg_on_button = 0; // counter where begins to give a true signal
byte maxDelayPrg_on_button = 8; // seconds
/*
Indicate status about program
  GPIO2 (D4) = LED_Indicator
*/
const static uint8_t LED_Indicator = D4;
/*
Read external signals from inverter where tells about successful working stage
  GPIO14(D5) = Inv.ReadSignal+      ->Contact Input
*/
const static uint8_t Inv_ReadSignal = D5;
bool sensorDoInv_ReadSignal = false;
/*
Read external signals from inverter where tells about successful working stage
  GPIO12(D6) = Prg.StopInv+         ->48-70V Input
*/
const static uint8_t Prg_StopInv = D6;
bool sensorDoPrg_StopInv = false;

/*
Swtich temperature sensor that give a contact then reached critical level
Prg.StopInvTemp _/_
*/
const static uint8_t Prg_StopInvTemp = D7; 
bool sensorPrg_StopInvTemp = false;

/*Control Solar input to a inverter*/
const static uint8_t Solar_CtrlInput = D8; 
bool sensorSolar_CtrlInput = false;


/*
--Analog Read--
  A0 = Read.Batery.Volt
*/
const static uint8_t Read_Battery_Volt = A0;
int sensorRead_Battery_Volt = 0;  // value read from the pot
//(uint8_t pin_input = A0 ,              float broken_voltage = 0.0        ,  float R1 = 100000.0 , float R2 = 10000.0  )
VoltMeter voltAvrBattery  (Read_Battery_Volt, 0.0 ,   680000.0 ,5100.0  ); // my home  its good only for 48v becouse at 48v is only 120 bits
// VoltMeter voltAvrBattery(Read_Battery_Volt, 0.0, 2001000.0, 56200.0);        // else 2M-56.2k-| // working shit , values to high and give absolut non lineal result
// VoltMeter voltAvrBattery(Read_Battery_Volt, 0.0, 681000.0, 15000.0); // ELSE2 maybe ok , 320bits ant 53v
byte minBatVlt = 15;
byte maxBatVlt = 16;
byte fixBatVlt = 100; // 1 equil 0.01 * 5100 = 510  // fix voltage in 255 range
bool doBatMaxVltReached = false; // enable when maximum voltage was reached and disable when minimum voltage is gained

bool doBatIsLow = false; // save condition about battery low voltage in the scope
bool doBatBeHigh = false; // save condition about battery high voltage in the scope
bool doReactInBatVlt = true; // react in battery automaticly turn on inverter by changend voltage;

// maximum time where are awaiting to initiate a program if  maximum voltage is tabley high  
int maxBatVltSustainedMax_sec = 60;
int maxBatVltSustained_cnt = 0; //  keep count of maxBatVltSustained_sec
int maxBatVltSustained_sec = maxBatVltSustainedMax_sec; //  if not interuption in 30 seconds have in reached by maximum voltage range then
byte maxBatVltSustained_User;
// initiate after reached low voltage protection and this timing counters works as confirming that battery is low
int turnOffTimer = 0 ;// to avoid turning on and very fast turning off a boiler thant create junp in voltage at nearly fully charget batery 
int turnOffTimerMaxConst = 0; // not changing base value
int turnOffTimerMax = turnOffTimerMaxConst ;// seconds 
byte turnOffTimerUser = 1; // store addition timer value , additional for a turnOffTimerMax

// conditional desribtions statements from a program

   bool desctiptionInv_readAC = false ;
   bool desctiptionInv_readACActive = false;
   bool desctiptionInv_ReadSignal = false ;   
   bool desctiptionPrg_StopInv = false ;     
   bool desctiptionPrg_StopInvTemp = false ;
   bool desctiptionSuddenVoltageChange = false;

   // conditional description statements from a user
   bool desctiptionUserInv_readAC = false ;      
   bool desctiptionUserInv_ReadSignal = false ;   
   bool desctiptionUserPrg_StopInv = false ;     
   bool desctiptionUserPrg_StopInvTemp = false ; 
   String desribtionsInTextSensing = ""; // hold information about failed current from a sensors that failed
  //  String desribtionsInTextAI = ""; // hold information about failed conditions from a  sensors in AI frame 0 / 3
   int regulateComandLineOutput = 0;
   int regulateComandLineOutputMax = 250;


   
   // sec delay to pass power throw power relay from inverter

   // EEPROM memory address
   int16_t memMinBatVlt = 1;
   int16_t memMaxBatVlt = 2;
   int16_t memfixVltR = 3;
   int16_t memReactInBatVlt = 5;
   int16_t memInv_readAC = 6;      // ignore sensoring condition from a user leaved statements
   int16_t memInv_ReadSignal = 7;  // ignore sensoring condition from a user leaved statements
   int16_t memPrg_StopInv = 8;     // ignore sensoring condition from a user leaved statements
   int16_t memPrg_StopInvTemp = 9; // ignore sensoring condition from a user leaved statements
   int16_t memturnOffTimer = 10;   // turn off timeout saved at memory point from a user side
   int16_t memBatVltSustained = 11;
   int16_t memDelay_Inv_Output220 = 12; // aditional time from a user
   int16_t memSustain220vReactionTime = 13;
   int16_t memActivate220vReactionTime = 14;

   // fast blink
   bool LED_IndicatorBlink = false;
   uint8_t LED_IndicatorBlinkFast = 0;
   uint8_t LED_IndicatorBlinkFast_Common = 4;
   uint8_t LED_IndicatorBlinkFast_Long = 8;
   uint8_t LED_IndicatorBlinkFast_CommonShort = 2;

   // Set web server port number to 80
   WiFiServer server(80);

   // Current time
   unsigned long currentTime = millis();
   // Previous time
   unsigned long previousTime = 0;
   // Define timeout time in milliseconds (example: 2000ms = 2s)
   const long timeoutTime = 2000;

   // Variable to store the HTTP request
   String header;

   /// input commands from user
   String cmd_received;
   String cmd_msgOut; // write to user what condition have hit on the webserver

  byte Sustain220vReactionTimeUser = 7; // React to a 220v output where was lower then expected in the of a certain time.
  bool Activate220vReactionTimeUser = true; //activate triggered protection if user allowed in the setting 
      // Auxiliar variables to store the current output state
      String output5StateInvOutput = "off";
  String output4State = "off";

  // Assign output variables to GPIO pins
  const int output5 = Inv_Output220;
  const int output4 = Inv_On;

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  void updateMemoryBool(int16_t *memAddress, bool defaultValue, String text, bool *saveValueToVar)
  {
    if (readMemoryByte(*memAddress) >= 255)
    {
      writeMemory(*memAddress, defaultValue);
      Serial.println(getText(text, defaultValue));
    }
    else
    {
      *saveValueToVar = (bool)readMemoryByte(*memAddress);
      Serial.println(" Reseived bool: addr[" + String(*memAddress) + "]  " + text + ": " + String(*saveValueToVar));
    }; // Works as potentiometer
    Serial.print("");
}
void updateMemoryByte (int16_t *memAddress, byte defaultValue , String text, byte *saveValueToVar ) {
if (readMemoryByte(*memAddress)>=255) {writeMemory(*memAddress,defaultValue); Serial.print (": "+ String (defaultValue));} else{  *saveValueToVar = (byte)readMemoryByte(*memAddress); Serial.println (" Reveived byte: addr["+ String (*memAddress) + "]  " + text + ": " + String (*saveValueToVar));} // Works as potentiometer
Serial.print ("");
}

void setup() {
  Serial.begin(115200);
  Serial.println ("Starting setup . . .");

  EEPROM32_INIT( 18);
 delay (10);
 getmemBatVlt ();

 if (readMemoryByte(memfixVltR)>=255) {writeMemory(memfixVltR,(byte)100); Serial.println ("Writing memfixVltR: 100");} else voltAvrBattery.FixVltR(readMemoryByte(memfixVltR)); // Works as potentiometer
 // if (readMemoryByte(memReactInBatVlt)>=255) {writeMemory(memReactInBatVlt,true); Serial.println ("Writing memReactInBatVlt: true");} else doReactInBatVlt = (bool)(readMemoryByte(memReactInBatVlt)); // Works as potentiometer
 Serial.print ("ROLER--------------------");
 Serial.print ("\n\n");
//  if (readMemoryByte(memInv_readAC)>=255) {writeMemory(memInv_readAC,true); Serial.println ("Writing memInv_readAC: true");} else                desctiptionUserInv_readAC = (bool)(readMemoryByte(memInv_readAC)); // ignore sensoring condition from a user leaved statements
//  if (readMemoryByte(memInv_ReadSignal)>=255) {writeMemory(memInv_ReadSignal,true); Serial.println ("Writing memInv_ReadSignal: true");} else    desctiptionUserInv_ReadSignal = (bool)(readMemoryByte(memInv_ReadSignal)); // ignore sensoring condition from a user leaved statements
//  if (readMemoryByte(memPrg_StopInv)>=255) {writeMemory(memPrg_StopInv,true); Serial.println ("Writing memPrg_StopInv: true");} else             desctiptionUserPrg_StopInv = (bool)(readMemoryByte(memPrg_StopInv)); // ignore sensoring condition from a user leaved statements
//  if (readMemoryByte(memPrg_StopInvTemp)>=255) {writeMemory(memPrg_StopInvTemp,true); Serial.println ("Writing memPrg_StopInvTemp: true");} else desctiptionUserPrg_StopInvTemp = (bool)(readMemoryByte(memPrg_StopInvTemp)); // ignore sensoring condition from a user leaved statements
//  if (readMemoryByte(memturnOffTimer)>=255) {writeMemory(memturnOffTimer,(byte)1); Serial.println ("Writing memturnOffTimer: 1");} else turnOffTimerUser = (byte)(readMemoryByte(memturnOffTimer)); 

updateMemoryBool (&memReactInBatVlt, true , "memReactInBatVlt", &doReactInBatVlt );
updateMemoryBool (&memInv_readAC, true ,    "memIgnoreInv_readAC", &desctiptionUserInv_readAC );
updateMemoryBool (&memInv_ReadSignal, true , "memIgnoreInv_ReadSignal", &desctiptionUserInv_ReadSignal );
updateMemoryBool (&memPrg_StopInv, true ,    "memIgnorePrg_StopInv", &desctiptionUserPrg_StopInv );
updateMemoryBool (&memPrg_StopInvTemp, true ,"memIgnorePrg_StopInvTemp", &desctiptionUserPrg_StopInvTemp );
updateMemoryByte (&memturnOffTimer, 0 ,     "memturnOffTimerLownVolt", &turnOffTimerUser );
updateMemoryByte (&memBatVltSustained, 1 ,  "memBatVlotSustainedHighVolts", &maxBatVltSustained_User );
updateMemoryByte (&memDelay_Inv_Output220, 1 , "memDelay_Inv_Output220Relay", &delay_Inv_Output220_User );
updateMemoryByte (&memSustain220vReactionTime, 7 , "memSustain220vReactionTime", &Sustain220vReactionTimeUser );
updateMemoryBool(&memActivate220vReactionTime, true, "memActivate220vReactionTime", &Activate220vReactionTimeUser);

// recalculate value where was changed by user stored values in EEPROM memory
funmaxBatVltSustained ();
funDelay_Inv_Output220 ();
funTurnOffTimer (true); // refresh calculations
// Serial.print ("Init turnOffTimer: " + String (turnOffTimer));

 if (doReactInBatVlt)        output4State = "on"; // change graphical user interface


 delay (10);
 funInv_On_then_Output220("off", false); // fix low voltage bug when program booted first time , doesnt worked
 delay(10);
 // Initialize the output variables as outputs
 pinMode(Inv_readAC, INPUT);
 pinMode(Inv_Output220, OUTPUT); // output
 pinMode(Inv_On, OUTPUT);        // output
 pinMode(Prg_on_button, INPUT);
 pinMode(LED_Indicator, OUTPUT); // output
 pinMode(Inv_ReadSignal, INPUT);
 pinMode(Prg_StopInv, INPUT);
 pinMode(Prg_StopInvTemp, INPUT);
 pinMode(Read_Battery_Volt, INPUT);
 // Set outputs to LOW
 // digitalWrite(output5, LOW);
 // digitalWrite(output4, LOW);

//A
 
  // Uncomment and run it once, if you want to erase all the stored information
  // wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  
  wifiManager.setConfigPortalTimeout(280);
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Inverter Manager");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  



  //B
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  //WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }


  //Serial.println (WiFiManager.localIP());
  // if you get here you have connected to the WiFi
  Serial.println("Connected________________________");
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


   // myClass.getVal();
void loop(){
  
  WiFiClient client = server.available();   // Listen for incoming clients

   

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis(); 
     // prgm
     // btnPrg_on.scaning();
       oneSecTimer ();


      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
           
            
            // turns the GPIOs on and off
            if (!doAvoidInv_On && header.indexOf("GET /5/on") >= 0) {//sec delay to turn to on inverter to fast, avoid toggle switching
              Serial.println("GPIO 5 on");
             
               // output5StateInvOutput = "on"; // alredy in funInv_On_then_Output220 function

               // second relay that pass power throw power 220v relay from inverter and also turn on inverter
              if (!doBatIsLow && reactionFromASensors ()) // turn on inverter when no battery voltage is to low
               {
                funInv_On_then_Output220 ("on") ;
                funResetTriggeredAction ();
              } 
              else
               delayAvoid_Inv_On = maxDelayAvoid_Inv_On;  // activate protection against turning on to fast and multiple times


            } else if (header.indexOf("GET /5/off") >= 0) {
              
              Serial.println("GPIO 5 off");
              // output5StateInvOutput = "off"; // alredy in funInv_On_then_Output220 function

              
               funInv_On_then_Output220 ("off") ; 
               // delayAvoid_Inv_On = 5;
             

            } else if (header.indexOf("GET /4/on") >= 0) {

              Serial.println("GPIO 4 on");
              output4State = "on";
              //digitalWrite(output4, HIGH);
              writeMemory(memReactInBatVlt,true);
              doReactInBatVlt = true;

            } else if (header.indexOf("GET /4/off") >= 0) {

              Serial.println("GPIO 4 off");
              output4State = "off";
             // digitalWrite(output4, LOW);
              writeMemory(memReactInBatVlt,false);
              doReactInBatVlt = false;

            } else if (header.indexOf ("/configurations?finput") > 0 ){
              
              cmd_received = header.substring(header.indexOf("/configurations?finput") + 23, header.indexOf (" HTTP") ); // //test cmd_received output
               // cmd_received = header.substring ( cmd_received.indexOf ("finput="), 67 );
               // cmd_received = header.substring( 3, 55 ); // //test raw cmd_received output
               //
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_CommonShort;
            }
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<script> \n      setTimeout(()=> {\n        window.history.replaceState({}, document.title, \"/\" + \"refresh\");\n      }\n      ,1400);\n   </script>"); // refresh page with new default values after some time
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background: #ffffEE;}");
            client.println(".button { background-color: #77878A;  border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("p{margin 5px auto; margin-top: 0px;}"); // all paragraph tountered as not tall

            // battery indicator
            client.println("\n.batteryContainer {\n display: -webkit-box;\n display: -moz-box;\n display: -ms-flexbox;\n display: -webkit-flex;\n text-align: center;\n  margin: 0px auto;\n}\n.batteryOuter {\n border-radius: 3px;\n border: 2px solid #444;\n padding: 2px;\n width: 45px;\n height: 20px;\n}\n.batteryBump {\n  border-radius: 2px;\n background-color: #444;\n margin-top: 9px;\n width: 2px;\n height: 9px;\n}\n#batteryLevel {\n border-radius: 2px;\n background-color: #73AD21;\n width: 80%;\n height: 20px;\n}");
            client.println(".button2 {background-color:MediumSeaGreen ;}</style></head>");
            client.println("<div>");
            // Battery indicator with Web Page Heading
            client.println("<div class=\"batteryContainer\">\n    <h1 class=\"batteryContainer\" style=\" position: relative; left: 8 px; \">BatteryOperationOS</h1>\n    <div class=\"batteryOuter\"><div id=\"batteryLevel\"></div></div>\n    <div class=\"batteryBump\"></div>\n  </div>");
            client.println("<div style = \"position: relative; text-align: right; \">" + currentTimeHeader + "</div>");
            // Display current state, and ON/OFF buttons for GPIO 5
            // client.println("<p>Inv_On - State " + output5StateInvOutput + " Avoid: " + String(delayAvoid_Inv_On) + "</p>");
            // clear URL
            // client.println("\n<script>\n     console.log(" + (WiFi.localIP().toString()) + "\":: Redirecting page++++++++++++++++++\");\n      // prints the current URL \n      location.href = " + "'http://" + (WiFi.localIP().toString()) + "/refresh';\n</script>\n");
            // If the output5StateInvOutput is off, it displays the ON button
            if (output5StateInvOutput == "off")
            {
              // indicate about turning on an Inverter
             if (!doAvoidInv_On)

                //  ObjTriggerInv_Output220;
                //  ObjTriggerInv_ReadSignal
                //  ObjTriggerPrg_StopInv
                //  ObjTriggerPrg_StopInvTemp;
                //  ObjSuddenVoltageChange;
                  if(doBatBeHigh) client.println("<p><a href=\"/5/on\"><button class=\"button\">Inv Off / High Battery</button></a></p>");
                  
                  else if (desctiptionInv_ReadSignal)             client.println("<p><a href=\"/5/on\"><button class=\"button\">S-Inv. Read Signal!</button></a></p>");
                  else if (desctiptionPrg_StopInv)                client.println("<p><a href=\"/5/on\"><button class=\"button\">S-Prg. Stop Inverter!</button></a></p>");
                  else if (desctiptionPrg_StopInvTemp)            client.println("<p><a href=\"/5/on\"><button class=\"button\">S-Temp.Stop!</button></a></p>");
else if (ObjTriggerInv_Output220.getTriggeredAIReached())         client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:No ~220v Output " + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
                  else if (desctiptionInv_readAC)                 client.println("<p><a href=\"/5/on\"><button class=\"button\">S-No ~220v Output!</button></a></p>");
else if (ObjTriggerInv_ReadSignal.getTriggeredAIReached())        client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:Inv Read Signal " + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
else if (ObjTriggerPrg_StopInv.getTriggeredAIReached())           client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:Prg. Stop Inverter " + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
else if (ObjTriggerPrg_StopInvTemp.getTriggeredAIReached())       client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:Temperature " + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
else if (ObjSuddenVoltageChange.getTriggeredAIReached())          client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:Sudden Voltage Change" + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
                  else if (triggeredLongAITimeReached)            client.println("<p><a href=\"/5/on\"><button class=\"button\">Triggered:Protection AI  " + fungetfromatedTime(triggeredLongAITimeCnt) + "!</button></a></p>");
                  else if (turnOffTimer > 0 && doBatIsLow)        client.println("<p><a href=\"/5/off\"><button class=\"button\">Off After "+String(turnOffTimer)+"</button></a></p>");
                  else
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Inv Off</button></a></p>");

              else// give numeric timeout visualatation
                 if(!doBatIsLow)
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">"+String(delayAvoid_Inv_On)+"!</button></a></p>");
                  else
                  client.println("<p><a href=\"/5/on\"><button class=\"button\">Low Battery!</button></a></p>");
              //------------------------------------------------------
            }
            else
            { //+(delay_Inv_Output220_cnt > 0) ?+"P:"+String(delay_Inv_Output220_cnt): +"^"+

              if (delay_Inv_Output220_cnt > 2)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">P:"+String(delay_Inv_Output220_cnt-2)+" Inv On</button></a></p>");
              else if (turnOffTimer > 0 && doBatIsLow)
                  client.println("<p><a href=\"/5/off\"><button class=\"button\">Off After "+String(turnOffTimer)+"</button></a></p>");
              else if (doInv_Output220)
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">^Inv On</button></a></p>");
              else 
                 client.println("<p><a href=\"/5/off\"><button class=\"button button2\">xInv On</button></a></p>");
            }
                // Display current state, and ON/OFF buttons for GPIO 4
                // client.println("<p>GPIO 4 - State " + output4State + " "+ String (delay_Inv_Output220_cnt) +" b:"+String (doInv_Output220)+"</p>" );
                // client.println("<p> "+ getStatusText () +"</p>" );

                // If the output4State is off, it displays the ON button
                if (output4State == "off")
            {
              client.println("<p style=\"margin-bottom:1px\"><a href=\"/4/on\"><button s class=\"button\">Disabled Auto</button></a></p>");
            }
            else
            {
              client.println("<p style=\"margin-bottom:1px\" ><a href=\"/4/off\"><button  class=\"button button2\">Activated " + String(voltAvrBattery.voltage) + "v</button></a></p>");
            }

            //Javascript Battery Indicator in the top right corner
            client.println("\n  <script>\nfunction batIndicator(proc) {\n  if(proc>100 ) proc=100;if(proc<0 ) proc=0;\n   var procToByteInv =  255 - 2.55 * proc;\n   var procToByteRel =  1.9 * proc;\n   var procToByteGreen = 255 - (proc + 155);\n   var procf=proc;\n   \n   if (proc > 30)\n   document.getElementById(\"batteryLevel\").style.backgroundColor = \"rgba(\"+procToByteInv+\",\"+procToByteRel+\" , 25 , 0.8)\";\n    else\n      document.getElementById(\"batteryLevel\").style.backgroundColor = \"rgba(\"+procToByteInv+\",\"+ 0 +\", 25, 0.8)\";\n      \n\n      document.getElementById(\"batteryLevel\").style.width  = procf + \"%\"; /*position bar*/\n      document.getElementById(\"batteryLevel\").innerHTML = proc + \"%\";\n    }\n</script>\n");
            // cal Javascript function that show procentage in the battery in top right corner
            client.println("<script>batIndicator(" +String( (int)(((voltAvrBattery.voltage - minBatVlt) * 100) / (maxBatVlt  - minBatVlt)) )  +") </script>"); // recipe ((input - min) * 100) / (max - min)
            //Input configurations
            // Configuration input with submit
            client.println("<div>\n<form style=\"margin:5px;\" action=\"/configurations\"><label for=\"finput\"></label><input style=\"padding:10px;\" type=\"submit\" value=\"Command\"><input style=\"padding:10px; width: 160px;\"  type=\"text\" id=\"finput\" name=\"finput\"></form>\n</div>");
            // client.println("<form action=\"/configurations\"><input style= \"margin-top:2px\"  type=\"text\" id=\"finput\" name=\"finput\"><input  type=\"submit\" value=\"Submit\"><br></form><label for=\"finput\"></label>");

            // test cmd_received output
            // client.println("<p>NVD" + cmd_received + "  inderxf "+ String (cmd_received.indexOf ("=")) + "  http>" + String (cmd_received.indexOf ("HTTP"))  + "</p>");
            client.println("<p>Pout> " + /*cmd_received*/ cmd_msgOut + "</p>");

            // battery indicator
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }

     
      
    }
    // Clear the header variable
    // fun_CmdRead (header); // doesnt work cleanly
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

    

  }else {

    if (((long)clock_volatge + 2UL) < millis()) //1 miliseconds
    {
      clock_volatge=millis();

      voltAvrBattery.VoltageMeterUpdate(true); // update with each passed clock

    }
      // btnPrg_on.scaning();

      oneSecTimer();

    // btnPrg_on.endScaning();


   
 }
}

void __main () {
 

  sensorDoInv_readAC      = digitalRead (Inv_readAC);          // need more then 170v dc-ac or short a test points
  sensorDoInv_ReadSignal  = digitalRead (Inv_ReadSignal);      // __/ __ need contact to activate
  sensorDoPrg_StopInv     = digitalRead (Prg_StopInv);         // need source and ground to activate opticouple
  sensorPrg_StopInvTemp   = digitalRead (Prg_StopInvTemp);
  sensorProg_on_button    = !digitalRead (Prg_on_button);
  // sensorProg_on_button         = btnPrg_on.getBtnTPressLongTime (); // hold 3 seconds to activate and return true

  //if (btnPrg_on.onlyReadPressedSignleTime()){
  //          sensorProg_on_button = true;
  //}

  
  // btnPrg_on.endScaning ();
  

}

String NVal (String name , int value) {
  return " " + name + ": " + String (value);
}




// return desidion about sensor reactence----------------------------++++++++++++++++++
void descTxt (String txt , bool enable) {
  if (enable)
    desribtionsInTextSensing += txt;
}


bool reactionFromASensors (bool react) {
bool cnd = true; // failed to obtain condition from a sensors
desribtionsInTextSensing = ""; // clear each time

//  desctiptionInv_readAC
//  desctiptionInv_ReadSignal
//  desctiptionPrg_StopInv
//  desctiptionPrg_StopInvTemp
// ObjTriggerInv_Output220;
// ObjTriggerInv_ReadSignal
// ObjTriggerPrg_StopInv
// ObjTriggerPrg_StopInvTemp;
// ObjSuddenVoltageChange
// desctiptionSuddenVoltageChange


  if (!sensorDoInv_readAC && !desctiptionUserInv_readAC  && output5StateInvOutput == "on"   && delay_Inv_Output220_cnt < int(maxDelay_Inv_Output220_sec * delay_Inv_Output220_ReactRatio))         {/* if (!Activate220vReactionTimeUser) {cnd = false;}; no reaction time ! *//* cnd = false; */ desctiptionInv_readAC = true ; desctiptionInv_readACActive = true;  descTxt("No Inv.~220v output ,", react);}  else { desctiptionInv_readACActive = false; /* desctiptionInv_readAC = false; */}; 
  if (sensorDoInv_ReadSignal && !desctiptionUserInv_ReadSignal)  {/* cnd = false; */ desctiptionInv_ReadSignal = true ;descTxt("Read Inv. Signal ,", react);}else{desctiptionInv_ReadSignal = false;};
  if (sensorDoPrg_StopInv    && !desctiptionUserPrg_StopInv)     {/* cnd = false; */ desctiptionPrg_StopInv    = true ; descTxt("Prg. Stop Inverter ," , react);                    } else { desctiptionPrg_StopInv = false;};
  if (sensorPrg_StopInvTemp  && !desctiptionUserPrg_StopInvTemp) {/* cnd = false; */ desctiptionPrg_StopInvTemp = true ; descTxt("Stop inverter of critical temperature ," , react); } else { desctiptionPrg_StopInvTemp = false;};
  if (voltAvrBattery.voltage >= maxBatVlt) {desctiptionSuddenVoltageChange = true;} else if (voltAvrBattery.voltage <= minBatVlt) {desctiptionSuddenVoltageChange = false;};
  // triggeredTracketEventsMax

  
  //  if (cnd == false)
  //    triggeredAction = true ; // if any sensor is detected , will trigger a error AI but must be controlled with correct intervals becouse this go directly into counter without a stop
       ObjTriggerInv_ReadSignal.InteractionCountinerGlobalUseOneTime (); // must be used one time from any object
       
       ObjTriggerInv_Output220.setEnableTriggerEvents(Activate220vReactionTimeUser); // enable or disable in the trigger errors event
  if ( ObjTriggerInv_Output220.InteractionTime(desctiptionInv_readACActive, true ,int(Sustain220vReactionTimeUser))) {cnd = false;}; // only works if user is activated triggered protection
  // if (!Activate220vReactionTimeUser &&  !desctiptionInv_readACActive) {cnd = false;}; // doest work
  if (ObjTriggerInv_ReadSignal.InteractionTime(desctiptionInv_ReadSignal, true , 6)){cnd = false;}
  if (ObjTriggerPrg_StopInv.InteractionTime(desctiptionPrg_StopInv, true , 5)) {cnd = false;};
  if (ObjTriggerPrg_StopInvTemp.InteractionTime(desctiptionPrg_StopInvTemp,true , 6)) {cnd = false;};
  if (ObjSuddenVoltageChange.InteractionTime(desctiptionSuddenVoltageChange,true , 1) && ObjSuddenVoltageChange.getTriggeredAIReached()) {cnd = false;}; // react to a sudden voltage change from low to hign in wery short period of time 
  

  return cnd;
}

void cPrint () {
          if (ObjTriggerInv_Output220.getTriggeredTracketEventsAny()) serialPrintln5s(ObjTriggerInv_Output220.getStatementStr());
          if (ObjTriggerInv_ReadSignal.getTriggeredTracketEventsAny()) serialPrintln5s(ObjTriggerInv_ReadSignal.getStatementStr());
          if (ObjTriggerPrg_StopInv.getTriggeredTracketEventsAny()) serialPrintln5s(ObjTriggerPrg_StopInv.getStatementStr());
          if (ObjTriggerPrg_StopInvTemp.getTriggeredTracketEventsAny()) serialPrintln5s(ObjTriggerPrg_StopInvTemp.getStatementStr());
          if (ObjSuddenVoltageChange.getTriggeredTracketEventsAny()) serialPrintln5s(ObjSuddenVoltageChange.getStatementStr());

          if (!doReactInBatVlt) serialPrint1s ("auto mode is disabled and / ");
          // if (triggeredLongAITimeReached) serialPrintln3s ("-->Initiated  triggered timeout " + fungetfromatedTime (triggeredTimeoutCnt) + " " + String (triggeredTracketEventsCnt) + " / " + String (triggeredTracketEventsMax));
           if (triggeredLongAITimeReached) serialPrintln5s ("triggered AI protection :O "+ fungetfromatedTime (triggeredLongAITimeCnt) + " and /");

}
void quarterSecondTimer () { //0.1 second

   __main();


  if (((long)clock_0_1sec + 100UL) < millis())

  {
      //turnOnByButtonEvent ();
      clock_0_1sec= millis();
      clock_1secCounter  = clock_1secCounter + 1;
      clock_3secCounter  = clock_3secCounter + 1;
      clock_5secCounter =  clock_5secCounter + 1;

      if (output5StateInvOutput == "on")
          digitalWrite (Inv_On, HIGH);

       // blink led while turning on a 220v relay 
      
      if (LED_IndicatorBlinkFast > 0){
          LED_IndicatorBlinkFast--;
          LED_IndicatorBlink = !LED_IndicatorBlink;
        
      if (!LED_IndicatorBlink)//blick when turning on relay
        {
          digitalWrite (LED_Indicator, LOW);

        }else{
          digitalWrite (LED_Indicator, HIGH);
        }

      }else if (delay_Inv_Output220_cnt  == 0) { // ignoring a to interrput blinking light from a 220v relay ouput from inverter
        // return to a current state of a program
        if (output5StateInvOutput == "on")
          digitalWrite (LED_Indicator, HIGH);
        else
          digitalWrite (LED_Indicator, LOW);

      }


    


     //Sensor
      sensorRead_Battery_Volt = analogRead(Read_Battery_Volt);



     //sec delay to pass power throw power relay from inverter
     if (delay_Inv_Output220_cnt < 3 &&  delay_Inv_Output220_cnt >= 1) {

       doInv_Output220 = true; // to avoid cycling maxDelay 

       digitalWrite (Inv_Output220,HIGH);

    }
    //----------------------------------------------------------

    //sec delay to turn to on inverter to fast, avoid toggle switching
    if (delayAvoid_Inv_On > 0){

      doAvoidInv_On = true;
    }else{
      doAvoidInv_On = false;
    }
    //---------------------------------------------------------------

  





//---------------// React to turning on a inverter by voltage range ------------//660


    String invTxt = "";
      if (doReactInBatVlt/*<auto on condition from user*/ && !doBatMaxVltReached && (voltAvrBattery.voltage >= maxBatVlt) && reactionFromASensors() /*and no reaction from a sensors*/ && !triggeredLongAITimeReached /*AI not triggered*/) // turn on a inverter
        {
      
       if (ObjTriggerInv_Output220.getTriggeredTracketEventsAny()) serialPrintln5s(ObjTriggerInv_Output220.getStatementStr()); // just print about what its happening with inverter 220v output
       
       if (timer1sec ())
         {
           serialPrintln1s ("Condition: sensor " + String (voltAvrBattery.voltage) +"v is more then maximum batery voltage " + String ( maxBatVlt) + "v " );
           funTurnOffTimer(true);
          if (maxBatVltSustained_cnt <= maxBatVltSustained_sec && timer1sec ()) // keep counting in each second timer1sec ()
              maxBatVltSustained_cnt++;


            serialPrintln1s ("sustaining max voltage: " + String (maxBatVltSustained_cnt) + " of " +String (maxBatVltSustained_sec) + " as " + fungetfromatedTime (maxBatVltSustained_cnt) +  " -_- " );
            doBatBeHigh = true; // sub condition, telling about high voltage at the moment
          if (maxBatVltSustained_cnt >= maxBatVltSustained_sec) // when voltage was sustained more then 60 seconds when turn on a inverter 
             {
              doBatMaxVltReached = true;
              funInv_On_then_Output220 ("on",false);

              serialPrintln1s ("turn on inv  when voltage is in healthy level :)");
             }
          }
         }
   //one of the sensor is activated       
          else if (!reactionFromASensors ())
       {

          cPrint(); // output print conditions
          reactionFromASensors (true);
          serialPrintln5s("Failed becouse of sensors ERR: " + desribtionsInTextSensing + " full condition: " + desribtionsInTextAI);


              // if (!triggeredLongAITimeReached)
                  // serialPrintln3s("-->Initiated triggered timeout " + fungetfromatedTime(triggeredTimeoutCnt) + " " + String(triggeredTracketEventsCnt) + " / " + String(triggeredTracketEventsMax));
          if (triggeredLongAITimeReached)
             serialPrintln3s("triggered AI protection :O " + fungetfromatedTime(triggeredLongAITimeCnt) + " and /");

          funInv_On_then_Output220 ("off",false);
          
          doBatIsLow = false;
          doBatBeHigh = false;
          maxBatVltSustained_cnt = 0;
       }
      else if (voltAvrBattery.voltage <= minBatVlt ) // when battery is to low then turn of inverter and relay 
      {


             if (turnOffTimer > 0) // avoid to turn off to fast
           {  
             
              serialPrintln1s ("turnOffTimer: " + String (turnOffTimer) + " , ");
             if (timer1sec ())    turnOffTimer--; // timer1sec () give as one second timer 
                
           } else {
                 doBatMaxVltReached = false;
                 maxBatVltSustained_cnt = 0;
                 funInv_On_then_Output220 ("off",false);
           }

           cPrint(); // output print conditions

               serialPrintln1s("inv is off when low battery v " + String(voltAvrBattery.voltage));

           doBatIsLow = true;
           doBatBeHigh = false;
      }

      else // if voltage between low or high or reached high voltage by itself and exit from that condition after sustained 'maxBatVltSustained_cnt' time
      {

     
        maxBatVltSustained_cnt = 0;
        doBatIsLow = false;
        doBatBeHigh = false;

        cPrint(); // output print conditions

        if (voltAvrBattery.voltage >= maxBatVlt)
        serialPrintln3s ("high battery condition but no events" );
        else
        serialPrintln3s("no battery condition v " + String(voltAvrBattery.voltage));

        funTurnOffTimer(true);  
       
      }




  }

}

void oneSecTimer () {

      

  quarterSecondTimer () ;
     
  if (timer5sec()) {
            clock_5secCounter = 0;
            funTimeClient();
  }


  if (timer3sec ())
            clock_3secCounter = 0; //reset

 if (timer1sec ()){
          clock_1secCounter = 0;

          if (regulateComandLineOutput > 0)
          {
            regulateComandLineOutput--;
          }else
          {
            cmd_msgOut = ""; //reset Pout command text
          }

        if (cmd_received.length() > 0)
        {
        fun_CmdRead(cmd_received);
        cmd_received = "";
        }


          if (delayAvoid_Inv_On > 0){
            delayAvoid_Inv_On--; // delay given to avoid turn on to fast
           }
///////////////////////////////////////////////////////////////////////////////////   AI Section  ///////////////////////////////////////////////////////////////////////////
    //  // enable timeout only one time after a triggeredAction event
    // if (triggeredAction  && triggeredTimeoutCnt == 0) { 
    //     triggeredTimeoutCnt = triggeredTimeoutMax;
    // }

    // // count down a timeout where more could accures triggeredAction
    // if (triggeredTimeoutCnt > 0)  triggeredTimeoutCnt--;

    // //  time where triggered actioct will be tracked in 10 minutes period
    // if (triggeredTimeoutCnt > 0  && triggeredAction){

    //         Serial.println ("Reached triggeredTracketEvents");
    //   // keep counting triggered actioct
    //   if (triggeredTracketEventsCnt < triggeredTracketEventsMax - 1) triggeredTracketEventsCnt ++;
    //   // reached maximum error AI level where will enable program waiting for a long time after many failures from a sensors or a program attempts to work propietly
    //   else {Serial.println ("Maxed out triggeredTracketEventsMax"); triggeredLongAITimeReached = true; };
    // }

    // // initiate long time timer
    // if (triggeredLongAITimeReached && triggeredLongAITimeCnt == 0)
    //     triggeredLongAITimeCnt = triggeredLongAITimeMax;

    // // keep holding a progrom to work about 6h
    // if (triggeredLongAITimeCnt > 0) {

    //         triggeredLongAITimeCnt--;
    //     if (triggeredLongAITimeCnt == 2)
    //         desribtionsInTextAI = "" ;// clear text
    //     }

    // else{ 
    //    triggeredLongAITimeReached = false; // reset AI 
    //    triggeredTimeoutCnt = 0;
    //    }
 ///////////////////////////////////////////////////////////////////////////////////AI Section END///////////////////////////////////////////////////////////////////////////

       // reset triggeredAction for another trigger event
       triggeredAction = false;
       // represent what happening in AI
       // if (triggeredTimeoutCnt > 0 && !triggeredLongAITimeCnt)
       //   Serial.println (" Initiated triggered timeout " + fungetfromatedTime (triggeredTimeoutCnt) + " " + String (triggeredTracketEventsCnt) + " / " + String (triggeredTracketEventsMax));

       //---------------Program Button are pressed------------------------------//

       if (sensorProg_on_button)
       {
       Serial.println("Pressing a button " + String(delayPrg_on_button) + "  %  " + String(delayPrg_on_button % 2 == 1));

       if (delayPrg_on_button < maxDelayPrg_on_button)
       {
        delayPrg_on_button++;

        // when pressing a button led inditcator should blink normaly when reached doPrg_on_button for a posetive result
        if (delayPrg_on_button % 2 == 1) // then button was successfully pressed all the way down
              digitalWrite(LED_Indicator, LOW);
        else
              digitalWrite(LED_Indicator, HIGH);
       }

       if (delayPrg_on_button == maxDelayPrg_on_button - 1)
       {
        doPrg_on_button = !doPrg_on_button; // here change bolean condition

        if (doPrg_on_button) // turn on inverter and passing power relay
        {
              funInv_On_then_Output220("on");
              funResetTriggeredAction();
        }
        else
        {
              funInv_On_then_Output220("off");
        }
       }

       else if (delayPrg_on_button == maxDelayPrg_on_button)
       { // fast blink then reached a condtion
        LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
        // Serial.print("  it fast blinks +++  button: " + String(doPrg_on_button));
       }

             


              




           } else {
             delayPrg_on_button = 0;
           }


///////////////////////////// // second relay that pass power throw power 220v relay
          if (delay_Inv_Output220_cnt > 0){
                delay_Inv_Output220_cnt--; // substrack when "inv_on" relay is activated
                

                // blink led while turning on a 220v relay 
                if (!doInv_Output220 && delay_Inv_Output220_cnt % 2 == 1 || doInv_Output220 )//blick when turning on relay
                   digitalWrite (LED_Indicator, HIGH);
                 else
                   digitalWrite (LED_Indicator, LOW);

                 Serial.println ("delay_Inv_Output220_cnt: " + fungetfromatedTime (delay_Inv_Output220_cnt));
                 //--------------------
              }

          
          // Serial.println (getStatusText ());

          
    }

}


/*int maxBatVltSustainedMax_sec = 60;
int maxBatVltSustained_sec = maxBatVltSustainedMax_sec; //  if not interuption in 30 seconds have in reached by maximum voltage range then
int maxBatVltSustained_cnt = 0; //  keep count of maxBatVltSustained_sec
byte maxBatVltSustained_User;*/



void funDelay_Inv_Output220 (bool enableTimer) {
   maxDelay_Inv_Output220_sec = (int)delay_Inv_Output220Max_sec + ((int)delay_Inv_Output220_User);
}

void funmaxBatVltSustained (bool enableTimer) {
    maxBatVltSustained_sec = (int)maxBatVltSustainedMax_sec + ((int)maxBatVltSustained_User);
   // if (emableTimer) turnOffTimer = turnOffTimerMax; // cal working in other place
}

void funTurnOffTimer (bool enableTimer) {/// turn off timer calculation
       turnOffTimerMax = (int)turnOffTimerMaxConst + ((int)turnOffTimerUser);
   if (enableTimer)     turnOffTimer = turnOffTimerMax;
}

// turn on both relays
void funInv_On_then_Output220 (String x  , bool silence) {

   if  (x == "on"){
     // digitalWrite (Inv_Output220, HIGH);
      // digitalWrite (Inv_On, HIGH);
      output5StateInvOutput = "on"; 
      desctiptionInv_readAC = false; // only this sensor repot requared to reset 
      funTurnOffTimer (true);

      if (delay_Inv_Output220_cnt == 0 && !doInv_Output220 ){ // pass value only one time when was turned on at least one time
         delay_Inv_Output220_cnt = maxDelay_Inv_Output220_sec; 
         delayAvoid_Inv_On = maxDelayAvoid_Inv_On;  // activate protection against turning on to fast and multiple times
         doPrg_on_button = true; // only for in sinck with website button
         desribtionsInTextAI = "";
          if (silence) Serial.println("funInv_On_then_Output220:on");
       }

      
      }
   else{
      digitalWrite (Inv_Output220, LOW);
      digitalWrite (Inv_On, LOW);
      digitalWrite (LED_Indicator, LOW);
      output5StateInvOutput = "off"; // is opposite
       if (silence) Serial.println ("funInv_On_then_Output220:off");
      delay_Inv_Output220_cnt = 0;
      doInv_Output220 = false;
      doPrg_on_button = false;
      doBatMaxVltReached = false;
      maxBatVltSustained_cnt = 0; // reset
      desctiptionInv_readAC = false; // only this sensor repot requared to reset
   }
}

void funTurnInvAutomaticallyByVoltage () {

  // if ( voltAvrBattery.voltage < minBatVlt){

  // }

  // if (voltAvrBattery.voltage > maxBatVlt)
}

String getText (String index, bool conditionNaming){
  if (conditionNaming)
    return ",  " + index + ": true";
  else
    return ",  " + index + ": false";

}


bool isValidNumber(String str){
    bool isNum=false;
    for(byte i=0;i<str.length();i++)
    {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if(!isNum) return false;
    }
    return isNum;
}

 

String getStatusText () {
 return   
            (". bits:" + String (sensorRead_Battery_Volt) +" = "
  + String  (voltAvrBattery.voltage) + "v.  "
  + getText ("DoInv_readAC D0 ~220v",sensorDoInv_readAC) 
  + getText ("  DoInv_ReadSignal D5_/_",sensorDoInv_ReadSignal)
  + getText ("  DoPrg_StopInv D6 +48v",sensorDoPrg_StopInv) 
  + getText ("  btnPrg_on D3 _/_", doPrg_on_button ) 
  + getText ("  StopInvTemp D7_/_", sensorPrg_StopInvTemp ) 
  + getText ("  doBatMaxVltReached", doBatMaxVltReached ) 
  + getText ("  IgnoreInv_readAC", desctiptionUserInv_readAC ) 
  + getText ("  IgnoreInv_ReadSignal", desctiptionUserInv_ReadSignal ) 
  + getText ("  IgnorePrg_StopInv", desctiptionUserPrg_StopInv ) 
  + getText ("  IgnorePrg_StopInvTemp", desctiptionUserPrg_StopInvTemp ) 
  +          "  Inv_TimeReact~220v ->" + ( maxDelay_Inv_Output220_sec * delay_Inv_Output220_ReactRatio ) + "s before transfer relay is on"
  + getText ("  doInv_Output220v  " , sensorDoInv_readAC) + " -> reacted: " + ObjTriggerInv_Output220.getTriggerActionCnt() * 100 + "ms"
  + getText ("  doReactInBatVlt", doReactInBatVlt ) 
  + "//" );
 
}

// int intDataTipe (int & changeOriginal , String  ) {

// }

String fun_CmdRead (String cmdRead /*input commands here*/) 
{
   String cmdGetSpecial; // special value simbol begins from '-'
   int    cmdGetSpecialInt; // value convert into integer 
   bool   cmdIsValidInt = false;
   String cmdNRead = cmdRead;
   bool sP = false;
    // Serial.println ("+++++++++++++++++++++++++++full link> " + cmdRead);

   regulateComandLineOutput = regulateComandLineOutputMax; // set timeout for clearing  cmd_msgOut text

   if (cmdRead.length() > 0)
   { // read comed arround

    // cmdRead = cmdRead.substring (0,cmdRead.length()-1); //to  remove incoming '\n'
    // Serial.println ("------------found Text:" + cmdRead);

    cmd_msgOut = ""; // reset

    if (cmdRead.lastIndexOf("-") > 0)
    {
          cmdGetSpecial = cmdRead.substring (cmdRead.lastIndexOf("-") + 1 ,cmdRead.length()); //separate text after-
          cmdRead = cmdRead.substring (0,cmdRead.indexOf("-"));

           if (cmdGetSpecial.indexOf ("+"))//remove spaces
         {
          Serial.println ("found+++ Spaces +++: " + String (cmdGetSpecial.indexOf ("+")) + "~" + String (cmdGetSpecial.length ()) );
          cmdGetSpecial = cmdGetSpecial.substring  ( 0 , cmdGetSpecial.indexOf ("+")); // delete leave text before spaces '+' begins
          Serial.println ("rslt: " + cmdGetSpecial );
         }

          //to integer
          cmdGetSpecialInt = (cmdGetSpecial.toInt());

          // check in string is a valid integer without any characters
          cmdIsValidInt = isValidNumber (cmdGetSpecial);

          Serial.println ("comand: "  + cmdRead) ;
          Serial.println ("-value: " + cmdGetSpecial);
          Serial.println ("-valueInt: " + String (cmdGetSpecialInt));
          Serial.println (getText ("is integer data type: ", isValidNumber (cmdGetSpecial)  ));
          
         }

  


        if (cmdRead == "help")
         {

          cmd_msgOut = "Awailable commands:,fixVltR-byte,fixTurnOffLowVoltTimer-byte,fixSustainedMaxVoltTimer-byte,fixDelayInvOutputRelay220v-byte,Sustain220vReactionTime,maxBatVlt-byte,minBatVlt-byte,IgnoreSensorReadAC-bool,IgnoreSensorReadInvSignal-bool,ActivateTriggeredAC220vProtection-bool,IgnoreSensorPrgStopInv-bool,IgnoreSensorPrgStopInvTemp-bool,IgnoreAllSensors,clear,status,restart,resetWifi-intpswrd, ";
          Serial.println(cmd_msgOut);
         }
//          delay turn on 220v relay to a home before chenking its all right with inverter         
          else if (cmdRead == "fixDelayInvOutputRelay220v" ){
            funDelay_Inv_Output220 (); // refresh calculation values
           if (cmdIsValidInt) 
            {  
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
              delay_Inv_Output220_User = (byte)cmdGetSpecialInt;
              funDelay_Inv_Output220 (); // refresh calculation values
              cmd_msgOut += cmdRead + " a value: " + String(delay_Inv_Output220_User) + " as " + String(maxDelay_Inv_Output220_sec) + "s timer  and start to check if inverter output 220v after " + String(int(maxDelay_Inv_Output220_sec * delay_Inv_Output220_ReactRatio)) + "s left.";
              writeMemory(memDelay_Inv_Output220,(byte)delay_Inv_Output220_User);

            } 
            else {cmd_msgOut+="Failed register "+cmdRead+" a mem value: " + String (delay_Inv_Output220_User) + " as " + String (maxDelay_Inv_Output220_sec) + "s timer  and start to check if inverter output 220v after " + String(int(maxDelay_Inv_Output220_sec * delay_Inv_Output220_ReactRatio)) + "s left.";};
         }

//          Low Voltage         
          else if (cmdRead == "fixTurnOffLowVoltTimer" ){
            funTurnOffTimer (); // refresh calculation values
           if (cmdIsValidInt) 
            {  
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
              turnOffTimerUser = (byte)cmdGetSpecialInt;
              funTurnOffTimer (); // refresh calculation values
              cmd_msgOut+= cmdRead + " a value: " + String (turnOffTimerUser) + " as " + String (turnOffTimerMax) + "s timer ";
              writeMemory(memturnOffTimer,(byte)turnOffTimerUser);

            } 
            else {cmd_msgOut+="Failed register "+cmdRead+" a mem value: " + String (turnOffTimerUser) + " as " + String (turnOffTimerMax) + "s timer ";};
         }
//            High voltage 
          else if (cmdRead == "fixSustainedMaxVoltTimer" ){
            funmaxBatVltSustained (); // refresh calculation values
           if (cmdIsValidInt) 
            {  
              LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
              maxBatVltSustained_User = (byte)cmdGetSpecialInt;
              funmaxBatVltSustained (); // refresh calculation values
              cmd_msgOut+= cmdRead + " a value: " + String (maxBatVltSustained_User) + " as " + String (maxBatVltSustained_sec) + "s timer ";
              writeMemory(memBatVltSustained,(byte)maxBatVltSustained_User);

            } 
            else {cmd_msgOut+="Failed register "+cmdRead+" a mem value: " + String (maxBatVltSustained_User) + " as " + String (maxBatVltSustained_sec) + "s timer  ";};
         }
    
             //        AC ~220 voltage sensor
             else if (cmdRead == "IgnoreSensorReadAC")
         {
            if (cmdIsValidInt) 
            {
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 cmd_msgOut+= cmdRead + " ["+  String ((byte)cmdGetSpecialInt) + "]" + "from a was value: " + String (readMemoryByte (memInv_readAC) );
                 writeMemory (memInv_readAC, (byte)cmdGetSpecialInt);
                 desctiptionUserInv_readAC = bool (cmdGetSpecialInt);
            }
          else { cmd_msgOut += cmdRead + " [not a bool],stored: "+ String (readMemoryByte (memInv_readAC))  ;};
         }
         //     Enable  AC ~220 voltage triggered protection
         else if (cmdRead == "ActivateTriggeredAC220vProtection")
         {
          if (cmdIsValidInt)
          {
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 cmd_msgOut += cmdRead + " [" + String((bool)cmdGetSpecialInt) + "]" + "from a was value: " + String(readMemoryBool(memActivate220vReactionTime));
                 writeMemory(memActivate220vReactionTime, (bool)cmdGetSpecialInt);
                 Activate220vReactionTimeUser = bool(cmdGetSpecialInt);
          }
          else
          {
                 cmd_msgOut += cmdRead + " [not a bool],stored: " + String(readMemoryBool(memActivate220vReactionTime)) + " works as enabling for a triggered protection";
          };
         }
//        Inverter read condtional sensor 
         else if (cmdRead == "IgnoreSensorReadInvSignal" ){
           if (cmdIsValidInt) 
            {
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 cmd_msgOut+= cmdRead + " ["+  String ((byte)cmdGetSpecialInt) + "]" + "from a was value: " + String (readMemoryByte (memInv_ReadSignal) );
                 writeMemory (memInv_ReadSignal, (byte)cmdGetSpecialInt);
                 desctiptionUserInv_ReadSignal = bool (cmdGetSpecialInt);

            }
          else {
                 cmd_msgOut += cmdRead + " [not a bool],stored: " + String(readMemoryByte(memInv_ReadSignal));
          };
        }
//        Stop inverter / Pause condtional sensor 

         else if (cmdRead == "IgnoreSensorPrgStopInv" ){
           if (cmdIsValidInt) 
            {
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 cmd_msgOut+= cmdRead + " ["+  String ((byte)cmdGetSpecialInt) + "]" + "from a was value: " + String (readMemoryByte (memPrg_StopInv) );
                 writeMemory (memPrg_StopInv, (byte)cmdGetSpecialInt);
                 desctiptionUserPrg_StopInv = bool (cmdGetSpecialInt);

            }
          else {
                 cmd_msgOut += cmdRead + " [not a bool],stored: " + String(readMemoryByte(memPrg_StopInv));
          };
        }
//       Stop inverter by extreme cold or worm condtional sensor 

         else if (cmdRead == "IgnoreSensorPrgStopInvTemp" ){
           if (cmdIsValidInt) 
            {
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 cmd_msgOut+= cmdRead + " ["+  String ((byte)cmdGetSpecialInt) + "]" + "from a was value: " + String (readMemoryByte (memPrg_StopInvTemp) );
                 writeMemory (memPrg_StopInvTemp, (byte)cmdGetSpecialInt);
                 desctiptionUserPrg_StopInvTemp = bool (cmdGetSpecialInt);

            }
          else {
                 cmd_msgOut += cmdRead + " [not a bool],stored: " + String(readMemoryByte(memPrg_StopInvTemp));
          };
        }

//        IgnoreSensor all  condtional sensors 

         else if (cmdRead == "IgnoreAllSensors") {
          writeMemory (memInv_readAC, true);
          writeMemory (memInv_ReadSignal, true);
          writeMemory (memPrg_StopInv, true);
          writeMemory (memPrg_StopInvTemp, true);
          desctiptionUserInv_readAC = true; 
          desctiptionUserInv_ReadSignal = true; 
          desctiptionUserPrg_StopInv = true; 
          desctiptionUserPrg_StopInvTemp = true; 
          cmd_msgOut += "Changed to ignore all sensors " + desribtionsInTextSensing;
        }
//---------------------------End of desctriptionUser memory variables
         else if (cmdRead == "fixVltR" ){
           cmd_msgOut = "Registered fix voltage of R resistor 1 byte is equal -> 0.01: ";

            if (cmdIsValidInt) 
            {  
                LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                voltAvrBattery.FixVltR((byte)cmdGetSpecialInt); 
                cmd_msgOut+="["+  String ((byte)cmdGetSpecialInt) + "]" + " a value: " + String (voltAvrBattery.getFixVltR());
               
                writeMemory (memfixVltR, (byte)cmdGetSpecialInt);
             } else { cmd_msgOut += " [not a byte],stored: "+ String (readMemoryByte (memfixVltR)) + "b a value: " +  String (voltAvrBattery.getFixVltR()) + "R" ;};

           Serial.println (cmd_msgOut);
         }

         else if (cmdRead == "Sustain220vReactionTime")
         {
           cmd_msgOut = "React to a 220v output where was lower then expected in the of a certain time ";
           if (cmdIsValidInt)
           {
                LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                cmd_msgOut += cmdRead + " [" + String((byte)cmdGetSpecialInt * clock_1secValue * 10) + "ms]" + "from a was value: " + String(readMemoryByte(memSustain220vReactionTime) * clock_1secValue * 10) + "ms";
                writeMemory(memSustain220vReactionTime, (byte)cmdGetSpecialInt);
                Sustain220vReactionTimeUser = byte(cmdGetSpecialInt);
           }
           else
           {
                cmd_msgOut += cmdRead + " [not a byte],stored: " + String(readMemoryByte(memSustain220vReactionTime) * clock_1secValue * 10) + "ms";
           };
         }

          else if (cmdRead == "clear" ){
           cmd_msgOut = "";
            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
         }

         else if (cmdRead == "status"){

            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
            cmd_msgOut = "Received maxBatVlt:" + String (maxBatVlt) + " minBatVlt:" + String (minBatVlt) + getStatusText () ;
            Serial.println (cmd_msgOut);
            

         }
         else if (cmdRead == "resetWifi"){
            cmd_msgOut = "Received, reset wifi settings:";
             if (cmdIsValidInt)
             {
               
                if (cmdGetSpecialInt == 1357) {
                      LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                      Serial.println (cmd_msgOut);
                      wifiManager.resetSettings();
                      ESP.restart();
                }else
                  cmd_msgOut+= " [wrong password!]";
             }else
                  cmd_msgOut+= " [enter a numeric password!]";
            
         }
          else if (cmdRead == "restart"){
            LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
            cmd_msgOut = "Received, start restart os:";
            Serial.println (cmd_msgOut);
            ESP.restart();
            
         }
         else if (cmdRead == "minBatVlt"){
           
             if (cmdIsValidInt){
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 minBatVlt = cmdGetSpecialInt;
                  
               if (avoidBatVltOutOfRangeThenMemCommit  () )
                     {cmd_msgOut = ("Registered minimum battery voltage: " + cmdGetSpecial  ); Serial.println (cmd_msgOut);}
               else
                     {cmd_msgOut = ("Failed to registered minimum battery voltage, out of range: " + cmdGetSpecial)  ; Serial.println (cmd_msgOut);}

                 Serial.println ("min out");

             }
            else 
              // sP = true; // failed to proceed whithout a proper number
                 cmd_msgOut += cmdRead + " [not a byte],stored value: " + String(readMemoryByte(memMinBatVlt)) + "v";
         }
         else if (cmdRead == "maxBatVlt"){
            
           
            if (cmdIsValidInt){
                 LED_IndicatorBlinkFast = LED_IndicatorBlinkFast_Common;
                 maxBatVlt = cmdGetSpecialInt;
                if ( avoidBatVltOutOfRangeThenMemCommit  ()  )
                  {cmd_msgOut = ("Registered maximum battery voltage:" + cmdGetSpecial) ; Serial.println (cmd_msgOut);}
                else
                  {cmd_msgOut = ("Failed to registered maximum battery voltage, out of range:") + cmdGetSpecial ; Serial.println (cmd_msgOut);}

                 Serial.println ("max out");
             }
            else
              // sP = true; // failed to proceed whithout a proper number
                 cmd_msgOut += cmdRead + " [not a byte],stored value : " + String(readMemoryByte(memMaxBatVlt)) + "v";
         }

         else 
          {
           cmd_msgOut ="For more information type 'help' ( " + cmdRead + " - " + cmdGetSpecial +" ?)" ;
           Serial.println (cmd_msgOut);
          //Serial.println ("No command found DDDDDD:"  + cmdNRead + " ,^^ " + cmdRead.substring (cmdRead.indexOf ( " H") , cmdRead.length()) ) ;
          // Serial.println ("Received " +String (cmdRead.length()) + ">" + String (cmdRead) + String (cmdRead == "help") + " == help");
          }
       
           


              if (sP) 
              {
                      Serial.println ("Unable to proceed");
                      cmd_msgOut += cmdRead + " [err:Unable to proceed , no integer] ";
              }
   }

  return "   ";
}




void setBatVltRange (uint8_t minBatVlt_ , uint8_t maxBatVlt_) // not in use
{

  minBatVlt = minBatVlt_;
  maxBatVlt = maxBatVlt_;

  if (minBatVlt > maxBatVlt) // if minimum voltage is higher then maximum then make as same level to with
  {
    maxBatVlt = minBatVlt_ + 1;
    Serial.println ("maximum voltage was reseted");
  }

  // if (maxBatVlt > ) // voltage range
}






bool avoidBatVltOutOfRangeThenMemCommit (){
  bool itsDone = true;// chenk if all condition passed  successfully

   if (minBatVlt > maxBatVlt) // if minimum voltage is higher then maximum then make as same level to with
  {
    maxBatVlt = minBatVlt + 1;
    Serial.println ("minimum voltage was higher then expected, reseted");
    itsDone = false; // at some point its failed
  }

 
      if (minBatVlt < 10 &&  maxBatVlt <= 16)
      {
        Serial.println ("12v mode");
      }
 else if (minBatVlt < 24 &&  maxBatVlt <= 32)
      {
        Serial.println ("24v mode");

      }
 else if (minBatVlt < 36 &&  maxBatVlt <= 48)
      {
        Serial.println ("36v mode");

      }
 else if (minBatVlt < 54 &&  maxBatVlt <= 64)
      {
        Serial.println ("48v mode");

      }
  else
  {
         itsDone = false;
    Serial.println ("doesnt found range in 'avoidBatVltOutOfRange' funcion");
  } 

  if (itsDone) // prepare to commit into EEPROM memory
  {
    Serial.println ("successfully updated battery variables values to EEPROM");

    writeMemory(memMinBatVlt,minBatVlt);
    writeMemory(memMaxBatVlt,maxBatVlt);
    return true;
  }else
  {
    Serial.println ("Unable to save battery variables values to EEPROM");
  }

 if (maxBatVlt >= 64)
 {
  maxBatVlt = 64;
 }

 if (minBatVlt >= 63)
 {
  minBatVlt = 63;
 }

  return false;
}







//==========
void getmemBatVlt () {
    Serial.println ("Received battery voltage range from memory");
  minBatVlt = readMemoryByte(memMinBatVlt);
  maxBatVlt = readMemoryByte(memMaxBatVlt);

  if (minBatVlt > 62 || maxBatVlt > 64)
  {
    minBatVlt = 42;
    maxBatVlt = 64;

    writeMemory(memMinBatVlt,minBatVlt);
    writeMemory(memMaxBatVlt,maxBatVlt);

    Serial.println("Changing bad boundaries having values");
  }

}



// String h = (getHours() > 0) ? String(getHours()) +"h " : " " ;
//               String m = (getMinutes() > 0) ? String(getMinutes()) +"m " : " " ;
//               String s = (getSeconds() > 0) ? String(getSeconds()) +"s " : " " ;


String fungetfromatedTime (signed  int seconds)
 {
  String txt;
   int h = (seconds  % 86400L) / 3600;
   int m = (seconds  % 3600) / 60;
   int s = (seconds  % 60) ;

   if (h > 0) //hours
      txt +=String(h) + "h ";
  
   if (m > 0 && h == 0) // min  // m == 0 to save space and only show hours
      txt +=String(m) + "m ";
  
   if (s > 0 && h == 0 && m == 0) //seconds then minints and last will be a seconds
      txt += String(s) + "s" ;

  return txt;
 }

void funResetTriggeredAction () {
    triggeredTimeoutCnt = 0;
    triggeredLongAITimeCnt = 0;
    triggeredLongAITimeReached = false;
    triggeredTracketEventsCnt = 0;
    triggeredAction = false;

    ObjTriggerInv_Output220.resetErrors();
    ObjTriggerInv_ReadSignal.resetErrors();
    ObjTriggerPrg_StopInv.resetErrors();
    ObjTriggerPrg_StopInvTemp.resetErrors();
    ObjSuddenVoltageChange.resetErrors();
 }

 void funTimeClient()
 {

    timeClient.update();
    /*
    //Serial.print(daysOfTheWeekLT[timeClient.getDay()]);
    //Serial.print(", ");
    //Serial.print(timeClient.getHours());
    //Serial.print(":");
    //Serial.print(timeClient.getMinutes());
    //Serial.print(":");
    //Serial.println(timeClient.getSeconds());
    */

    // currentTimeHeader = String(daysOfTheWeekLT[timeClient.getDay()]) + "<br> " + timeClient.getFormattedTime(); // String(daysOfTheWeekLT[timeClient.getDay()]) + ", " + String(timeClient.getHours()) + ": " + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());
    currentTimeHeader = String(daysOfTheWeekLT[timeClient.getDay()]) + " " + String(timeClient.getHours()) + ":" + String(timeClient.getMinutes());
    
    // Serial.print (currentTimeHeader);
    // delay (1000);
 }

bool   timer1sec () {
  if (clock_1secCounter >= clock_1secValue) return true; else return false;
}
bool   timer3sec (){
  if (clock_3secCounter >= clock_3secValue ) return true; else return false;
}
bool   timer5sec (){
  if (clock_5secCounter >= clock_5secValue ) return true; else return false;
}


void   serialPrint1s (String txt){
if (timer1sec ())
    Serial.print (txt);
}
void   serialPrintln1s (String txt){
if (timer1sec ())
    Serial.println (txt);
}

void   serialPrint3s (String txt) 
{
  if (timer3sec ())
    Serial.print (txt);
}

void   serialPrintln3s (String txt) 
{
  if (timer3sec ())
    Serial.println (txt);
}

void serialPrintln5s(String txt)
{
  if (timer5sec())
    Serial.println(txt);
}