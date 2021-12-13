
//#define BUTTON_UP A0
//#define BUTTON_DOWN 12
//#define BUTTON_SET 13

byte BUTTON_DOWN =  5;
byte BUTTON_SET = 4;
byte BUTTON_UP = 6;

bool buttonUp;
bool buttonDown;
bool buttonSet;

#include "lib_meniuInterface16x2_LiquidCrystal_I2C.h"
#include "functions.h"

lib_meniuInterface16x2_LiquidCrystal_I2C menu( buttonUp, buttonDown,  buttonSet);


unsigned long timer1Sec;


// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
    delay (100);
    menu.initiateDisplay();
    delay(500);
    
    menu.IncludeFunction(&func1, var1, "var1_Tesstat", "psi");
    menu.IncludeFunction(&func2, var2, "var 2", "psi");
    menu.IncludeFunction(&func3, var3, "var3", "l/min");
    menu.IncludeFunction(&func4, var4, "var  4", "val", false);
    menu.IncludeFunction(&func5, var5, "var5", "val", true);

    //separate functions to include more specific menu options
    menu.IncludeQuckAccessFunction(&func6, var6, "Quick Access", "psi", true);
    menu.IncludeFunctionSetDefault(&func7);


    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_SET, INPUT_PULLUP);

   delay (100);
   
    // set up memory default 
    if (readMemoryByte(0) != true) {

        print("MEMORY SET!",2,0);
        
        writeMemory(0, true);
        
        delay (100);
        menu.defaultFunc();// call a default function to set a new values 
        menu.userSetValuesToMemory();
        delay (3000);
    };
    
delay (100);
    menu.userGetValues();
delay (100);
	//buttonSet = true;

}

int inc = 0;

// the loop function runs over and over again until power down or reset
void loop() {
    buttonUp = digitalRead(BUTTON_UP);
    buttonDown =! digitalRead(BUTTON_DOWN);
    buttonSet =! digitalRead(BUTTON_SET);

    if (!menu.InterfaceDinamic()) {
    //    display.setCursor(0, 1);
		
      //  menu.displayButtonsValue();

       // print(menu.func_stored[1].__functionName); // access a unit from menu memory 
       
        print("LOOT"+String (inc),1,0);

        
    }

    if (millis() > (long)(timer1Sec + 5000L) )
    {
        timer1Sec = millis();
        inc++;
    };

   // timerPrintAvailable = false;
}
