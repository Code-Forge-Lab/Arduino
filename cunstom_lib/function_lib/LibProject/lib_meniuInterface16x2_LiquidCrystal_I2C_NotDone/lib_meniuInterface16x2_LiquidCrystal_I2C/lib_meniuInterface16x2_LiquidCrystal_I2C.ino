
//#define BUTTON_UP A0
//#define BUTTON_DOWN 12
//#define BUTTON_SET 13

#define BUTTON_DOWN 5
#define BUTTON_SET 4
#define BUTTON_UP 6

bool buttonUp;
bool buttonDown;
bool buttonSet;

#include "lib_meniuInterface16x2_LiquidCrystal_I2C.h"
#include "functions.h"

lib_meniuInterface16x2_LiquidCrystal_I2C menu( buttonUp, buttonDown,  buttonSet);


unsigned long timer1Sec;


// the setup function runs once when you press reset or power the board
void setup() {
    menu.initiateDisplay();
    delay(500);

    menu.IncludeFunction(&func1, var1, "var1_TestTestTestTestTest", "psi");
    menu.IncludeFunction(&func2, var2, "var 2", "psi");
    menu.IncludeFunction(&func3, var3, "var3", "l/min");
    menu.IncludeFunction(&func4, var4, "var  4", "val", false);
    menu.IncludeFunction(&func5, var5, "var5", "val", true);

    //separate functions to include more specific menu options
    menu.IncludeQuckAccessFunction(&func6, var6, "Quick Access", "psi", true);
    menu.IncludeFunctionSetDefault(&func7);


    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_SET, INPUT);
    
    // set up memory default 
    if (readMemoryByte(0) != true) {

        print("MEMORY SET!");
        writeMemory(0, true);
        menu.defaultFunc();// call a default function to set a new values 
        menu.userSetValuesToMemory();
    };

    menu.userGetValues();
	
	Serial.begin(115200);

}

int inc = 0;

// the loop function runs over and over again until power down or reset
void loop() {
    buttonUp =! digitalRead(BUTTON_UP);
    buttonDown =! digitalRead(BUTTON_DOWN);
    buttonSet =! digitalRead(BUTTON_SET);

    if (!menu.InterfaceDinamic()) {
        display.setCursor(0, 1);
		
        //menu.displayButtonsValue();

       // print(menu.func_stored[1].__functionName); // access a unit from menu memory 
       
        print("are we there"+String (inc));

        
    }

    if (millis() > (long)(timer1Sec + 5000L) )
    {
        timer1Sec = millis();
        inc++;
    };

    
}
