#include <stdlib.h>
#include <stddef.h>
#include "ManageReadWrite8bitEEPROM.h"
#include "menuLiquidCrystal.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6,7,3, POSITIVE );


controls MAXTEMP(1); // (1)address
controls FANSPEED(2); //(1)address
controls PELLETPUSHER(3); //(1)address



void fun0 (){
  lcd.clear();  lcd.print("Page1:"); delay (3000);
};

void fun1 (){
  lcd.clear();  lcd.print("Page2:" );delay (3000);
  };
  
void fun2 (){
  lcd.clear();  lcd.print("Page3:" );delay (3000);
};

void fun3 (){
  lcd.clear();  lcd.print("Page4:" );delay (3000);
};



menuLiquidCrystal menu[3];
menuLiquidCrystalNavigate navmenu;


void initiate_functions () {
   
   menu[0].IncludeFunction(&fun0,"MAXTEMP" , MAXTEMP.getValue()); 
   menu[1].IncludeFunction(&fun1,"FANSPEED",FANSPEED.getValue()); 
   menu[2].IncludeFunction(&fun2,"PELLETPUSHER",PELLETPUSHER.getValue()); 
   navmenu.setmenuLenght (sizeof(menu)/sizeof(menu[0])) ; // find out about size 
  }


//0x3F need to find out with :LINK:http://playground.arduino.cc/Main/I2cScanner


const static  int pusher = 10;
const static  int wind = 11;

//b1(12) set, b2(13) down; b3(A0) up
//buttons
const static  int8_t BUTTON_SET = 12;
const static  int8_t BUTTON_DOWN = 13;
const static  int8_t BUTTON_UP = A0;
////////////////////
//Program Variables




void setup() {
  lcd.begin(16,2);

  lcd.blink();
  
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

bool buttonRelease (int btn ) {
       bool pressed = false; 
         while (digitalRead(btn) > 0)
         {
              pressed = true;
              delay(100);
              Serial.println("Button Pressed: "+ String(btn));
          }
             if (pressed) {
                 Serial.println("Button Released");
                 pinMode (13,OUTPUT);
                  delay(100);
                  digitalWrite(13,HIGH);
                  delay(100);
                  digitalWrite(13,LOW);
                 pinMode (13,INPUT);
             }
        return pressed;
  }


int CLK_TIME=0;

bool menu1 = false;
int16_t menu1Timeout = 0;

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
         lcd.clear();
//         lcd.print("MEMORY:" + String (FANSPEED.value));
//            lcd.print("MEMORY:" + String (FANSPEED.value));
            
            
            lcd.print ( menu[navmenu.getMenuSelected()].functionName);
            lcd.setCursor(0,1);
            lcd.print (menu[navmenu.getMenuSelected()].functionValue);

 
         if (__up) {
     //        FANSPEED.addValue();
           navmenu.menuUp();
          
         }
    
         if (__down) {
     //        FANSPEED.subValue();
               navmenu.menuDown();
          
         }
      
       
               if (__set) {
     //              FANSPEED.setValue();
                     menu[navmenu.getMenuSelected()].DrawFunction();                      
                 }


 delay (100);
   
  

}

