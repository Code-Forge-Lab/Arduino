//How a locate Dynamic Struct 
class myClass {
	//Struct
	private:
	struct SaveTextInt {
		int value_ =-1;
		};
		
	 SaveTextInt* textIntData; // initialize dynamic constructor	
	public:	
		myClass ( int arraySizeAlocate = 0 )
		{
			// Dynamic allocating memory from variable value
			textIntData = new SaveTextInt[arraySizeAlocate]; // Giving Size of Struct That look is  working 
		}
		
		//De constructor
		~myClass() {
			delete[] textIntData; // To void memory leaks
		}
	};


class MainClass {
	private:
	
	public:
	 myClass myClassObject ; //Creating Object 
	 public:
	 
	 MainClass (): myClassObject(5) {} // Passing Variables into myClassObject  using constructor initializer list
	};

MainClass classOK;
