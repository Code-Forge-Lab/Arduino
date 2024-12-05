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

controls LCDLIGHT(0,1);//address
// Celcius
controls TEMPMIN (1,19); // default flagg
controls TEMPMAX(2,23); //address
// Fan
controls FANMINSPEED(3,90); //address
controls FANMAXSPEED(4,150); //address

controls FANSECONDSHOLD(5,70); //Hold time after fast shift , that make to burn last cycle dropet pellets
               

// Pellets  

long int PELLETON_TIMEOUT=0;
long int PELLETOFF_TIMEOUT=0;

controls PELLETPUSHERMODE(7,1); // Select Mode 1)Only Timer, 2)By Temp. Regulating Min or Max Power, 3)Power Regulating By Temp. Bitween Min or Max in Procentage Range

controls PELLETPUSHERMILLISECONDSON(8,10); //address //  push time until sleep time + seconds
controls PELLETPUSHERMINSPEED(9,20);
controls PELLETPUSHERSECONDSOFF(10,100); //address  // sleep time until pushing

//   




void init_memory_defaults (bool conditiondefault = false) {

       LCDLIGHT.setDataDefault();
      
       TEMPMIN.setDataDefault(); 
       TEMPMAX.setDataDefault();
      
       FANMINSPEED.setDataDefault();
       FANMAXSPEED.setDataDefault();
      
       FANSECONDSHOLD.setDataDefault();
       
                     
       PELLETPUSHERMODE.setDataDefault();
       PELLETPUSHERMILLISECONDSON.setDataDefault();
       PELLETPUSHERMINSPEED.setDataDefault();
       PELLETPUSHERSECONDSOFF.setDataDefault();

  }

bool ScreenStatusDisplay = false;



// START menu functions
void funTEMP (){
  printMenuFunc("Max temperature",&TEMPMAX,"C* ");

};
//////////////////

void __FANMINSPEED () {analogWrite (FANPIN, FANMINSPEED.getValue());};
void funFAN (){
   analogWrite (PELLETPUSHERPIN, 0); //Disable Pellet Pusher 
    analogWrite (PELLETPUSHERPIN, 0); // Disable Pellet Pusher
   printMenuFunc("Max. Fan RPM",&FANMINSPEED,"RPM:",__FANMINSPEED);
  };

  
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void __PELLETPUSH () {analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue());}; 
 void funPelletModeOnlyTimer ()
  {
  analogWrite (PELLETPUSHERPIN,0); // disable pellet pusher 
  printMenuFunc("Gran. Greitis",&PELLETPUSHERMINSPEED,"RPM:");
  printMenuFunc("Gran.Veik.mlsc",&PELLETPUSHERMILLISECONDSON,"millisec:"); 
  printMenuFunc("Gran.Neveik.Sec",&PELLETPUSHERSECONDSOFF,"secundes:"); // nenaudoti nes nereikia

  PELLETPUSHERMODE.writeValue(1); // Set to state
  sey ();
  
  };

 void funPelletModeTempMinOrMax ()
 {
  analogWrite (PELLETPUSHERPIN,0); // disable pellet pusher 
  printMenuFunc("Gran. Greitis",&PELLETPUSHERMINSPEED,"RPM:"); 
  
//  printMenuFunc("Gran.Greitis Max",&PELLETPUSHERSECONDSOFF,"secundes:");
  printMenuFunc("Fan.Hold.Time",&FANSECONDSHOLD,"sec:"); // Delay of Keep  Turn On Fan sum While
  sey ("Gran. Min or Max");
  PELLETPUSHERMODE.writeValue(2);
  sey ();
 };
 
 void funPelletModeTempBetweenMinMaxProcentage ()

 {
  analogWrite (PELLETPUSHERPIN,0); // disable pellet pusher 
  printMenuFunc("Gran. Greitis",&PELLETPUSHERMINSPEED,"RPM:");
  
  printMenuFunc("Gran.Greitis Max",&PELLETPUSHERSECONDSOFF,"secundes:");
  printMenuFunc("Fan.Hold.Time",&FANSECONDSHOLD,"sec:"); // Delay of Keep  Turn On Fan sum While
  PELLETPUSHERMODE.writeValue(3);
   sey ("Gran. Procentage");
  };
  
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
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
//         printMenuFunc("Max. Fan RPM",&FANMINSPEED,"TEST-RPM:",__FANMINSPEED,true);
  }


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


menuLiquidCrystal menu[9]; // alway give exact size of menu
menuLiquidCrystalNavigate navmenu;

// load into menu external functions
void initiate_menu_functions () {

   // include menu objects
   menu[0].IncludeFunction(&funTEMP,"Temperatura"); 
   menu[1].IncludeFunction(&funFAN,"Oro Put. Fenas"); 
   menu[2].IncludeFunction(&funPelletModeOnlyTimer,"1-Veikimo Budas","Pagal Laika");
   menu[3].IncludeFunction(&funPelletModeTempMinOrMax,"2-Veikimo Budas","Temp.Min - Max");
   menu[4].IncludeFunction(&funPelletModeTempBetweenMinMaxProcentage,"3-Veikimo Budas","Temp.Min % Max") ;
   menu[5].IncludeFunction(&funLCDLIGHT,"Sviesa","On or Off");
   menu[6].IncludeFunction(&funTestingComponents,"Testavimas");
   
   menu[7].IncludeFunction(&funSettoDefault,"Set to Default","Gamik.Parametrai");
  
   menu[8].IncludeFunction(&funExit,"Iseiti"); 
   
   //total menu available
   navmenu.setmenuLenght (sizeof(menu)/sizeof(menu[0])) ; // find out about size 
  }

