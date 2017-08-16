#include "Arduino.h"
//Always 50 From 8333 Delay
#define Frequency 50
#define LEDBlinkFrequency 50
//  Working    : 8 , 7 , 4 , 12 , 13
// ~PWMWorking : 10, 9 , 3
//  Broken     : 6 , 5 , 11 
int PP = 9; // Always shoud be in PWM pins
int NN = 10 ;
int LED = 7;
int LED2 = 8;
bool LED_ActiveBool = false;

//delayMicroseconds(Hrz)  ;
void Posetive ();
void Negatyve ();
void MainLoop ();

void setup() {
  // put your setup code here, to run once:
  pinMode(PP,OUTPUT);
  pinMode(NN,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(LED2,OUTPUT);
  	//Serial.begin(9600);
  	//Serial.print ("Serial begin ...");

}
// Shoud Be delayMicroseconds(8333); == 58Hz
// delayMicroseconds(9775); == WithLEDLoad 50.70hz ,NoLEDLoad (9900) == 50.06 Hz
int Hrz = 9900;
int SecondCounter = 0;

void loop() {

MainLoop();
    //For Testing 
// analogWrite(NN,255);
// delay(300);
// analogWrite(NN,0);
// delay(300);	


} 




void Posetive (){
	analogWrite(PP,255);
	analogWrite(NN,0);
};
void Negatyve (){
	analogWrite(NN,255);
	analogWrite(PP,0);
};


void MainLoop () {
	// Led On At Second
	if (LED_ActiveBool && SecondCounter >= LEDBlinkFrequency) {
		//LED ON
		analogWrite(13, 255);
		//analogWrite(LED, 255);
		//analogWrite(LED2, 0);
	}
//------//Wave
	Posetive ();
	delayMicroseconds(Hrz);





	
		// Led On At Second
	if (!LED_ActiveBool && SecondCounter >= LEDBlinkFrequency) {
		//LED OFF
		analogWrite(13, 0);
		//analogWrite(LED, 0);
		//analogWrite(LED2, 255);
	}
//------//Wave	
	Negatyve ();
	delayMicroseconds(Hrz);
	//delay(1000);

	
	// Reset Second Counter
	if (SecondCounter > LEDBlinkFrequency ) {
			SecondCounter= 0;
			LED_ActiveBool = !LED_ActiveBool;
	}
		SecondCounter++;
}