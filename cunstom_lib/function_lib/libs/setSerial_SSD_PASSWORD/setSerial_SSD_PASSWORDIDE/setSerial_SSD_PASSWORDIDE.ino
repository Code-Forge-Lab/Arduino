#include "setSerial_SSD_PASSWORD.h"

serialSetSSDPassword setSSD(100);

void setup() {
  // put your setup code here, to run once:

setSSD.init ();

}

void loop() {
  // put your main code here, to run repeatedly:

setSSD.getSerialHelp ();
delay (1000);
}
