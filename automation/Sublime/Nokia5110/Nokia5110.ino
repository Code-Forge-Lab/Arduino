#include "nokia5110.h"
#include "myFunk.h"
#include "G:\root\MyFolder\Script\Arduino\Library\SubLibrary\myTest.h"
#include "G:\root\MyFolder\Script\Arduino\Library\SubLibrary\procent.h"
int PIN_LCD_LIGHT = 9;
// void blink (int time_) {
// digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//   delay(time_);               // wait for a second
//   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//   delay(time_);  

// }


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  analogWrite(PIN_LCD_LIGHT, 135);   
  SETUPLCD();
  //AllRowsSelect ();
  //LcdWriteString ("HEllo World");
 //Serial.begin (9600);   
}


char list[7][14] = {"Volt: ","Amp: ","Omh: ","Farad: ","Freq: "};
// the loop routine runs over and over again forever:
void loop() {

    float value = analogRead(A0)*5.0/1023 ;
     int   procVal = value/3.50 * 100;

     for(int i=0; i<4; i++){
      
        
       LcdXY(0,i);
       LcdWriteStringInt ("",i+1); 
       LcdXY(10,i);
       LcdWriteStringFloat (list[i],value);    
        
     }
       LcdXY(10,4);
       LcdWriteStringInt ("Procent:",procVal, "%  " );
       blink (2);
      
          
         LcdXY(40,5);
         LcdWriteString("| ");   
     	 delay(175);
    	 //blink(2);
         LcdXY(40,5);
         LcdWriteString("\\");   
         
         Serial.print("o");
         Serial.println("VOLT: ");
         Serial.print(value);

         if (procVal >= 100) 
            analogWrite(PIN_LCD_LIGHT, 0);  
         else if (procVal >= 10)
         	analogWrite(PIN_LCD_LIGHT, (float) ProcToValue (100 - procVal,  255 )); 
         else if(procVal > 1)
            analogWrite(PIN_LCD_LIGHT, (float) ProcToValue (99 ,  255 )); 
         else  
            analogWrite(PIN_LCD_LIGHT, 255);  

         delay(175);

       

}

