//#include <stdio.h>

void print();
void execute(void());


    
    
//Main Arduino Loop
void loop() {
	execute(print); // sends address of print
}

void print()
{
    Serial.print("Hello!");
}

void execute(void f()) // receive address of print // f() can be any name 
{
    f();
}