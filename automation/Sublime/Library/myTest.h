


void Dance (int time_) {
 for (int x = 0; x < 10; ++x)
 {
 digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(time_*x);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(time_*x); 
 }
  

}