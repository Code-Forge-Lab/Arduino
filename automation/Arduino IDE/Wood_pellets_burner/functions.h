#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6,7,3, POSITIVE );

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
              Serial.println("Button Pressed: "+ String(btn));
          }
             if (pressed) {
                 Serial.println("Button Released");
                 pinMode (13,OUTPUT);
                  delay(30);
                  digitalWrite(13,HIGH);
                  delay(30);
                  digitalWrite(13,LOW);
                 pinMode (13,INPUT);
             }
        return pressed;
  }


  void printMenuFunc (String text , controls* EEPROM  ) {
      
      int8_t __set; 
      int8_t __up;
      int8_t __down; 
      int8_t prnSpeed =4;

  lcd.noBlink();      
  delay(500);
  while ( true ) 
  {
     
        
           __set = digitalRead(BUTTON_SET) ;
           __up = digitalRead(BUTTON_UP);
           __down = digitalRead(BUTTON_DOWN);      
    
        
          if (   __up) {
                 EEPROM->addValue();         
             }
         
    
             if (  __down ) {
                EEPROM->subValue();
             }
            
            if (prnSpeed <= 0 | true) { // No flickering lcd
                lcd.clear();
                lcd.print (text);
                lcd.setCursor(0,1);
                lcd.blink();
                lcd.print (EEPROM->getValue());
                prnSpeed=4;
            }  
                
       
     delay (100);      
     prnSpeed--; 
     if (__set >=1) break; //end loop life then set button is pressed     
            
  }
        EEPROM->setValue ();
        lcd.noBlink();
    }
