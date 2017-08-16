
#include "SPI.h"

// Original Adafruit lib
/*/
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#define libName "Adafruit library"
//*/

// Updated Adafruit lib
/*/
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341_fast.h"
Adafruit_ILI9341 tft = Adafruit_ILI9341();
#define libName "modified Adafruit lib"
//*/

// PDQ_GFX
//*/
#include "PDQ_GFX.h"
#include "PDQ_ILI9341_config.h"
#include "PDQ_ILI9341.h"
PDQ_ILI9341 tft;
#define libName "PDQ_GFX library"
//*/

float volts = 12.345;
float amps = 1.234;
float watts = 0.0;

void setup() {
tft.begin();
tft.setRotation(1);
tft.fillScreen(ILI9341_WHITE);

tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);

tft.setTextSize(2);
tft.setCursor(20, 46);
tft.println(libName);

delay(2500); tft.fillScreen(ILI9341_WHITE);
}

void loop(void) {
volts += 0.001;
amps += 0.001;
watts = volts * amps;

tft.setTextSize(5);
tft.setCursor(20, 10);
tft.print(volts, 3);
tft.setTextSize(3);
tft.setCursor(204, 24);
tft.println("V");
tft.setTextSize(5);
tft.setCursor(20, 60);
tft.print(" ");
tft.print(amps, 3);
tft.setTextSize(3);
tft.setCursor(204, 74);
tft.println("A");
tft.setTextSize(5);
tft.setCursor(20, 110);
tft.print(watts, 3);
tft.setTextSize(3);
tft.setCursor(204, 124);
tft.println("W");

tft.setTextSize(2);
tft.setCursor(20, 158);
tft.print("SET ");
tft.print(volts, 3);
tft.println(" V");
tft.setCursor(20,178);
tft.print(" ");
tft.print(amps, 3);
tft.println(" A");

tft.setTextSize(2);
tft.setCursor(20, 198);
tft.print("LIMIT ");
tft.print(24.000, 3);
tft.println(" V");
tft.setCursor(20,218);
tft.print(" ");
tft.print(2.000, 3);
tft.println(" A");

delay(200);
}311807