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


class AnotherClass {
	myClass myClassObject; //1. Step

	public:
	AnotherClass (): myClassObject(666) { // constructor initializer list
		
	}
	void test () {
		int mint = myClassObject.xMent;
	}
};

AnotherClass testThisObj;