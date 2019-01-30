const static int stepPinX = 2;
const static int dirPinX = 5;

const static int stepPinY = 3;
const static int dirPinY = 6;


const static int stepPinZ = 4;
const static int dirPinZ = 7;


void setup() {
  // put your setup code here, to run once:

 pinMode (stepPinX, OUTPUT);
 pinMode (dirPinX, OUTPUT);

 pinMode (stepPinX, OUTPUT);
 pinMode (dirPinX, OUTPUT);

 pinMode (stepPinX, OUTPUT);
 pinMode (dirPinX, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:


/*
  digitalWrite (dirPinX, HIGH);


  for (int x = 0; x < 200; x++) {
    
      digitalWrite (stepPinX,HIGH);
      delayMicroseconds (500);
      digitalWrite(stepPinX, LOW);
      delayMicroseconds (500);
      
    }

delay (1000);


digitalWrite (dirPinZ, HIGH);

for (int x = 0; x < 200; x++) {
    
      digitalWrite (stepPinZ,HIGH);
      delayMicroseconds (500);
      digitalWrite(stepPinZ, LOW);
      delayMicroseconds (500);
      
    }

delay (1000);

*/



spinStepper (dirPinX, "right", stepPinX);
delay(1000);
spinStepper (dirPinX, "left", stepPinX);
delay(1000);


spinStepper (dirPinZ, "right", stepPinZ);
delay(1000);
spinStepper (dirPinZ, "left", stepPinZ);
delay(1000);

};


void spinStepper (int directionPin, String direction, int stepPin) {

int dir = LOW;

  if (direction == "right")
      dir = HIGH;  
 
     digitalWrite (directionPin, dir);
     

for (int x = 0; x < 200; x++) {
    
      digitalWrite (stepPin,HIGH);
      delayMicroseconds (500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds (500);
      
    }
    
  }
