int led = 13;

//Functions 
void MovePixel();

void blink (int time_) {
digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(time_);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(time_);  

}

// void MoveSlash () {
//  AllRowsSelect ();
//  MovePixel();
//  blink(1);
// }

// void MovePixel(){
// LcdWriteData (0x01);
// LcdWriteData (0x03);
// LcdWriteData (0x08);
// LcdWriteData (0x30);
// LcdWriteData (0x80);
// //delay(10);
// }