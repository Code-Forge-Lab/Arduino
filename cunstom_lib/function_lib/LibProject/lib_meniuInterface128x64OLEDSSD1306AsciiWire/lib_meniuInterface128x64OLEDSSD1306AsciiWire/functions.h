bool isMenuActive; // save menu is active 
bool manualReapetEach1sec; // allow print each second

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





void printbool(bool statement) {
	menu.println__(statement);
}


void printeach_1sec(String text) {

	if ( manualReapetEach1sec)
		menu.println__(text);

};



void printeach_1sec(byte text) { // allow to print a only each second.

	if ( manualReapetEach1sec)
		menu.println__(text);

};



// show status print out.
/*
void printeach_1secWhenButtonSet(String text) { // if button right  is set then print this 

	if (!isMenuActive && manualReapetEach1sec && allowPrintWhenRightButton)
		print(text);
}

void printeach_1secWhenButtonNotSet(String text) { // if button right  is unset then not print this 

	if (!isMenuActive && manualReapetEach1sec && !allowPrintWhenRightButton)
		print(text);
}

*/