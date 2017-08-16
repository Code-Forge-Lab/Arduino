#include "Arduino.h"
int PP = 11; // Always shoud be in PWM pins
int NN = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(PP,OUTPUT);
  pinMode(NN,OUTPUT);

}

int Hrz = 1;
void loop() {
  // put your main code here, to run repeatedly:

digitalWrite(PP,HIGH);
digitalWrite(NN,LOW);
delayMicroseconds(10)  ;
//2
digitalWrite(NN,HIGH);
digitalWrite(PP,LOW);
delayMicroseconds(10)  ;

}