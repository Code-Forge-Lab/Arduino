//Structure
#include <stdio.h>
#include <stdarg.h> 
using namespace std;

struct menuLiquidCrystal {
   
    public:	 // Only works in public , private throw error 
    void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
    String functionName; // unique name of 
    String functionValue; // second value of function
    // Execute Selected menu option and point to responsive function
    
   

    void  DrawFunction () { //Expect Selected menu function
            
                    fnc_(); // How Call Pointed Function ;
               
        };
    
        
    void IncludeFunction (void (*functionPointer)(),String functionName = "Uknown" , String  functionValue = "" ) { // where magic on
            fnc_ = functionPointer;
            this->functionName = functionName;
            this->functionValue = functionValue;
        };	
        
    bool isEmptyFunctionValue () /// If any String paramiters are  in functionValue give true condition.
      {
             return (functionValue != "");
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

                void menuReset() {
                    menuSelected=0;
                  };
                
                int getMenuSelected () {return menuSelected ;};

                int getMenuLenght () {return menuLenght;};
                
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


// menuLiquidCrystal menu[3];
// menuLiquidCrystalNavigate navmenu;



// void initiate_functions () {
    
//     menu[0].IncludeFunction(&fun0,"MAXTEMP" , MAXTEMP.getValue()); 
//     menu[1].IncludeFunction(&fun1,"FANSPEED",FANSPEED.getValue()); 
//     menu[2].IncludeFunction(&fun2,"PELLETPUSHER",PELLETPUSHER.getValue()); 
//     navmenu.setmenuLenght (sizeof(menu)/sizeof(menu[0])) ; // find out about size 
//    }


//  4.Use with events and print execude function

        //   // print block
        //   lcd.clear();
        //   lcd.print ( menu[navmenu.getMenuSelected()].functionName);
        //   lcd.setCursor(0,1);
        //   lcd.print (menu[navmenu.getMenuSelected()].functionValue);

// if (__up) {
//     //        FANSPEED.addValue();
//           navmenu.menuUp();
          
//         }
    
//         if (__down) {
//     //        FANSPEED.subValue();
//               navmenu.menuDown();
          
//         }
      
       
//               if (__set) {
//     //              FANSPEED.setValue();
//                     menu[navmenu.getMenuSelected()].DrawFunction();                      
//                 }
