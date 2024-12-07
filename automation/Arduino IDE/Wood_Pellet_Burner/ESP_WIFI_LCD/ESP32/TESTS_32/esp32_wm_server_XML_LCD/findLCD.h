#include <Wire.h> // ESP32 Wrover Module  Com[piler]
#include <LiquidCrystal_I2C.h>
#define WIRE Wire

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 0x3F for a 16 chars and 4 line display but works also with 2 lines


 byte error, address;
bool  isDeviceLCDFound = false;
 void findLCD () 
 {
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
      }
 }


 void lcdPrint (String txt , bool clear = false ,  int x = 0 , int y = 0 ){
    if (clear)
     lcd.clear();   
     lcd.setCursor(x,y);
     lcd.print(txt);
 }