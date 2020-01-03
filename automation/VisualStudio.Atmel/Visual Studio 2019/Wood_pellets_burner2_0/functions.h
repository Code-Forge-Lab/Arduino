#include "Arduino.h"
#include <LiquidCrystal_I2C.h> //0x3F mine or 0x27 need to find out with :LINK:http://playground.arduino.cc/Main/I2cScanner
#include <RotaryEncoder.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6,7,3, POSITIVE ); //was 0x3F  Eldas: 0x27 ,A4 and A5 Are Used

RotaryEncoder encoder(A0, 12);
bool rotaryEnable = false; // if is rotory encoder in the system then activate this.
String removed = "               ";



// define AC Motor control

#define INTERRUPT_ZeroCrossDetection 0 // detect when phase angle reach zero volts crossing in AC
#define RELAY_TRIAC_MotorSpeedControl 5
byte motorSpeed = 0;// store speed of AC fan
int_fast16_t motorPulseWidthTime; // save stored timer for triggering a triac
bool isMotorSpeedEnabled = false; // default : false
bool isMotorSpeedEGrounded = false; // if pin 3 or 4 not groundet with resistor , gives a false trigger and enable  a MotorSpeedControlFuncZeroCrossDetected function with that change a state for AC Triac Fan instead DC. Enable  for ignoring in my case
//byte motorCountFor10Times = 0; // set a timer for a main program fron a interrupts 
//bool motorIsCountedFor10Times = false; // chache if counted a 10 times in interrupt function
int_fast16_t motorMicroSecondsTriggerTimer = 5700; // usuly 7200, but max 8990(see in the scope) But requared fan to  work:5700

byte changedValueInterrupt = 0;
// Interrupts functions
void MotorSpeedControlFuncZeroCrossDetected() {

    // triac on condition

    //if (/*changedValueInterrupt != meniuIndex &&*/  frequencyControlLoadCounter >= frequencyControlLoad) {
        /*changedValueInterrupt = meniuIndex;*/
        // power control

    if (motorMicroSecondsTriggerTimer != motorPulseWidthTime) { // if max trigger not equal program generated value then do 


        delayMicroseconds(motorPulseWidthTime); //only delayMicroseconds can work in interrupt functions
        digitalWrite(RELAY_TRIAC_MotorSpeedControl, HIGH);

        // Serial.println(digitalRead(triacPulse));

        delayMicroseconds(50);  //delay   uSec on output pulse to turn on triac and 50uS no trigger all the time

        // reset a signal was set in triac condition

    }
    // reset frequency counter  
    //frequencyControlLoadCounter = 0;
//}

    digitalWrite(RELAY_TRIAC_MotorSpeedControl, LOW);
    //frequencyControlLoadCounter++; // counting runtime

    isMotorSpeedEnabled = true;
   

}

//Variables
/*  Verutes
const static  int8_t BUTTON_DOWN = 13;
const static  int8_t BUTTON_SET = 12; 
const static  int8_t BUTTON_UP = A0;
*/

// Mano
const static  int8_t BUTTON_DOWN = A0;
const static  int8_t BUTTON_SET = 12; 
const static  int8_t BUTTON_UP = 13;



//  0x3F
//const static  int8_t BUTTON_DOWN = 12;
//const static  int8_t BUTTON_SET = 13; 
//const static  int8_t BUTTON_UP = A0;

void print(String txt) {
    lcd.print(txt + "  ");
}

bool buttonRelease (int btn ) {
       bool pressed = false; 
         while (digitalRead(btn) > 0)
         {
              pressed = true;
              delay(50);
//              Serial.println("Button Pressed: "+ String(btn));
          }
             if (pressed) {
//                 Serial.println("Button Released");
                 pinMode (13,OUTPUT);
                  delay(30);
                  digitalWrite(13,HIGH);
                  delay(30);
                  digitalWrite(13,LOW);
                 pinMode (13,INPUT);
             }
        return pressed;
  }

//int *optional = 0
//
void myF () {}; // dummy function 
  void printMenuFunc (String text , controls* EEPROM  ,String n="", void (*functionPointer)() = myF  , bool testmode = false , String valueGreaterOrLessCondition = "Less" ,int valueGreaterOrLess=-1) {
      
      int8_t __set; 
      int8_t __up;
      int8_t __down;
      int  testmode_value; 
      String wrongStatment =  "";
//  lcd.noBlink();      
          if ( testmode ) // not save value in EEPROM that was changed here 
              testmode_value = EEPROM->getValue() ; 
          
  delay(map(EEPROM->getValue(), 0, 255, 10, 500)); // map(FANSPEEDRUN, 255, 0, 150, 7200)
  while ( true ) 
  {
            
          if (rotaryEnable) 
          {

              int side;// = encoder.getPositionSideTimeout(100); //timeout
             __up = 0; // reset
             __down = 0; // reset
             
            if (side == 1)
              __up = 1; // set condition
            else if (side == -1)
              __down = 1; // set condition
          } 
           else 
          {
            
          // if no rotory encoder but only push buttons
           __up = digitalRead(BUTTON_UP);
           __down = digitalRead(BUTTON_DOWN);
           //delay(100);
           delay(map(EEPROM->getValue(), 0, 255, 100, 25)); // map(FANSPEEDRUN, 255, 0, 150, 7200)          
          }

            __set = digitalRead(BUTTON_SET) ;
          
                  
          if (   __up  ) { // Boundry value not exeed
                 
                 if ( ( valueGreaterOrLess == -1 || valueGreaterOrLessCondition == "Greater"))
                      EEPROM->addValue();
                 else if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() < valueGreaterOrLess) {
                      EEPROM->addValue();

                      
                  } else  if ( EEPROM->getValue() > valueGreaterOrLess )   // if value to hight when valuegreaterorLess then equlize to minimum 
                           {   EEPROM->setValue (valueGreaterOrLess );   wrongStatment="";}
                             
                  
                     //or
                    if (valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() < valueGreaterOrLess && valueGreaterOrLess !=-1)
                       { EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}   

                     if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1)
                    { EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}           
             }
         
    
             if (  __down  ) { // Boundry value not exeed
                if ( ( valueGreaterOrLess == -1 || valueGreaterOrLessCondition == "Less"))
                      EEPROM->subValue();
                 else if (valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() > valueGreaterOrLess) {
                      EEPROM->subValue();
                
                  }
                 else  if ( EEPROM->getValue() > valueGreaterOrLess )   // if value to hight when valuegreaterorLess then equlize to minimum 
                          {   EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";   }; 
                       

                if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1)
                    { EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}
                    //or
                     
             }

                              
                             
               // lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print (text + removed);
                lcd.setCursor(0,1);
                lcd.blink();
                lcd.print (n+EEPROM->getValue()+ String (wrongStatment) + removed);
                
                
          functionPointer();
