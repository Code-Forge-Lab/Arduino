#include <stdlib.h>
#include <stddef.h>
#include "ManageReadWrite8bitEEPROM.h"
#include "menuLiquidCrystal.h"
#include "functions.h"


controls LCDLIGHT(0);//address
controls MAXTEMP(1); //address
controls FANSPEED(2); //address

controls PELLETPUSHERTIME(3); //address
controls PELLETPUSHERSPEED(4); //address



void funTEMP (){
  printMenuFunc("Max temperature",&MAXTEMP);

};

void funFAN (){
   printMenuFunc("Max. Fan RPM",&FANSPEED);
  };
  
void funPELLETPUSHER (){
  printMenuFunc("Gran. Greitis",&PELLETPUSHERSPEED);
  printMenuFunc("Gran.Laiko Truk.",&PELLETPUSHERTIME);
};

void BESTFUNCTION () {
      Serial.print ("WORKIN!!!!G");
      delay (5000);
  };

void funLCDLIGHT (){
  
  printMenuFunc("LCD Sviesa",&LCDLIGHT ,BESTFUNCTION );

  if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW); 
};



menuLiquidCrystal menu[4];
menuLiquidCrystalNavigate navmenu;


void initiate_functions () {

   // include menu objects
   menu[0].IncludeFunction(&funTEMP,"Vanens Temp." , MAXTEMP.getValue()); 
   menu[1].IncludeFunction(&funFAN,"Oro Put. Fenas",FANSPEED.getValue()); 
   menu[2].IncludeFunction(&funPELLETPUSHER,"Gran.stumiklis",PELLETPUSHERSPEED.getValue()); 
   menu[3].IncludeFunction(&funLCDLIGHT,"LCD BG Sviesa"); 
   //total menu available
   navmenu.setmenuLenght (sizeof(menu)/sizeof(menu[0])) ; // find out about size 
  }

void initiate_updatePins () {
      if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW); 
     
  }

const static  int pusher = 10;
const static  int wind = 11;

//b1(12) set, b2(13) down; b3(A0) up
//buttons
////////////////////
//Program Variables




void setup() {
  lcd.begin(16,2);

  
  
  // put your setup code here, to run once:
  pinMode(pusher,OUTPUT);
  pinMode(wind,OUTPUT);
  pinMode (BUTTON_SET,INPUT);
  pinMode (BUTTON_DOWN,INPUT);
  pinMode (BUTTON_UP,INPUT);
   Serial.begin(9600);
   Serial.println ("Load Complete: " + String  ("Structs array") + sizeof (menu) + String (",Menu [0] Size") + sizeof (menu[0]));
   delay(500);

initiate_functions ();  

}




int CLK_TIME=0;

bool menu1 = false;
int16_t menu1Timeout = 0;

void printmenu () {
            int8_t menuselected = navmenu.getMenuSelected();
            lcd.clear();
            lcd.print (String (menuselected+1)+String (")") + menu[menuselected].functionName);
            lcd.setCursor(0,1);
//            lcd.print (menu[navmenu.getMenuSelected()].functionValue);
  };

void loop() {
  
  // counters
  if (menu1Timeout > 0) --menu1Timeout; 
  if (CLK_TIME >= 10) CLK_TIME=0;CLK_TIME++; // program one second timer
  int8_t __set = digitalRead(BUTTON_SET) ;
  int8_t __up = digitalRead(BUTTON_UP);
  int8_t __down = digitalRead(BUTTON_DOWN);


     Serial.println("__set:"+ String(__set) + ",__up:"+ String(__up) + ",__down:" + String(__down)  );

//  if (CLK_TIME ==10) {} // slow components
//  {
         
//         lcd.print("MEMORY:" + String (FANSPEED.value));
//            lcd.print("MEMORY:" + String (FANSPEED.value));
            
            printmenu ();
           
    

             
         if (   __up) {//  Serial.print ( "getMenuSelected:" + String (navmenu.getMenuSelected ()) + ",getMenuLenght:" + String (navmenu.getMenuLenght()) );
     //        FANSPEED.addValue();
               navmenu.menuUp();
               printmenu ();
                // Wait button realess
               buttonRelease (BUTTON_UP );
         }

         

         if (  __down ) {
     //        FANSPEED.subValue();
               navmenu.menuDown();
               printmenu ();
          // Wait button realess
              buttonRelease (BUTTON_DOWN );
         }
      
       
               if (__set) {
                     lcd.blink();
                     menu[navmenu.getMenuSelected()].DrawFunction();                      
                     lcd.noBlink();
                 }  


 delay (100);
   
  

}
