#include <Arduino.h>
#include "ThermoSencor.h"

const static int8_t LED_ACTIVE = 3;// Led inditation of runnung program and condition in some point
const static int8_t WATERIN = 4; // Solenoid allowing to get water 
const static int8_t DISHPUNP =5; // Pump and push water throw dish washing spinners
const static int8_t WATERHEATER = 6; // Heat a water in dish wather.  2048Watt of power.
const static int8_t THERMOSENSOR = A1;  // Temperature in washing mashine 
const static int8_t  VREZPRG = A2; // Potentiometer that each selected value equal to separate program
const static int8_t TRIGGERBUTTON = A3; // Beggin or stop program, soft swith button connected to microcontroler .

void setup() {
    // put your setup code here, to run once:
    pinMode(3,OUTPUT); //LED +
    pinMode(4,OUTPUT); //relay +
    pinMode(5,OUTPUT); //relay + 
    pinMode(6,OUTPUT); //relay -
    pinMode(7,OUTPUT); //relay Heater 2048w  -
    

    pinMode (13,OUTPUT);

    pinMode(A3,INPUT); // Activation swith
    pinMode(A2,INPUT); // potentiometer 
    pinMode(A1,INPUT); // termo sensor
    Serial.begin(9600);
}

ThermoSencor Water(THERMOSENSOR);


void TestActiveComponent () {

    digitalWrite (13,HIGH);
    delay (500);
    digitalWrite (13,LOW);

    for (int8_t c = 3; c<=7;c++) {
        
        digitalWrite (13,HIGH);
        delay (350);
        digitalWrite (13,LOW);
        
        
        digitalWrite (c,HIGH);
        delay (1800);
        digitalWrite (c,LOW);
        delay (1800);
        Serial.print ("RELAY:"+String (c)+"\n");
    }
}


class __VRezPrg { // Variable potentiometer/Variable resistor Program Select Per Turn
private:
public:
    // VRezPrg () {};
    int8_t active;
    int8_t get() {return active;};
     
    void read () { active = analogRead (VREZPRG); }; //A2
}VRezPrg;


class __TriggerButton { // Button That activate selected program or use as brake
    
public:
    // TriggerButton () {};
    int active;
     bool get() {
    //         int tmpR = analogRead (3);
    //         Serial.println (tmpR);
            if (active > 13 ) return true; 
                else 
             return false;
    };
     
    void read () { active = analogRead (TRIGGERBUTTON); }; // A3
}TriggerButton;

void loop() {
    // put your main code here, to run repeatedly:
    
    Water.SingleImpreciseUpdate ();

    // int8_t PotPrg = analogRead (2);
    // int8_t Button = analogRead (3);

     VRezPrg.read();
     TriggerButton.read();

    
     Serial.println ("A2.Swith:" + String (TriggerButton.active) + ", A3.PotPrg:" + String (VRezPrg.active) + ",A1.Temp:" + String( Water.Temperature) +   "\n");

        if (TriggerButton.get() ) TestActiveComponent ();

    // digitalWrite (13,HIGH);
    // digitalWrite (7,HIGH);
    // delay (2500);

    // digitalWrite (13,LOW);
    // digitalWrite (7,LOW);
    // delay (2800);
    
   
    delay (500);

}



// int8_t VRezPrg () {return analogRead (2);}; // Variable potentiometer/Variable resistor Program Select Per Turn
// int8_t TriggerButton () {return analogRead (2);}; // Button That activate selected program or use as brake
