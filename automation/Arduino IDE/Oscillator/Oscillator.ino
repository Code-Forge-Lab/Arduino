#include "Arduino.h"
int PP = 9; // Always shoud be in PWM pins
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
delayMicroseconds(Hrz)  ;
delay (1000);
//2
digitalWrite(NN,HIGH);
digitalWrite(PP,LOW);
delayMicroseconds(Hrz)  ;
delay (1000);
}
