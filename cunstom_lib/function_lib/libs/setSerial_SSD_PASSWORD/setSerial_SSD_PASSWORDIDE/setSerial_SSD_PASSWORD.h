//Structure
#include <stdio.h>
#include <stdarg.h> 
#include <Arduino.h>
using namespace std;

struct serialSetSSDPassword {
   int setMemPos;

    public:

    serialSetSSDPassword (int setMemPos)   {
           this-> setMemPos =  setMemPos;
    }


    void init () {

    	Serial.begin (115200);

    }

    void getSerialHelp () {

        Serial.println("memPos: "+String (setMemPos));
    }

};