//     delay (100); // SLEEP COMMAND         
     if (__set >=1) {
         if (( valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1   ) ||
               valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() < valueGreaterOrLess && valueGreaterOrLess !=-1)
                {
                   wrongStatment = "?";
                }
                else  break; //end loop life then set button is pressed            
     }
  } /////// END
        
        if ( testmode ) { // not save value in EEPROM that was changed here 
             EEPROM->setValue(testmode_value  );  // return original state
             functionPointer();
        } 
   /////////////////////////////////////////  
            
        EEPROM->setValue ();
        lcd.noBlink();
        
        pinMode (13,OUTPUT);
        delay(30);
        digitalWrite(13,HIGH);
        delay(30);
        digitalWrite(13,LOW);
        pinMode (13,INPUT);
    }

    // map(FANSPEEDRUN, 255, 0, 150, 7200)

bool ArgueAgree (String text = "Ar tikrai taip?"  ) {
      
      int8_t __set; 
      int8_t __up;
      int8_t __down;
      bool  condition = false;

               
  delay(500);
  while ( true ) 
  {
       
        
              
          if (rotaryEnable) 
          {
              int side;// = encoder.getPositionSideTimeout(100); // timeout
             __up = 0; // reset
             __down = 0; // reset
             
            if (side == 1)
              __up = 1; // set condition
            else if (side == -1)
              __down = 1; // set condition
          } 
           else 
          {
            
          // if no rotory encoder but only push buttons
           __up = digitalRead(BUTTON_UP);
           __down = digitalRead(BUTTON_DOWN); 
          
              
               if (isMotorSpeedEnabled) // if TRIAC Fan Motor Exist, then adapt a timers
                  delay(map(motorPulseWidthTime, 0, motorMicroSecondsTriggerTimer, 100, 30)); // map(FANSPEEDRUN, 255, 0, 150, 7200)
               else
               {
                   delay(100);
               }
          }

          

            __set = digitalRead(BUTTON_SET) ;     
          
        
          if (   __up  && !condition   ) 
                    condition = true;
             
         
    
          if (  __down && condition ) 
                    condition = false;
             

          if ( __set )                            
                return condition; // return desided condition
                
                lcd.clear();
                lcd.print (text);
                lcd.setCursor(0,1);
              
              if (condition) // if true condition
                lcd.print ("taip");
              else // not true codition
                lcd.print ("ne");
    
          
   
    }
}

void sey (String lineA = "Issaugoti nauji", String lineB = "Nustatimai",int timeLastHold=2000) {
    lcd.clear();
   lcd.setCursor(0,0);
   lcd.print (lineA);
  
   if (isMotorSpeedEnabled) // if TRIAC Fan Motor Exist, then adapt a timers
       delay(map(motorPulseWidthTime, 0, motorMicroSecondsTriggerTimer, 500, 100)); // map(FANSPEEDRUN, 255, 0, 150, 7200)
   else
   {
       delay(500);
   }

   lcd.setCursor(0,1);
   lcd.print (lineB);
   delay(timeLastHold);
   lcd.clear();
  }



/*
  byte  findLiquidCrystal_I2C () {
 
 
 
  byte error, address;
  int nDevices;
 
//  Serial.println("Scanning...");/
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0)
    {
//      Serial.print("I2C device found at address 0x");/
      if (address<16)
//        Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
 
      nDevices++;
    }
    
  }

  
//  if (nDevices == 0)
//    return false; //    Serial.println("No I2C devices found\n");/
//  else
//    return true; //    Serial.println("done\n");/
 
  return  nDevices;
}
 */ 

/**
    void f () {
      
 
 
  byte error, address;
  int nDevices;
 
//  Serial.println("Scanning...");/
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
//      Serial.print("I2C device found at address 0x");/
      if (address<16)
//        Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
//      Serial.print("Unknown error at address 0x");/
      if (address<16)
//        Serial.print("0");/
//      Serial.println(address,/HEX);
    }    
  };**/
