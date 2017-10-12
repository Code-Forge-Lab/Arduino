#include <Arduino.h>
#include <Servo.h>
#include "Files/programs/Anemometer.h"
#include "Files/programs/WindTubineBreakServo.h"

void blk (int delayT = 1000,int pin =13);
WindTurbineBreakServo WindTurbineBreakServoObj;
//Anemometer AnemometerObj;


 Servo Break; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print ("Serial begin ...");
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT); 
   pinMode(9, OUTPUT);
   pinMode(8, OUTPUT);
  //BreakServo
 
//   Break.attach (9);
   
    
  WindTurbineBreakServoObj.IncludeWindTurbineBreakServoPin (9);
  WindTurbineBreakServoObj.turn (0);
  //AnemoMeter 
//AnemometerObj.FotoElectric_MinimumThreshold = 20 ; // Minimum = 2 or 13 / max 57  or 86 bit value
}

int c = 0;
int c_max = 600;

void loop() {
//int finger =  AnemometerObj.FotoElectricAveragePerTime(1000 , A0); // Keep Wathing Passing finger 
//int input = analogRead(A0);

 delay(6000);
 // digitalWrite(13,HIGH);
  delay(100);
 WindTurbineBreakServoObj.ServoBreakON();

 delay(8000);
 blk (3000);

 Serial.print (String ("Angle") + Break.read());
 WindTurbineBreakServoObj.ServoBreakOFF();
 delay(4000);


//WindTurbineBreakServoObj.turn ()

 // // Program per second 

   //if(c >= c_max ) {
 //       Serial.print ("Average,");
//        Serial.println (FotoElectric_AveragePerSelectedTime); 
   
    //}


 c++;
 if (c > c_max)
 {
 c = 0;
 
 }
 delay(1);
}



void blk (int delayT , int pin) {
	for(int i=0; i<2; i++){
	    
	
	digitalWrite(pin, HIGH);
	delay(delayT);
	digitalWrite(pin, LOW);
	delay(delayT);
}

}














