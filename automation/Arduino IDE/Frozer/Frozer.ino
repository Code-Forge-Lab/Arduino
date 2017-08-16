
#include "ThermoSencor.h"
#include <Arduino.h>
const static int  Red = 3; 
const static int  Yellow = 6;
const static int  Green =5;
const static int  thermo = A4;
const static int  pot = A0;
const static int  relay = 4;
  

void ledOn ( int8_t addr , bool enable = true) {
     if (enable)
          analogWrite (addr , 3 );
     else     
          analogWrite (addr , 0 );
  }

  
void setup() {
  Serial.begin (9600);
  pinMode(Red,OUTPUT);
  pinMode(Yellow,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(thermo,INPUT);
  pinMode(relay,OUTPUT);
  
  
  };



 ThermoSencor sensor ( thermo ) ;

int potvSave = analogRead(pot);
int potPush=0;
bool potToggle = false;

int freezeLevel = 999;
bool freezeToggle = true;

int relayTimeOut = 0;
int relayTimeOutSleep =0;

  void loop () {



//Freezing level menu indicator
    int potV = analogRead(pot);
  
//   pot1 > captureValuePot1 +5 || pot1 < captureValuePot1 -5
      if ( (potV >  potvSave +20 || potV <  potvSave -10)  || potPush > 0) 
      {
//          potvSave = analogRead(pot);
          delay (50);
//         Give loop time 
          if ((potV >  potvSave +20 || potV <  potvSave -10) ) potPush = 50;
          
          potToggle=!potToggle;
          
          if (potV > 340) {
              ledOn (Green,potToggle);
               freezeLevel=0;
          }else { ledOn (Green,false); 
              freezeLevel = 999; // off freezer mode
          }

          if (potV > 682) {
             ledOn (Yellow,potToggle);
             freezeLevel=-1;
          }else  ledOn (Yellow,false);

          if (potV > 980) {
             ledOn (Red,potToggle);
             freezeLevel=-2;
          }else  ledOn (Red,false);

          
          
              
             delay (50);
              Serial.println ("Potentiometer:" + String ( potV)+ ",povtSave"+String(potvSave) + ", loop:" + String(potPush) + ",potPush % num" + String (potPush % 5) );
             delay (50);
             
              if (potPush % 5 == 0)  // update in each 5 steps
                 potvSave = analogRead(pot);

                 
//              Snake Blink to end this loop
              if ( potPush == 1) 
              {
                  ledOn (Red,false);(Yellow,false);ledOn(Green,false);     
                  ledOn (Red);delay (500);ledOn(Yellow);delay(500);ledOn(Green);delay(500);
                  ledOn (Red,false);delay (500);ledOn(Yellow,false);delay(500);ledOn(Green,false);delay(500);     
                }
             potPush--;
          return ;
        }
    
   
    sensor.PreciseSencorUpdate ();
        
    int r = analogRead (A4);

     Serial.println(String("freeze reaching:")+freezeLevel+",now temperature:" + String( sensor.Temperature) + " bin:" + String(r) + ",relayTimeOut:" +String (relayTimeOut) +",relayTimeOutSleep:"+String(relayTimeOutSleep) );

// Freezing stage inditation
 freezeToggle = !freezeToggle;
 
     if (sensor.Temperature > freezeLevel && sensor.Temperature >= 0 && freezeLevel <=0 ) ledOn (Green,freezeToggle); else if (freezeLevel <= 0)ledOn(Green); else ledOn(Green,false); 
     if (sensor.Temperature > freezeLevel && sensor.Temperature >= -1 && freezeLevel <=-1 ) ledOn (Yellow,freezeToggle); else if (freezeLevel <= -1)ledOn(Yellow); else ledOn(Yellow,false); 
     if (sensor.Temperature > freezeLevel && sensor.Temperature >= -2 && freezeLevel <=-2) ledOn (Red,freezeToggle); else if (freezeLevel <= -2)ledOn(Red); else ledOn(Red,false); 
 delay (4000);

 
//Control freezer work
     if (sensor.Temperature > freezeLevel) // if temperature is to high
           relayTimeOut = 10;
          
          

      if (relayTimeOut== 0) // if relayTimeOut reached then freezing is ower
       {

        relayTimeOutSleep =75  ; // 225= 15, 75 = 5 minutes sleep activator;
       }    
          
     
          
     if (  relayTimeOutSleep != 0) {

          relayTimeOutSleep--;
      }
     
     if (relayTimeOut !=0 && relayTimeOutSleep == 0 )    { //
          digitalWrite(relay,HIGH);
          relayTimeOut--; // count down after sleeped and alway zero when are requared amount of cold
     }   
              else
          digitalWrite(relay,LOW);
        
    
     
}
