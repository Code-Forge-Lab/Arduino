#include <stdlib.h>
#include <stddef.h>
#include "ManageReadWrite8bitEEPROM.h"
#include "menuLiquidCrystal.h"
#include "functions.h"

int8_t FANPIN = 10; // ~
int8_t PELLETPUSHERPIN = 11; // ~

controls DefaultTrigger (0); // default flagg
controls LCDLIGHT(1,1);//address
controls MAXTEMP(2,30); //address
controls FANSPEED(3,100); //address

// Pellets            
long int PELLETON_TIMEOUT=0;
long int PELLETOFF_TIMEOUT=0;
//                       address


controls PELLETPUSHERMILLISECONDSON(5,1); //address //  push time until sleep time + seconds
controls PELLETPUSHERMINUTESOFF(6,0); //address  // sleep time until pushing
controls PELLETPUSHERSECONDSOFF(7,100); //address  // sleep time until pushing
controls PELLETPUSHERENABLE(8,1); // turn on or off a motor
controls PELLETPUSHERMINSPEED(9,20); 
controls PELLETPUSHERMAXSPEED(10,60); 
controls PELLETPUSHERMODE(11,1); // Select Mode 1)Only Timer, 2)By Temp. Regulating Min or Max Power, 3)Power Regulating By Temp. Bitween Min or Max in Procentage Range
controls SYSTEMONOFF(12,1); 


void init_memory_defaults (bool conditiondefault = false) {
//      if (DefaultTrigger.readValue () > 0  || conditiondefault)  // trigger one time default
//      { 
        LCDLIGHT.setDataDefault();
        MAXTEMP.setDataDefault();
        FANSPEED.setDataDefault();
        PELLETPUSHERMILLISECONDSON.setDataDefault ();
        PELLETPUSHERMINUTESOFF.setDataDefault ();
        PELLETPUSHERSECONDSOFF.setDataDefault ();
        PELLETPUSHERENABLE.setDataDefault ();
        PELLETPUSHERMINSPEED.setDataDefault ();
        PELLETPUSHERMAXSPEED.setDataDefault ();
        SYSTEMONOFF.setDataDefault ();
//      }  
//    DefaultTrigger.setValue(1);
  }

bool ScreenStatusDisplay = false;



// START menu functions
void funTEMP (){
  printMenuFunc("Max temperature",&MAXTEMP,"C* ");

};
//////////////////

void __FANSPEED () {analogWrite (FANPIN, FANSPEED.getValue());};
void funFAN (){
   analogWrite (PELLETPUSHERPIN, 0); //Disable Pellet Pusher 
    analogWrite (PELLETPUSHERPIN, 0); // Disable Pellet Pusher
   printMenuFunc("Max. Fan RPM",&FANSPEED,"RPM:",__FANSPEED);
  };

  
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void __PELLETPUSH () {analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue());}; 
 void funPelletModeOnlyTimer ()
  {
  analogWrite (PELLETPUSHERPIN,0); // disable pellet pusher 
  printMenuFunc("Gran. Greitis",&PELLETPUSHERMINSPEED,"RPM:");
  printMenuFunc("Gran.Veik.mlsc",&PELLETPUSHERMILLISECONDSON,"millisec:"); 
  printMenuFunc("Gran.Neveik.Min",&PELLETPUSHERMINUTESOFF,"min:");
  printMenuFunc("Gran.Neveik.Sec",&PELLETPUSHERSECONDSOFF,"secundes:");
  printMenuFunc("Gran.Activuoti",&PELLETPUSHERENABLE,"OFF/ON:");
  PELLETPUSHERMODE.writeValue(1);
  };

 void funPelletModeTempMinOrMax ()
 {
  PELLETPUSHERMODE.writeValue(2);
 };
 
 void funPelletModeTempBetweenMinMaxProcentage ()
 {
  PELLETPUSHERMODE.writeValue(3);
  };
  
//////////////////////////////////////////////////
//////////////////////////////////////////////////

/////////////////
void __funLCDLIGHT () {    // Sub Function 
      if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW); 
  };

void funLCDLIGHT (){
  
  printMenuFunc("LCD Sviesa",&LCDLIGHT ,"on/off:",__funLCDLIGHT );
};

