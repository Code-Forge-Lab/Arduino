//Structure
#include <stdio.h>
#include <stdarg.h> 
using namespace std;

struct menuLiquidCrystal {
   
    public:	 // Only works in public , private throw error 
    void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
    // Execute Selected menu option and point to responsive function
    
   

    void  DrawFunction () { //Expect Selected menu function
            
                    fnc_(); // How Call Pointed Function ;
                delay (3000);
        };
    
        
    void IncludeFunction (void (*functionPointer)() ) { // where magic on
            fnc_ = functionPointer;
        };	
        
       
        
};


// void main {
//         // Only Works in function,objects or in main loop
//         arrayTest[0].IncludeFunction(&myFnc) // also is working arrayTest[0].IncludeFunction( 	myFnc) giving address 
// }

struct menuLiquidCrystalNavigate {
    
private:
    int16_t menuLenght;
   struct  menuLiquidCrystal *tmenu;
public:
    // menuLiquidCrystalNavigate ( struct menuLiquidCrystal *menuload ) {
    //      tmenu = menuload;
    // }

                void loadMenuLiquidCrystal (struct menuLiquidCrystal  *menuload) {
                    tmenu =   menuload;
                    
                    Serial.print ("SIZE:: " +String (sizeof (tmenu) ));
                    tmenu[2].DrawFunction();
                    delay (4000);

                    
                }

                void size () {
                    
                };

                void menuUp () {
        
                 }; 
                void menuDown () {};
                
                void setmenuLenght (int16_t len) {
                    menuLenght = len;
                };

};