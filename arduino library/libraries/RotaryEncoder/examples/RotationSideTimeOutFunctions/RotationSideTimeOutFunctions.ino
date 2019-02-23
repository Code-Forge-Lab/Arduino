// -----
// SimplePollRotator.ino - Example for the RotaryEncoder library.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// -----

// This example checks the state of the rotary encoder in the loop() function.
// The current position is printed on output when changed.

// Hardware setup:
// Attach a rotary encoder with output pins to A2 and A3.
// The common contact should be attached to ground.

#include <RotaryEncoder.h>

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A0, 12);

void setup()
{
  Serial.begin(9600);
  Serial.println("SimplePollRotatorTimeOut example for the RotaryEncoder library.");
} // setup()


// Read the current position of the encoder and print out when changed.

int oldVal;

bool reapet = true;

void loop()
{





 
// value mode 
if (encoder.getPosition() > 25 || !reapet) {
   int val = encoder.getPositionSideTimeout ();
   
     if (val == 1) {
        Serial.println ("up++:");
      } else if  (val == -1) {
        Serial.println ("down--:");  
      }
      reapet = false;
  }
else if (reapet)
{ // condition mode after 25 count
   int  newVal = encoder.getPositionTimeout (50); // <-- 50 miliseconds delay to capture rotory encoder event

     if (newVal != oldVal) {
          Serial.println (":"+String(newVal) );
        oldVal = newVal;
      }
     
  }
 





 delay(100); // do something else

  
} // loop ()

// The End


