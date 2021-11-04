/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#define RELAY 0
void setup() {
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.print("Completed loading");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RELAY, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  Serial.print("Low");
  delay(1000);                      // Wait for a second
  digitalWrite(RELAY, HIGH);  // Turn the LED off by making the voltage HIGH
  Serial.print("High");
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
