//Structure
#include <stdio.h>
#include <stdarg.h> 
using namespace std;
struct menuLiquidCrystal {
    public:
        int16_t menuLenght;
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
        
        void menuUp () { }; 
        void menuDown () {};
        

        
};




// void main {
//         // Only Works in function,objects or in main loop
//         arrayTest[0].IncludeFunction(&myFnc) // also is working arrayTest[0].IncludeFunction( 	myFnc) giving address 
// }