void funTestingComponents () { // temporery loaded value that not changed can be tested with commands
         printMenuFunc("Gran. Greitis",&PELLETPUSHERMINSPEED,"TEST-RPM:",__PELLETPUSH , true);
//         printMenuFunc("Max. Fan RPM",&FANSPEED,"TEST-RPM:",__FANSPEED,true);
  }

      
     void __funSystemOnOff (){ // test mode ??/!
            if (SYSTEMONOFF.getValue() > 0 ){ // if system flag set to On
        analogWrite (FANPIN, FANSPEED.getValue()); 
        analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue());
       } else
       {
           analogWrite (FANPIN, 0); 
           analogWrite (PELLETPUSHERPIN, 0);
       }
      }
      void funSystemOnOff () {
         printMenuFunc("Gran.Activuoti",&SYSTEMONOFF,"OFF=0/ON:",__funSystemOnOff   );
      };

     void funSettoDefault () {
                analogWrite (PELLETPUSHERPIN,0); // disable pellet pusher 

               if  ( ArgueAgree ()) 
                {
                    lcd.clear();
                    lcd.setCursor(0,1);
                    lcd.print ("Default was");
                      lcd.setCursor(0,0);
                    for (int x = 0; x <16; x++) //animation arrow
                     {
                         
                         delay(100);                
                          lcd.print (">");
    //                      
                      }
                    lcd.setCursor(0,1);
                    lcd.print ("Default was set");
                    delay(200);
                    init_memory_defaults (true);
                    delay(2000);
                } else 
                {
                     lcd.clear();                 
                     lcd.setCursor(0,1);
                     delay(300);
                     lcd.print ("Ok");
                     delay(800);
                     lcd.setCursor(0,0);
                     lcd.print ("Default Not Set");
                     delay (1000);
                }
                
      }; 

void funExit () {
      ScreenStatusDisplay = false;
  };

// END menu functions


menuLiquidCrystal menu[10]; // alway give exact size of menu
menuLiquidCrystalNavigate navmenu;

// load into menu external functions
void initiate_menu_functions () {

   // include menu objects
   menu[0].IncludeFunction(&funTEMP,"Temperatura"); 
   menu[1].IncludeFunction(&funFAN,"Oro Put. Fenas"); 
   menu[2].IncludeFunction(&funPelletModeOnlyTimer,"1-Gran. Stumiklis","Pagal Laika");
   menu[3].IncludeFunction(&funPelletModeTempMinOrMax,"2-Gran. Stumiklis","Temp.Min - Max");
   menu[4].IncludeFunction(&funPelletModeTempBetweenMinMaxProcentage,"3-Gran. Stumiklis","Temp.Min % Max") ;
   menu[5].IncludeFunction(&funLCDLIGHT,"Sviesa","On or Off");
   menu[6].IncludeFunction(&funTestingComponents,"Testavimas");
   menu[7].IncludeFunction(&funSystemOnOff,"System","On or Off");
   menu[8].IncludeFunction(&funSettoDefault,"Set to Default","Gamik.Parametrai");
  
   menu[9].IncludeFunction(&funExit,"Iseiti"," :)"); 
   
   //total menu available
   navmenu.setmenuLenght (sizeof(menu)/sizeof(menu[0])) ; // find out about size 
  }

// when program loaded newly .
void initiate_updatePins ( bool print = true) {
      if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW); 
       if (SYSTEMONOFF.getValue() > 0 ){ 
        analogWrite (FANPIN, FANSPEED.getValue()); 
        analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue());
       }

      if (print) 
      {
      Serial.println ("FANSPEED:" + String (FANSPEED.getValue()));
      Serial.println ("PELLETPUSHERMINSPEED:" + String (PELLETPUSHERMINSPEED.getValue()));
      Serial.println ("MAXTEMP:" + String (MAXTEMP.getValue()));
      }
  }

// Update Pins 

