#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Water Level in Well 
const static int minimumWaterSensor = A0;
const static int minimuWaterSensorTransistor = 9; // Transistor that activate current going throw sensor.
int minimumWaterLevel = 0;
int minimumWaterCounter = 0;// delay of incominge logic pulses from sensor

//Hidrofore Relay
const static int relayHidrofore = 13;
int hidroforeSensorLevel = 0;
const static int hidroforeSensor = A1;
int  relayHidroforeCounter = 0;

//Beep No Water Indicator
const static int BeepModeIndicator = 11; //analog
int8_t BeepModeCounter = 0;

//ValveRelay open water from the city
const static int valveRelay = 8;
int  valveRelayCounter = 0;

//led1Indicator No Water INDICATOR
const static int led1ModeIndicator = 10;


//pots
const static int pot1_pin = A4;

int captureValuePot1 = analogRead(pot1_pin);

//exseption 
const static int8_t  exeptionCurrentSensor = A5;
bool  exeptionCurrentSensorBool = false;


bool modeManualAuto = false; // determinate if react to water level chages or not / Manual Not Chenck Water Level/ Auto Check Water Level /\ true=Manual
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void setup() {
	pinMode(minimumWaterSensor, INPUT);
	pinMode(minimuWaterSensorTransistor, INPUT);
	pinMode(exeptionCurrentSensor, INPUT);

	pinMode(relayHidrofore, OUTPUT);
	pinMode(led1ModeIndicator, OUTPUT);
	pinMode(valveRelay, OUTPUT);
	pinMode(BeepModeIndicator, OUTPUT);
	//analogWrite(led1ModeIndicator,255);//testing purpose
	//analogWrite(8, 255);//testing purpose
	analogWrite(BeepModeIndicator, 35);
	Serial.begin(9600);
	Serial.print("ON");
	display.begin();
	display.setContrast(60); //Set contrast to 50
	display.clearDisplay();
	display.display();
	analogWrite(BeepModeIndicator, 0);
}

int c = 0;
void loop() {

	++c;



	int  pot1 = analogRead(pot1_pin);
	bool waterLevelSensorRezultBool = WaterLevelSensorRead();
	bool hidroforeSensorRezultBool = HidroforeSensorRead();
	exeptionCurrentSensorBool = ExeptionCurrentSensorRead();


	if (pot1 > captureValuePot1 + 5 || pot1 < captureValuePot1 - 5) //chech if after some time pot value is chahged
	{
		captureValuePot1 = pot1;
		display.clearDisplay();
		display.setCursor(0, 5);
		display.setTextSize(2);

		if ((pot1 < 512))
		{
			display.print("Auto");
			modeManualAuto = false;
		}
		else {
			display.print("Manual");
			modeManualAuto = true;
		}
		display.display();
		delay(300);
		return;
	}

	//Serial.println("pot1:" + String(pot1));
	bool allOn = false;

	if (waterLevelSensorRezultBool && (hidroforeSensorRezultBool || exeptionCurrentSensorBool) || modeManualAuto && hidroforeSensorRezultBool) // if find out about sensor are hight then means water is enouth
	{

		onRelayHidrofore();

		allOn = true;
	}
	else {
		offRelayHidrofore();
		// if fails to drain water then on valve relay vto provide city water
		if ((exeptionCurrentSensorBool || hidroforeSensorRezultBool) && !modeManualAuto) {
			onRelayValve();
		}
		else
			offRelayValve();
	}

	// LED indicate if Water Is In Well
	if (!waterLevelSensorRezultBool && !modeManualAuto) {
		analogWrite(led1ModeIndicator, 20);
		onBeepModeIndicator();
	}
	else {
		analogWrite(led1ModeIndicator, 0);
		offBeepModeIndicator();
	}





	if (allOn && !exeptionCurrentSensorBool) { delay(100); display.clearDisplay(); display.setTextSize(2); display.setCursor(16, 0); display.println("FREE  WATER"); display.display(); return; };
	if (exeptionCurrentSensorBool) { delay(100); display.clearDisplay(); display.setTextSize(2); display.setCursor(16, 0); display.println("BOOST--|:|--"); display.display(); return; }
	display.setTextSize(1);
	display.clearDisplay();
	//display.setTextColor(BLACK, WHITE);
	display.setCursor(0, 0);
	display.println("W-level:" + String(minimumWaterLevel));
	//display.setCursor(0,20);
	display.println("Hidro:" + String(hidroforeSensorLevel));
	if (relayHidroforeCounter > 0) { display.println("Hidro-time:"); display.setTextSize(3); display.println(String(" ") + centerInt(relayHidroforeCounter)); }
	else if (valveRelayCounter > 0) { display.println("Valve-time:"); display.setTextSize(3); display.println(String("") + centerInt(valveRelayCounter)); }
	display.setTextSize(1);




	if (c >= 9) c = 0;
	display.display();

	delay(10);

	delay(500);
}

bool WaterLevelSensorRead() {
	analogWrite(minimuWaterSensorTransistor, 255); //turn on transitor that transmit vcurrent
	delay(4);
	minimumWaterLevel = analogRead(minimumWaterSensor);
	delay(4);
	//        Serial.println("WaterLevel:" + String(minimumWaterLevel)+ bol(minimumWaterLevel>50));
	analogWrite(minimuWaterSensorTransistor, 0);

	//UpdateTimerCount
	delay(4);

	minimumWaterCounter--;

	if (minimumWaterLevel > 500) { //bugdplst
		minimumWaterCounter = 20; // ~ substract 2 = time
	};



	if (minimumWaterCounter > 0)
		return true;
	return false;

}


bool HidroforeSensorRead() {

	analogWrite(minimuWaterSensorTransistor, 255);// activate +5 to go throw sensor
	delay(4);
	hidroforeSensorLevel = analogRead(hidroforeSensor);
	delay(4);
	//    Serial.println("Hidrofore:" + String(hidroforeSensorLevel)+ bol (hidroforeSensorLevel >50) );
	delay(2);
	return (hidroforeSensorLevel > 50);
}
void onRelayHidrofore() {
	analogWrite(relayHidrofore, 255);
	relayHidroforeCounter = 7;
};
void offRelayHidrofore() {
	if (relayHidroforeCounter <= 0)
		analogWrite(relayHidrofore, 0);
	else
		relayHidroforeCounter--; // fade out of fast swithing

};


bool ExeptionCurrentSensorRead() {
	int readthis = analogRead(exeptionCurrentSensor);

	if (readthis > 100) {
		Serial.println("Exeption: " + String(readthis));
		return true;
	}
	else {
		return false;
	}


};

void onRelayValve() {
	analogWrite(valveRelay, 255);
	valveRelayCounter = 10;
};
void offRelayValve() {
	if (valveRelayCounter <= 0)
		analogWrite(valveRelay, 0);
	else
		valveRelayCounter--; // fade out of fast swithing
};

void onBeepModeIndicator() {
	analogWrite(BeepModeIndicator, 0);
	if (BeepModeCounter <= 0) // set 1/3 sound and light signal on
		analogWrite(BeepModeIndicator, 35);

	BeepModeCounter++;

	if (BeepModeCounter >= 6)
		BeepModeCounter = 0;
}

void offBeepModeIndicator() {
	analogWrite(BeepModeIndicator, 0);
}
String centerInt(int txt) {
	if (txt > 10)
		return String(txt);
	else
		return (String(" ") + String(txt));
}

String bol(bool var) {
	if (var) return " true "; else return " false ";
}

