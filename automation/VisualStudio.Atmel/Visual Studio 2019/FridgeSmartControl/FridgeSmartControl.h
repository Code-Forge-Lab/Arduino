#pragma once

/*
 Name:		FridgeSmartControl.ino
 Created:	9/5/2019 11:42:28 AM
 Author:	zick
*/

#include <Arduino.h>
//#include "ThermoSensor.h"
#include <SPI.h>
#include <avr/power.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ThermoSencor.h"



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// buttons
#define BUTTON_DOWN 10
#define BUTTON_SET 12
#define BUTTON_UP 11
// sensors
#define SENSOR_TEMP0 A1
#define SENSOR_TEMP1BACKUP A4
// relays
#define RELAY_FRIDGE0 13
#define RELAY_FRIDGE1BACKUPRead A4

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ThermoSencor sensorTemp(A3, 8740);

void setup() {

	// This runs MCU at full speed at 32Mhz for LGT8F328P  as ATMEGA328P clone.
	clock_prescale_set(clock_div_1);

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
	  //Serial.println(F("SSD1306 allocation failed"));
		for (;;); // Don't proceed, loop forever
	}
	delay(100);
	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display.display();
	delay(500); // Pause for 2 seconds

	// Clear the buffer
	display.clearDisplay();


	//display.setTextColor(WHITE);        // Draw white text // not printing if aren't use

	// Vertical 64 small lines

	// include buttons
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_SET, INPUT);
	pinMode(BUTTON_UP, INPUT);

	// include temp. sensor
	pinMode(SENSOR_TEMP0, INPUT);

	// include control fridge relay
	pinMode(RELAY_FRIDGE0, OUTPUT);
	pinMode(RELAY_FRIDGE1BACKUPRead, INPUT);

}

float temp = 30;

void loop() {
	int_fast16_t buttonUp = digitalRead(BUTTON_UP);
	int_fast16_t buttonSet = digitalRead(BUTTON_SET);
	int_fast16_t buttonDown = digitalRead(BUTTON_DOWN);


	
	

	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextColor(WHITE);  // Draw white text // not printing if aren't use
	display.setTextSize(1);
	display.println("buttonUp:" + String(buttonUp));
	display.println("buttonSet:" + String(buttonSet));
	display.println("buttonDown:" + String(buttonDown));

	if (buttonUp)
		digitalWrite(RELAY_FRIDGE0, 255);
	else if (buttonSet)
		digitalWrite(RELAY_FRIDGE0, 0);


	txtTemperature(sensorTemp.temperature);
	temp = temp - 1.1;
	if (temp < -140) temp = 35;
	delay(1000);
	display.display();
}

// Print temperature in screen
void txtTemperature(float value) {
	display.setCursor(0, 29);
	display.setTextSize(4);

	display.setTextColor(WHITE);
	display.println(subFloatDec(value));
};

String subFloatDec(float value) {
	String strValue = String(value);
	return (strValue.length() > 6) ? (strValue.substring(0, strValue.length() - 2)) : (strValue.substring(0, strValue.length() - 1));
};

void functionTest() {


	display.clearDisplay();

	display.setTextSize(1);             // Normal 1:1 pixel scale
	display.setTextColor(WHITE);        // Draw white text
	display.setCursor(0, 0);             // Start at top-left corner


	display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display.println(3.141592);

	display.setTextSize(6);             // Draw 2X-scale text


	txtTemperature(temp + .555);

	display.display();
	delay(1000);
	temp = temp - 1.1;
	if (temp < -140) temp = 35;
}