void initControlPins () {
  analogWrite (FANPIN, FANSPEED.getValue()); 
  //////////analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); Negali buti naudojamas be logikos isikisimo
  __funLCDLIGHT (); // Update Background Light
  
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
  // Motors
  pinMode (FANPIN,OUTPUT);
  pinMode (PELLETPUSHERPIN,OUTPUT);
  
   Serial.begin(9600);
   Serial.println ("Load Complete: " + String  ("Structs array") + sizeof (menu) + String (",Menu [0] Size") + sizeof (menu[0]));
   delay(500);

initiate_menu_functions ();  
initiate_updatePins ();

 if (EEPROM.read(SYSTEMONOFF.getAddress ()) >=255 ) { // if fist time are loded to chip when set sum defaults 
    init_memory_defaults ();
    SYSTEMONOFF.setValue (0);
 } 
}





void printmenu () {
            int8_t menuselected = navmenu.getMenuSelected();
            lcd.clear();
            lcd.print (String (menuselected+1)+String (")") + menu[menuselected].functionName);
            lcd.setCursor(0,1);
            if (  menu[navmenu.getMenuSelected()].isEmptyFunctionValue() ) // ignore unknown value , no dinamic update support :<
                 lcd.print (menu[navmenu.getMenuSelected()].functionValue);
  };


int8_t printstatuscounter = 0;
int8_t printstatustimer = -1;
void printstatus (bool print =false) {
           
            lcd.clear();

            switch(printstatuscounter) 
            {
               case 1 :
                      lcd.print("Gran.stumiklis" );
                      lcd.setCursor(0,1);                 

                          if (PELLETPUSHERMODE.getValue () == 1) // by Timer
                          {
                            if (PELLETON_TIMEOUT != -1 && SYSTEMONOFF.getValue() > 0)
                                   lcd.print("Clock>on:"+ String (PELLETON_TIMEOUT)) );
                              else   
                                   lcd.print("Clock>off:"+ String (PELLETOFF_TIMEOUT) );; 
                          }
                          else if (PELLETPUSHERMODE.getValue () == 2) //Bitween Hight or Low , Controled by Temperature
                          {
                                   lcd.print("Temp.L-H:" + String(PELLETPUSHERMODE.getValue ()) );; 
                          }
                          else if (PELLETPUSHERMODE.getValue () == 3 )// Bitween  Hight or Low using Sum of Procentage Distage , Controled by Temperature
                          {
                                  lcd.print("T.L%H:" + String(PELLETPUSHERMODE.getValue ()) );; 
                          } else
                          {
                              lcd.print("Isjungta.");  
                          }
                       
//                      if ( SYSTEMONOFF.getValue() > 0) 
//                        {
//                              if (PELLETON_TIMEOUT != -1 && SYSTEMONOFF.getValue() > 0)
//                                   lcd.print("on:"+ String (PELLETON_TIMEOUT)+ " mode-" + String(PELLETPUSHERMODE.getValue ()) );
//                              else   
//                                   lcd.print("off:"+ String (PELLETOFF_TIMEOUT)+ " mode-" + String(PELLETPUSHERMODE.getValue ()) );; 
//                                      //////////////////////////////////// Print isjungta/ijungta in bottom
//                        }else       
//                                   lcd.print("Isjungta.");    
//                       
                          
                                                      
                  break; 
               case 2 :
                      lcd.print("Oro Put. Fenas");
                      lcd.setCursor(0,1);
                     if ( SYSTEMONOFF.getValue()> 0)  { 
                      lcd.print("RPM:"+ String (FANSPEED.getValue()*10));
                     }else
                             lcd.print("Isjungtas.");       
                  break; 
               case 3 :
                      lcd.print("Vanens Temp.");
                      lcd.setCursor(0,1);
                      lcd.print("C*:"+ String (MAXTEMP.getValue()));   
                 break;     
               case 4 :
                   
                      lcd.print("Systema yra");
                      lcd.setCursor(0,1);
                      if ( SYSTEMONOFF.getValue() > 0) {
                           lcd.print(":Ijungta");      
                      }else
                       {
                          lcd.print(":Isjungtas."); 
                       }
                  break;    
               }


                           
                
            if (printstatuscounter > 4 ) //how much status blocks in switch case 
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

int8_t OneSec = 10; // times in loop until reach one secunde

int exitmenutime = 30*10;//ms
int exitmenutimer = exitmenutime;

