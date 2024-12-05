#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int counter = 0;
int counter1 = 0;
int counter2 = 0;
LiquidCrystal_I2C lcd(0x27, 16, 4); // Set the LCD address to 0x27 0x3F for a 16 chars and 4 line display but works also with 2 lines
void setup() {
  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight
  lcd.clear();                      // Clear the LCD screen
}
void loop() {
  lcd.setCursor(0, 0);               // Set the cursor to the first column and first row
  lcd.print("    INOVATRIX   ");     // Print some text
  lcd.setCursor(0,1);
  lcd.print(counter);
  counter = counter + 1;

  lcd.setCursor(0,2);
  lcd.print(counter);
  counter1 = counter1 + 1;

  lcd.setCursor(0,3);
  lcd.print(counter);
  counter2 = counter2 + 1;


  delay(500);                        // Wait for 1/2 seconds
}

