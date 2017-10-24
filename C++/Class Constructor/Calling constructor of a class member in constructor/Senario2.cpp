
// First Class Created

class myClass {
	public:
	int xMent;
	public:
	myClass (int _xment) { // Constr argument
		xMent  = _xment;
	}
	
	int getxMent () {
		return xMent;
	}
	
};

// Another Class Created
class FirstClass
{
	public:
	FirstClass(int _vx)  {}; // Constr argument
	int x () {return vx;}
	public:
	int vx;
};





class AnotherClass {
	myClass myClassObject; //1. Step
    FirstClass ClassTwo;
	public:
	AnotherClass (): myClassObject(666) { //<<<---- Error ClassTwo also are canditate ,  correct: AnotherClass (): myClassObject(666) , ClassTwo(546) {};
		
	}
	void test () {
		int mint = myClassObject.xMent;
	}
};


AnotherClass testThisObj;
//Error in 36 line 
/*


Compiling 'DrawTextTest' for 'Arduino/Genuino Mega w/ ATmega2560 (Mega 2560)'
DrawTextTest.ino: In constructor AnotherClass::AnotherClass()
 
DrawTextTest.ino: 70:36: error: no matching function for call to 'FirstClass::FirstClass()
  AnotherClass ()*: myClassObject(666) { \\ constructor initializer list
DrawTextTest.ino:70: note  candidates are
DrawTextTest.ino:56: note  FirstClass  FirstClass(int)
   FirstClass(int _vx)  {}; \\ Constr argument
DrawTextTest.ino:56: note    candidate expects 1 argument, 0 provided
DrawTextTest.ino:53: note  constexpr FirstClass  FirstClass(const FirstClass&)
   class FirstClass
DrawTextTest.ino:53: note    candidate expects 1 argument, 0 provided
DrawTextTest.ino:53: note  constexpr FirstClass  FirstClass(FirstClass&&)
DrawTextTest.ino:53: note    candidate expects 1 argument, 0 provided
Error compiling project sources
Build failed for project 'DrawTextTest'

*/