void loop() {
initControlPins (); // update output pins

if ( SYSTEMONOFF.getValue() > 0 ) { // reset alway if flag is on
    if (PELLETON_TIMEOUT > -1)PELLETON_TIMEOUT--;
    if (PELLETOFF_TIMEOUT > -1)PELLETOFF_TIMEOUT --;
}else 
{
    PELLETON_TIMEOUT = 0;
    PELLETOFF_TIMEOUT = 0;
  }
  //
  if (printstatustimer > -1) printstatustimer--;

  
    


  
  int8_t __set = digitalRead(BUTTON_SET) ;
  int8_t __up = digitalRead(BUTTON_UP);
  int8_t __down = digitalRead(BUTTON_DOWN);


//     Serial.println("__set:"+ String(__set) + ",__up:"+ String(__up) + ",__down:" + String(__down)  );

// initiate_updatePins (false);

     //PELLETPUSHER
         
//           PELLETPUSHERTIMEON.getVaule();
//           PELLETPUSHERMINUTESOFF.getVaule();
//           PELLETPUSHERENABLE.getVaule(); 
//           PELLETPUSHERMINSPEED.getVaule();
      
          if (PELLETPUSHERENABLE.getValue()  > 0 ) { // Checnk if pellet burner flag is on
                
                    
                    
                    if (PELLETON_TIMEOUT == -1 && PELLETOFF_TIMEOUT == -1) // before ON_TIMEOUT become -1 , zero give window to step up a turn of mode enable
                       PELLETOFF_TIMEOUT = PELLETPUSHERMINUTESOFF.getValue() * OneSec * 60 + PELLETPUSHERSECONDSOFF.getValue() * OneSec; // min = 60 seconds  + cunstom seconds    

                    if (PELLETON_TIMEOUT == -1 && PELLETOFF_TIMEOUT == 0) // give beggining and turn on pellet pusher
                       PELLETON_TIMEOUT =  PELLETPUSHERMILLISECONDSON.getValue(); // min = 60 seconds + custom seconds

                   
                    if (PELLETON_TIMEOUT > -1 && SYSTEMONOFF.getValue() > 0) // execute rutine   
                           //ON
                        analogWrite(PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); //Give speed/power to motor
                     else // OFF
                        analogWrite(PELLETPUSHERPIN,0); //Give speed/power to motor    
                        
          }


           
     // END //PELLETPUSHER


     // Automatiskai iseinti pagal laika is option menu
     
      // to Those What to print 
           if (ScreenStatusDisplay){
                printmenu ();
                    
                      // exit timer 
                      if (exitmenutimer > 0)
                          exitmenutimer--;
                      else
                          funExit (); // automaticly exit after sum time from manual seletion menu
                      
                    }    
                 else {
                      printstatus ();
                  }
           
      

             
         if (   __up ) {//  Serial.print ( "getMenuSelected:" + String (navmenu.getMenuSelected ()) + ",getMenuLenght:" + String (navmenu.getMenuLenght()) );
     //        FANSPEED.addValue();
               navmenu.menuUp();
               printmenu ();
                // Wait button realess
               buttonRelease (BUTTON_UP );
               exitmenutimer = exitmenutime; // update each time auto menu exit timer after pressed a button
               
               ScreenStatusDisplay=true;
         }

         

         if (  __down ) {
     //        FANSPEED.subValue();
               navmenu.menuDown();
               printmenu ();
          // Wait button realess
              buttonRelease (BUTTON_DOWN );
              exitmenutimer = exitmenutime; // update each time auto menu exit timer after pressed a button
              
              ScreenStatusDisplay=true;
         }
      
       
               if (__set && ScreenStatusDisplay) {

                

                        
                         lcd.blink();
                         menu[navmenu.getMenuSelected()].DrawFunction();                      
                         lcd.noBlink();


//                       if (  menu[navmenu.getMenuSelected()].isEmptyFunctionValue() ) // ignore unknown value
//                          menu[navmenu.getMenuSelected()].functionValue  = 

                    
                       if ( ! ScreenStatusDisplay ) {
                        navmenu.menuReset(); // When user exit from menu , give ScreenStatusDisplay = false
                       }
//                      Serial.println ("After ScreenStatusDisplay:"+String (ScreenStatusDisplay)  );
                   
                        // fast swith status output
                                           
                 }  
               if (__set && !ScreenStatusDisplay) 
                {
                      printstatus (true);
                }

 delay (100);
   
  

}


