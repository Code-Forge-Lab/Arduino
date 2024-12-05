#include <Wire.h> // ESP32 Wrover Module  Com[piler]
#include <LiquidCrystal_I2C.h>


#define WIRE Wire


int counter = 0;
int counter1 = 0;
int counter2 = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 0x3F for a 16 chars and 4 line display but works also with 2 lines
void setup() {

  //WIRE.begin(); // Included I2cLCD
  Serial.begin(9600);



  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight
  lcd.clear();                      // Clear the LCD screen
}

bool isDeviceLCDFound = false;
void loop() {

 byte error, address;
  int nDevices;
  

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      isDeviceLCDFound = true;
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
      isDeviceLCDFound = false;
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else{
    if (!isDeviceLCDFound){
      lcd.init();                       // Initialize the LCD
      lcd.backlight();                  // Turn on the backlight
      lcd.clear();          

    }
                  // Clear the LCD screen
    Serial.println("done\n");
  }




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

