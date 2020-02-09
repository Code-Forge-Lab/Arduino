

void userSetDefault() {
	 var_one = 10;
	 var_two = 22;
	 var_three = 33;
	 var_four = 44;
	 var_five = 55;
	 //var_six = 66;
	
};




void func1() { print("func1 " + String(var_one)); menu.menuPrintManuallyValue(map(var_one, 0, 255, 0, 1024)); };
void func2() { print("func2 " + String(var_two)); };
void func3() { print("func3 ignore! "); menu.menuPrintManuallyValue(333);/*not works,good*/ };
void func4() { print ("func4 "); }; // map(var_four, 0, 255, 0, 1024)
void func5() { print("func5 " + String(var_five)); menu.menuQuckAccesPrintManuallyValue(map(var_five, 0, 255, 0, 1024)); };
void func6Default() {
	userSetDefault();
	print("HEY DEFAULT");
	delay(2000);
};



