#include "Arduino.h"
#include "../lib/Voltmeter.h"
void setup () {
Serial.begin(9600);
}

		VoltMeter voltage(A4,0.0,10000.0,10000.0);;

int main(int argc, char const *argv[]) {

  voltage.VoltageMeterUpdate();

  Serial.println("voltage: " + String (voltage.voltage) );

  delay(1000);

  return 0;
}
