#include "nokia5110.h"
#include "myFunk.h"
//#include "E:\MyFolder\Script\Arduino\Projects\Library\myTest.h"
//#include "E:\MyFolder\Script\Arduino\Projects\Library\procent.h"
short int PIN_LCD_LIGHT = 9;

//Raw Input From Batery Volt Data
short int BatVoltIn = 12;  // volt from batery
short int BatRez1 = 100000; // Firt Rezistor  100k 
short int BatRez2 = 10000;  // Second Rezistor 10k
float CalculateBatVoltage () {
  return ( (BatVoltIn*BatRez2) / (BatRez1 + BatRez2) ); //??
}

char list[7][14] = {":A0:",":A1:"};
void WriteOnScreenLine (int xPosMonitor , float value) {
       LcdXY(0,xPosMonitor);
       LcdWriteStringInt ("",xPosMonitor+1); 
       LcdXY(10,xPosMonitor);
       LcdWriteStringFloat (list[xPosMonitor],value);
}

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);

  analogWrite(PIN_LCD_LIGHT, 135);   
  SETUPLCD();
  //AllRowsSelect ();
  //LcdWriteString ("HEllo World");
 //Serial.begin (9600);   
}




// the loop routine runs over and over again forever:
void loop() {

    float value0 = analogRead(A0)*54.56/1023 ; // Loks great 
     float value1 = analogRead(A1);
     
     float listVal[7] = {value0, // Volt Reader
                         value1 // Pot Value
                        };
     int   procVal = value0/3.50 * 100;

      
      WriteOnScreenLine ( 0 ,  value0) ;
      WriteOnScreenLine ( 3 ,  value1) ; 
        
       // LcdXY(10,4);
       // LcdWriteStringInt ("Procent:",procVal, "%  " );
       // blink (2);
      
          
         LcdXY(40,5);
         LcdWriteString("| ");   
     	 delay(175);
    	 //blink(2);
         LcdXY(40,5);
         LcdWriteString("\\");   
         
         Serial.print("o");
         Serial.println("VOLT: ");
         Serial.print(value0);

         // if (procVal >= 100) 
         //    analogWrite(PIN_LCD_LIGHT, 0);  
         // else if (procVal >= 10)
         // 	analogWrite(PIN_LCD_LIGHT, (float) ProcToValue (100 - procVal,  255 )); 
         // else if(procVal > 1)
         //    analogWrite(PIN_LCD_LIGHT, (float) ProcToValue (99 ,  255 )); 
         // else  
         //    analogWrite(PIN_LCD_LIGHT, 255);  

         delay(800);

       

}

