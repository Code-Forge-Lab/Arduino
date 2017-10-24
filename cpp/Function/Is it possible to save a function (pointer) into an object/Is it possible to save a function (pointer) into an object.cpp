//Structure
	struct array {
		
		public:	 // Only works in public , private throw error 
		void (*fnc_)(); // whiout bracket not working , looks like try to use casting method
		// Execute Selected menu option and point to responsive function
		void  DrawFunction () { //Expect Selected menu function
				
				        fnc_(); // How Call Pointed Function ;
					
			};
		
			
		void IncludeFunction (void (*functionPointer)() ) { // where magic on
				fnc_ = functionPointer;
			};	
			
	};
	
	void myFnc (){};
	array arrayTest[5];
	
	
	void main {
			// Only Works in function,objects or in main loop
			arrayTest[0].IncludeFunction(&myFnc) // also is working arrayTest[0].IncludeFunction( 	myFnc) giving address 
	}



// Passive Function FunctionVariable Argument 
void testfunc () {Serial.print ("WORKIN!!!!G"); delay (5000);}

void myFunc (void (*functionPointer)() = 0) {
	delay (3000);	
	functionPointer();	 //Works
}

myFunc (testfunc); // That Works
