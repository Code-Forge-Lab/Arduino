int PP = 5; // Always shoud be in PWM pins
int NN = 6;


int opacity = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(PP,OUTPUT);
  pinMode(NN,OUTPUT);
  pinMode(13, OUTPUT);

}

int Hrz = 1000;
void loop() {
  // put your main code here, to run repeatedly:

analogWrite(PP,opacity);
analogWrite(NN,LOW);
digitalWrite(13, HIGH);
//delayMicroseconds(Hrz)  ;
delay(Hrz);
//2

analogWrite(NN,opacity);
analogWrite(PP,LOW);
digitalWrite(13, LOW);
//delayMicroseconds(Hrz)  ;
delay(Hrz);
 if(opacity >= 255){
     opacity = 0;
     analogWrite(PP,255);
	 analogWrite(NN,255);
	 digitalWrite(13, HIGH);
	 delay(3000);
 }else
 opacity+=36;
}