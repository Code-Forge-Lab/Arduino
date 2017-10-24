```c
//Structure
struct menuLiquidCrystal {
    public:
        int16_t menuLenght;
    public:	 // Only works in public , private throw error 
    void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
    // Execute Selected menu option and point to responsive function
    
    menuLiquidCrystal  ( void (*functionPointer)()   )  {
      
        fnc_ = functionPointer;
        
        // this->menuLenght = menuLenght; // lenght of array 
    }

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

void fun0 (){lcd.clear();  lcd.print("Page1:");}; //function where caling
 void main {
    menuLiquidCrystal menu(&fun0); // declarate
    menu.DrawFunction();
 }
 ///
 struct MyStruct PassedStruct[]
is mostly an alternative syntax for:

struct MyStruct * PassedStruct
So yes, you will access and modify the original structure.

Just one detail to change, the correct call to to the function is not

myFunction(StructArray[]);
but:

myFunction(StructArray);
///
 header: void myFunction(struct MyStruct * PassedStruct)
caller: myFunction(StructArray)
status: works, you work with a pointer in PassedStruct
header: void myFunction(struct MyStruct PassedStruct[])
caller: myFunction(StructArray)
status: works, you work with a pointer in PassedStruct
header: void myFunction(struct MyStruct (& PassedStruct)[10])
caller: myFunction(StructArray)
status: works, you work with a reference to an array of size 10
header: void myFunction(struct MyStruct (& PassedStruct)[11])
caller: myFunction(StructArray)
status: does not compile, type of array mismatch between prototype and actual parameter
header: void myFunction(struct MyStruct PassedStruct[10])
caller: myFunction(StructArray)
status: works, PassedStruct is a pointer, size provided is ignored
header: void myFunction(struct MyStruct PassedStruct[11])
caller: myFunction(StructArray)
status: works, PassedStruct is a pointer, size provided is ignored
///
void foo (char arr[])
into this:

void foo (char *arr)
 
 ```
