Source can [be](https://www.brainy-bits.com/connect-and-use-a-spi-oled-display/) found.
U8glib are used.
## Requare libraries ##
- U8GLib 

##  ##

![connection](https://github.com/Code-Forge-Lab/Arduino/blob/master/arduino%20library/lib/OLED-SPI/schematic_edited-1024x936.png)

## Working Example ##

```c
#include "U8glib.h"





U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);  // D0=13, D1=11, CS=10, DC=9, Reset=8

const uint8_t brainy_bitmap[] PROGMEM = {
 0x00, 0x00, 0x03, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x46,
0x00, 0x00, 0x00, 0x00, 0xFC, 0x47, 0xC0, 0x00, 0x00, 0x01, 0xCE, 0x4C, 0x60, 0x00, 0x00, 0x03,
0x02, 0x58, 0x30, 0x00, 0x00, 0x03, 0x02, 0x58, 0x10, 0x00, 0x00, 0x02, 0x02, 0x58, 0x18, 0x00,
0x00, 0x03, 0x06, 0x4C, 0x18, 0x00, 0x00, 0x07, 0x04, 0x44, 0x18, 0x00, 0x00, 0x0D, 0x80, 0x40,
0x3C, 0x00, 0x00, 0x09, 0xC0, 0x40, 0xE6, 0x00, 0x00, 0x18, 0x78, 0x47, 0xC2, 0x00, 0x00, 0x18,
0x0C, 0x4E, 0x02, 0x00, 0x00, 0x1F, 0x86, 0x4C, 0x7E, 0x00, 0x00, 0x0E, 0xC6, 0xE8, 0xEE, 0x00,
0x00, 0x18, 0x43, 0xF8, 0x82, 0x00, 0x00, 0x10, 0x06, 0x4C, 0x03, 0x00, 0x00, 0x30, 0x0C, 0x46,
0x01, 0x00, 0x00, 0x30, 0x18, 0x46, 0x01, 0x00, 0x00, 0x10, 0x18, 0x43, 0x03, 0x00, 0x00, 0x18,
0x10, 0x43, 0x03, 0x00, 0x00, 0x1C, 0x70, 0x41, 0x86, 0x00, 0x00, 0x0F, 0xE0, 0x40, 0xFE, 0x00,
0x00, 0x09, 0x1E, 0x4F, 0x06, 0x00, 0x00, 0x08, 0x30, 0x43, 0x86, 0x00, 0x00, 0x0C, 0x20, 0x41,
0x86, 0x00, 0x00, 0x06, 0x60, 0x40, 0x8C, 0x00, 0x00, 0x07, 0x60, 0x40, 0xB8, 0x00, 0x00, 0x01,
0xE0, 0x41, 0xF0, 0x00, 0x00, 0x00, 0x38, 0xE3, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBE, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xCF, 0x82, 0x0C, 0x86, 0x46, 0x1F, 0xEF, 0xC3, 0x0C,
0xC6, 0xEE, 0x1C, 0xEC, 0xC7, 0x0C, 0xE6, 0x7C, 0x1C, 0xED, 0x8D, 0x8C, 0xFE, 0x38, 0x1C, 0xED,
0x8D, 0xCC, 0xDE, 0x38, 0x1D, 0xCD, 0xDF, 0xCC, 0xCE, 0x38, 0x1F, 0x8C, 0xF8, 0xEC, 0xC6, 0x38,
0x1F, 0xEC, 0x08, 0x0C, 0xC2, 0x18, 0x1C, 0xEC, 0x00, 0xC0, 0x00, 0x00, 0x1C, 0xFD, 0xFB, 0xC0,
0x00, 0x00, 0x1C, 0xFC, 0x63, 0x00, 0x00, 0x00, 0x1C, 0xEC, 0x63, 0xC0, 0x00, 0x00, 0x1F, 0xEC,
0x60, 0xC0, 0x00, 0x00, 0x1F, 0xCC, 0x63, 0xC0, 0x00, 0x00, 0x1F, 0x0C, 0x63, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x2B, 0x4F, 0x67,
0x42, 0x38, 0x7B, 0xEA, 0x86, 0xB2, 0x28, 0xC7, 

};

void draw(void) {
  u8g.drawBitmapP( 76, 5, 6, 50, brainy_bitmap);  // put bitmap
  u8g.setFont(u8g_font_unifont);  // select font
  u8g.drawStr(0, 30, "Temp: ");  // put string of display at position X, Y
  u8g.drawStr(0, 50, "Hum: ");
  u8g.setPrintPos(44, 30);  // set position
//  u8g.print(27, 0);  // display temperature from DHT11
  u8g.drawStr(60, 30, "c ");
  u8g.setPrintPos(44, 50);
//  u8g.print(87, 0);  // display humidity from DHT11
  u8g.drawStr(60, 50, "% ");
}

void setup(void) {
}

void loop(void) {
  

  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  delay(5000);  
}

```