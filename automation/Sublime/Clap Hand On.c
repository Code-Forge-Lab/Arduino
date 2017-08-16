

int ledPin = 12;
int analgoPin =  10;

void LightLED (int , int);
void AnalogLED (int, int);
void setup() {

  pinMode (ledPin, OUTPUT);
  Serial.begin(9600);



}

float maxSencor = 0;
float clap2 = 0;
float ClapTresHold = 3;
float sensorValue;
bool toggleClap = false;
void loop() {

  //   digitalWrite(ledPin,HIGH);
  //   delay(5000);
  //    digitalWrite(ledPin,LOW);


  // adding a variable integer to a string:
  sensorValue = analogRead(A0);

  // Serial.print("Sencor value ");
  // Serial.print(sensorValue);

  // Serial.print(" max: " );
  //  Serial.print(maxSencor);
  //  Serial.println("::");

  //Serial.println(".");


  if (sensorValue > ClapTresHold  )
  {

    bool canWait = false;
    if (sensorValue <= clap2)
      canWait = true;

    clap2 = sensorValue;
    // try void sleep time
    if (canWait)
      return; // can't wait then jumpend
    Serial.println("CLAPED!");
    
    //Loop While Not Get Hither Value
    int sleep = 0;
    delay(50);
    while (sensorValue <= clap2 && sleep < 2400) {
      sensorValue = analogRead(A0);
      delay(1);
      sleep++;
    }


    // if Clap Streng More Then Was Then Shoud Toggle Light
    if (sensorValue >= clap2)
    {
      toggleClap = ! toggleClap;
      if (toggleClap) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED ON");
      } else {
        digitalWrite(ledPin, LOW);
        Serial.println("LED OFF");

      }
      Serial.print("Clap Success: ");
      Serial.print(clap2);
      Serial.print(" get: ");
      Serial.println(sensorValue);
      delay(1000);

    } else {
      Serial.print("Clap strenght to low expected: ");
      Serial.print(clap2);
      Serial.print(" get: ");
      Serial.println(sensorValue);
      //  delay(5000);

    }
  } else {
    if (sensorValue <= ClapTresHold && sensorValue > 1  ) {
      Serial.print("Voltage is To low: ");
      Serial.println(sensorValue);
    }
  }





  delay (1);
}


