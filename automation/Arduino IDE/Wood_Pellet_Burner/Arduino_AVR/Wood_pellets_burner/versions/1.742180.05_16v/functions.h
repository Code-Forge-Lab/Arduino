#include <LiquidCrystal_I2C.h> //0x3F mine or x27 need to find out with :LINK:http://playground.arduino.cc/Main/I2cScanner
LiquidCrystal_I2C lcd(0x27 , 2, 1, 0, 4, 5, 6,7,3, POSITIVE ); //was 0 x3F  Eldas: 0x27 ,A4 and A5 Are Used

//Variables
const static  int8_t BUTTON_SET = 12;
const static  int8_t BUTTON_DOWN = 13;
const static  int8_t BUTTON_UP = A0;



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
               
  delay(500);
  while ( true ) 
  {
       
        
           __set = digitalRead(BUTTON_SET) ;
           __up = digitalRead(BUTTON_UP);
           __down = digitalRead(BUTTON_DOWN);      
          

          
                  
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

                              
                             
                lcd.clear();
                lcd.print (text);
                lcd.setCursor(0,1);
                lcd.blink();
                lcd.print (n+EEPROM->getValue()+ String (wrongStatment) );
                
          functionPointer();
     delay (100);         
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



bool ArgueAgree (String text = "Ar tikrai taip?"  ) {
      
      int8_t __set; 
      int8_t __up;
      int8_t __down;
      bool  condition = false;

               
  delay(500);
  while ( true ) 
  {
       
        
           __set = digitalRead(BUTTON_SET) ;
           __up = digitalRead(BUTTON_UP);
           __down = digitalRead(BUTTON_DOWN);      
          
        
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
    
     delay (100);         
   
    }
}

void sey (String lineA = "Issaugoti nauji", String lineB = "Nustatimai",int timeLastHold=1400) {
    lcd.clear();
   lcd.setCursor(0,0);
   lcd.print (lineA);
   delay (500);
   lcd.setCursor(0,1);
   lcd.print (lineB);
   delay(timeLastHold);
  }
