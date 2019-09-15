#pragma once
// Simple I2C test for ebay 128x64 oled.
// Use smaller faster AvrI2c class in place of Wire.
// Edit AVRI2C_FASTMODE in SSD1306Ascii.h to change the default I2C frequency.
//
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1




SSD1306AsciiAvrI2c oled;
//------------------------------------------------------------------------------
void setup() {

#if RST_PIN >= 0
	oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
	oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
	// Call oled.setI2cClock(frequency) to change from the default frequency.



	uint32_t m = micros();
	oled.clear();
	oled.set1X();
	//oled.println("ASDFGHJKLQWERTYUIOPZXCVBNM");
	//oled.set2X();
	//oled.println("Jonuk ~");

	//oled.set2X();
	oled.println(" ");
	//oled.println("# & @ % ~");
	//oled.println("Hello");

	//oled.println();
	//oled.set2X();
	//oled.set2X();
	oled.setFont(ZevvPeep8x16);
	oled.setFont(CustomFont16);
	oled.println("Aa Bb Cc Dd Ee Ff");
	oled.setFont(CustomFont10x14);
	oled.println("Aa Bb Cc Dd Ee Ff");

	//oled.println(" Gg Hh Ii Yy Jj Kk");
	//oled.println("Ll Mm Nn Oo Pp Rr");
	//oled.println(" Ss Tt Uu Vv Zz Xx");
	//oled.println(" Qq Xx Ww");
	//oled.set1X();


	//oled.print("\nmicros: ");
	//oled.print(micros() - m);
}
//------------------------------------------------------------------------------
void loop() {}