
#define BUTTON_UP A0
#define BUTTON_DOWN 12
#define BUTTON_SET 13

bool buttonUp;
bool buttonDown;
bool buttonSet;

#include "lib_meniuInterface16x2_LiquidCrystal_I2C.h"
#include "functions.h"

lib_meniuInterface16x2_LiquidCrystal_I2C menu( buttonUp, buttonDown,  buttonSet);

// the setup function runs once when you press reset or power the board
void setup() {
    menu.initiateDisplay();
    delay(500);

    menu.IncludeFunction(&func1, var1, "var1", "psi");
    menu.IncludeFunction(&func2, var2, "var2", "psi");
    menu.IncludeFunction(&func3, var3, "var3", "l/min");
    menu.IncludeFunction(&func4, var4, "var4", "val", false);
    menu.IncludeFunction(&func4, var5, "var5", "val", true);

    //separate functions to include more specific menu options
    menu.IncludeQuckAccessFunction(&func6, var6, "Quic Access", "psi", true);
    menu.IncludeFunctionSetDefault(&func7);


    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_UP, INPUT);
    
}

// the loop function runs over and over again until power down or reset
void loop() {
    buttonUp = digitalRead(BUTTON_UP);
    buttonDown = digitalRead(BUTTON_DOWN);
    buttonSet = digitalRead(BUTTON_SET);

    if (!menu.InterfaceDinamic()) {
        //menu.displayButtonsValue();

       // print(menu.func_stored[1].__functionName); // access a unit from menu memory 
        display.setCursor(0, 0);
        print(" are");


    }

}
