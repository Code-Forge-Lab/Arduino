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
    int16_t menuSelected;
   
public:
     menuLiquidCrystalNavigate (  ) {
        menuSelected=0;
     }

               

                void menuUp () {
                        if (menuSelected < menuLenght -1)
                              ++menuSelected;
                 }; 
                void menuDown () {
                    if (menuSelected > 0 )
                    --menuSelected;
                };
                
                int getMenuSelected () {return menuSelected ;};

                void setmenuLenght (int16_t len) {
                    menuLenght = len;
                    // Serial.print ("SIZE: " + String (menuLenght));
                    // delay (2000);
                };

};

// //How to use it 
// 1.Create functions 

// void fun0 (){lcd.clear();  lcd.print("Page1:");};
// void fun1 (){lcd.clear();  lcd.print("Page2:" );};
// void fun2 (){lcd.clear();  lcd.print("Page3:" );};
// void fun3 (){lcd.clear();  lcd.print("Page4:" );};

// 2.Create objects
// menuLiquidCrystal menu[4];
// menuLiquidCrystalNavigate navmenu;

// 3.Initiate all  and then use in  void setup() {}

// void initiate_functions () {
    
//     menu[0].IncludeFunction(&fun0); 
//     menu[1].IncludeFunction(&fun1); 
//     menu[2].IncludeFunction(&fun2); 
//     menu[3].IncludeFunction(&fun3); 
 
 
//     navmenu.setmenuLenght (sizeof(menu)/2) ; // give dinamic size
//    }

//  4.Use with events and print execude function
//  if (__up) {
//     //        FANSPEED.addValue();
//           navmenu.menuUp();
//           menu[navmenu.getMenuSelected()].DrawFunction();
//         }
    
//         if (__down) {
//     //        FANSPEED.subValue();
//           navmenu.menuDown();
//           menu[navmenu.getMenuSelected()].DrawFunction();
//         }