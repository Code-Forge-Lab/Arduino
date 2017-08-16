#include "Arduino.h"
#include "PWM.h"
//Arduino Uno
static const int PIN_LCD_BackgroundLight = 9;
static const int PINREAD_BatteryVolt = A1;
static const int PIN_VoltageRegulation = 10;
static const int PIN_PulseLED = 13;
int32_t frequency = 9;
bool    PWM_Success;

static const float BatteryVolt_R1 = 100000.0;
static const float BatteryVolt_R2 = 10000.0;
float BatteryVolt_voltage = 0.0; //save voltage of battery
float BatteryMaxVoltCharged = 21.62;
float BatteryMinVoltCharged = 18.0;
bool BatteryIsCharged = false;
#include "Voltmeter.h"
#include "Icon.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16



void setup()   {
  InitTimersSafe();
  //sets the frequency for the specified pin


Serial.begin(9600);
display.begin();

//initialize all timers except for 0, to save time keeping functions
 PWM_Success = SetPinFrequencySafe(PIN_VoltageRegulation, frequency);

 pinMode (PIN_LCD_BackgroundLight,OUTPUT);
 pinMode (PINREAD_BatteryVolt,INPUT);
 pinMode (PIN_VoltageRegulation,OUTPUT);
 pinMode (PIN_PulseLED,OUTPUT);
 analogWrite (PIN_LCD_BackgroundLight,0);




// init done

// you can change the contrast around to adapt the display
// for the best viewing!
display.setContrast(60);


display.clearDisplay();
display.drawBitmap(1, 1,  WeCnargeLoadingIcon,WeCnargeLoadingIconX, WeCnargeLoadingIconY, 1);
display.display();
delay (2000);
//   // invert the display
//   display.invertDisplay(true);
//   delay(1000);
//   display.invertDisplay(false);
//   delay(1000);

// draw a bitmap icon and 'animate' movement
// testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);
}
// Custom vars
int cOut = 0;
float BatChargedProcent;
VoltMeter BatteryVoltageObj(PINREAD_BatteryVolt,0.9	);

// Functions
void UpdateBatteryProcent() {

  //display little battery in middle with procent
    BatChargedProcent = ((BatteryVoltageObj.voltage - BatteryMinVoltCharged)  / (BatteryMaxVoltCharged - BatteryMinVoltCharged))*100.0;


}





void loop() { /// LOOOOOOP
BatteryVolt_voltage = BatteryVoltageObj.voltage;


// To React Voltage faster
if (BatteryVoltageObj.voltage < BatteryMaxVoltCharged && !BatteryIsCharged){
//always set low as default to not charge bat
//use this functions instead of analogWrite on 'initialized' pins
pwmWrite(PIN_VoltageRegulation, 0);//BatteryIsCharged
if (PWM_Success && BatChargedProcent >=1.0)  digitalWrite(PIN_PulseLED, LOW);
//digitalWrite(PIN_PulseLED,0);//13 led
delay (200);
BatteryVoltageObj.VoltageMeterUpdate ();
//use this functions instead of analogWrite on 'initialized' pins
  pwmWrite(PIN_VoltageRegulation, 100);//BatteryIsCharged
  if (PWM_Success && BatChargedProcent >=1.0)  digitalWrite(PIN_PulseLED, HIGH);


}else
 {
    pwmWrite(PIN_VoltageRegulation, 0);//Battery Is Fully  Charged
 }
//Serial.print (String ("Reg: ")+voltageRegulatorUnit);
//Serial.println (String (".......Volt: ")+BatteryVoltageObj.voltage);

display.clearDisplay();
display.setCursor (1,1);

if (BatteryVoltageObj.voltage < BatteryMaxVoltCharged && !BatteryIsCharged ){

// Get Voltage From Battery


  UpdateBatteryProcent();


   display.setTextSize (1);
  display.setCursor (31,1);
   //Complaining about no battery inserter
  if (BatChargedProcent >=1.0)
      display.print ( float(BatChargedProcent) +String("%" ));
  else
      {

      display.setCursor (25,1);
      //display.setTextSize (2);
      display.print ( "No batt");
      }

  //Serial.println (String (BatChargedProcent >50)+" <<");


if (BatChargedProcent <=1.0)
  display.drawBitmap(BatX, BatY,  BatNotInserted,BatWidth, BatHeight, 1);// Show Not Inserted Bat
else if		(BatChargedProcent < 30.0 )
  display.drawBitmap(BatX, BatY,  BatEmpty,BatWidth, BatHeight, 1);
else if (BatChargedProcent < 50.0 )
  display.drawBitmap(BatX,  BatY,  Bat33,BatWidth, BatHeight, 1);
else if (BatChargedProcent < 85.0 )
  display.drawBitmap(BatX, BatY,  Bat50,BatWidth, BatHeight, 1);
else if (BatChargedProcent < 99.0 )
  display.drawBitmap(BatX, BatY,  Bat85,BatWidth, BatHeight, 1);
else if (BatChargedProcent == 100.0 )
  display.drawBitmap(BatX, BatY,  Bat100,BatWidth, BatHeight, 1);


display.setCursor (1,33);
display.setTextSize (2);
if (BatChargedProcent >=1.0)  // diplay if bat is inserted
display.print (String ("v")+BatteryVoltageObj.voltage);


} else {

display.setTextSize (1);
display.setCursor (20,1);
display.println ("Charged");
display.drawBitmap(35, 20,  Bat100,BatWidth, BatHeight, 1);
BatteryIsCharged =true; // is charged
}


if ( BatteryIsCharged) {
    BatteryVoltageObj.VoltageMeterUpdate (); // Warning, Cannot Connect and read Hihght Voltage From Transformer Only This Case Then Battery Is Charged And Voltage Reading Updated Under False Condition
      UpdateBatteryProcent();
     if (BatChargedProcent < 80.0 && BatChargedProcent > 5) {
         BatteryIsCharged = false;
     }

// For Debug
  //  display.setTextSize(1);
  //  display.setCursor (10,9);
  //  display.println (cOut + String (",")+BatChargedProcent+String ("%"));
}


display.display();

if (cOut>9)
cOut=0;
cOut++;


delay (1000);

// Reset Flag of Fully charged if voltage are below 60 % and is greater then 1 % "Just For say that shoud be more then zero"

}
