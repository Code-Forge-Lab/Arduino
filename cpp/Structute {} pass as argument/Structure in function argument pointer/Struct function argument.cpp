//Exsample1
//Can Work with multyple struct in the function
struct Items
{
	char code[10];
	char description[30];
	int stock;
};

void ReadFile(struct Items items[]) //items[10] can be also
{
		items[0].stock = 5;
}

void xxx()
{
	struct Items MyItems[10];
	ReadFile(MyItems);
}

//Exsample2
struct X { // struct's as good as class
	static void a();
	
};

void X::a() {int x = 5;};


//Exsample3
//Only Working with single struct using ponters or references
struct myStrut {
	public:
	int x , y;
	int *functionMy();
};

void Testobject ( myStrut* abc) {
	abc->x = 5;
	
	};
	
	
	
//Exsample4
//Only Working with single struct using ponters or references
//How define struct with custom iniliazed name in function with pointer
struct sampleData {
    int N;
    int M;
    string sample_name;
    string speaker;
};

bool data(sampleData *samples)
{
    samples->N = 10;
    samples->M = 20;
    // etc.
}

//Exsample3


struct myStrut {
	public:
	int x , y;
	int functionMy();
};


typedef struct myStrut Card ; // Custom Define Name	

void Testobject ( Card* abc) {
	abc->x = 5;
	//abc->functionMy () {return 3;};
	};