// when program loaded newly .
void initiate_updatePins ( bool print = true) {
      if (LCDLIGHT.getValue() > 0)lcd.setBacklight(HIGH); else lcd.setBacklight(LOW); 
       
        analogWrite (FANPIN, FANMINSPEED.getValue()); 
        analogWrite (PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue());
      

 //     if (print) 
//      {
//      Serial.println ("FANMINSPEED:" + String (FANMINSPEED.getValue()));
//      Serial.println ("PELLETPUSHERMINSPEED:" + String (PELLETPUSHERMINSPEED.getValue()));
//      Serial.println ("TEMPMAX:" + String (TEMPMAX.getValue()));
  //    }
  }

// Update Pins 

void initControlPins () {
  analogWrite (FANPIN, FANMINSPEED.getValue()); 
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
  // Temp. Sensor
   pinMode (TEMPSENSORPIN,INPUT);
  
//   Serial.begin(9600);
//   Serial.println ("Load Complete: " + String  ("Structs array") + sizeof (menu) + String (",Menu [0] Size") + sizeof (menu[0]));
   delay(500);

initiate_menu_functions ();  
initiate_updatePins ();

 if (EEPROM.read(1000) >=255 ) { // if first time are loded to chip when set sum defaults 
    init_memory_defaults ();
     EEPROM.write(1000,0);
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
int c=0;
float val;
void printstatus (bool print =false) {

   

//       c--;
//    if (c <=0) {
//////        val = analogRead(TEMPSENSORPIN)/25.7;
//        val=Temperature.RawValueUpdate () /25.7;
//        delay(20);
//        c=10;
//      }
            lcd.clear();

            switch(printstatuscounter) 
            {
               case 1 :
                      lcd.print("Gran.stumiklis" );
                      lcd.setCursor(0,1);                 

                          if (PELLETPUSHERMODE.getValue () == 1) // by Timer
                          {
                                if (PELLETON_TIMEOUT != -1 )
                                     lcd.print("Clock>on:"+ String (PELLETON_TIMEOUT) );
                                else   
                                     lcd.print("Clock>off:"+ String (PELLETOFF_TIMEOUT) );
                          }
                          else if (PELLETPUSHERMODE.getValue () == 2) //Bitween Hight or Low , Controled by Temperature
                          {
                                   lcd.print("Temp.L-H:"  );; 
                          }
                          else if (PELLETPUSHERMODE.getValue () == 3 )// Bitween  Hight or Low using Sum of Procentage Distage , Controled by Temperature
                          {
                                  lcd.print("T.L%H:"  );; 
                          } else
                          {
                              lcd.print("Isjungta.");  
                          }
                       

                          
                                                      
                  break; 
               case 2 :
                      lcd.print("Oro Put. Fenas");
                      lcd.setCursor(0,1);
                      lcd.print("RPM:"+ String (FANMINSPEED.getValue()*10));
                           
                  break; 
               case 3 :
                      lcd.print("Vanens Temp.");
                      lcd.setCursor(0,1);
                      lcd.print("C* max:"+ String (TEMPMAX.getValue()) + ",C:"+ String(Temperature.Temperature));    // Temperature.Temperature
                 break;     
               case 4 :
                   
                      lcd.print("TestBlank ");
                      lcd.setCursor(0,1);
                     
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

delay (100);
  
initControlPins (); // update output pins


//delay (90);
   if (PELLETON_TIMEOUT > -1)PELLETON_TIMEOUT--;
    if (PELLETOFF_TIMEOUT > -1)PELLETOFF_TIMEOUT --;

 
  if (printstatustimer > -1) printstatustimer--;

  
    


  
  int8_t __set = digitalRead(BUTTON_SET) ;
  int8_t __up = digitalRead(BUTTON_UP);
  int8_t __down = digitalRead(BUTTON_DOWN);


//     Serial.println("__set:"+ String(__set) + ",__up:"+ String(__up) + ",__down:" + String(__down)  );

// initiate_updatePins (false);

     //PELLETPUSHER
         

         switch(PELLETPUSHERMODE.getValue()) 
         {
            case 1 : 
                  if (PELLETON_TIMEOUT == -1 && PELLETOFF_TIMEOUT == -1) // before ON_TIMEOUT become -1 , zero give window to step up a turn of mode enable
                             PELLETOFF_TIMEOUT =  PELLETPUSHERSECONDSOFF.getValue() * OneSec; // min = 60 seconds  + cunstom seconds    
      
                  if (PELLETON_TIMEOUT == -1 && PELLETOFF_TIMEOUT == 0) // give beggining and turn on pellet pusher
                             PELLETON_TIMEOUT =  PELLETPUSHERMILLISECONDSON.getValue(); // min = 60 seconds + custom seconds
      
                         
                          
            break;
            case 2 :
            break;
            case 3 :
            break;
            default:
            break;
         }


          if (PELLETON_TIMEOUT > -1 ) // execute rutine   
                           //ON
                        analogWrite(PELLETPUSHERPIN, PELLETPUSHERMINSPEED.getValue()); //Give speed/power to motor
                     else // OFF
                        analogWrite(PELLETPUSHERPIN,0); //Give speed/power to motor  


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
     //        FANMINSPEED.addValue();
               navmenu.menuUp();
               printmenu ();
                // Wait button realess
               buttonRelease (BUTTON_UP );
               exitmenutimer = exitmenutime; // update each time auto menu exit timer after pressed a button
               
               ScreenStatusDisplay=true;
         }

         

         if (  __down ) {
     //        FANMINSPEED.subValue();
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

 
   
  

}


