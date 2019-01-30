const static int stepPin = 3;
const static int dirPin = 4;

void setup() {
  // put your setup code here, to run once:

 pinMode (stepPin, OUTPUT);
 pinMode (dirPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite (dirPin, HIGH);


  for (int x = 0; x < 200; x++) {
    
      digitalWrite (stepPin,HIGH);
      delayMicroseconds (500);
      digitalWrite(stepPin LOW);
      delayMicroseconds (500);
      
    }

delay (1000);
}
