#include <Wire.h> // ESP32 Wrover Module  Com[piler]
#include <LiquidCrystal_I2C.h>
int counter = 0;
int counter1 = 0;
int counter2 = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 0x3F for a 16 chars and 4 line display but works also with 2 lines
void setup() {
  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight
  lcd.clear();                      // Clear the LCD screen
}
void loop() {
  lcd.setCursor(0, 0);               // Set the cursor to the first column and first row
  lcd.print(" Pellet Burner 0/5   ");     // Print some text
  lcd.setCursor(0,1);
  lcd.print("Temperature: "+ String(counter) + "C");
  counter = counter + 1;

  lcd.setCursor(0,2);
  lcd.print("Fire: "+ String(counter1) + "LX");
  counter1 = counter1 + 2;

  lcd.setCursor(0,3);
  lcd.print("Fuel Left: " + String(counter2) + "%");
  counter2 = counter2 + 3;


  delay(1000);                        // Wait for 1/2 seconds
}

