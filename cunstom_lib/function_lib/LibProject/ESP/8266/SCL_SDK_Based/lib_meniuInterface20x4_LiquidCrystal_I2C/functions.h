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




void func1() { print("func1 " ); menu.menuPrintManuallyValue(map(var_one, 0, 255, 0, 1024)); };
void func2() { print("func2 " ); };
void func3() { print("func3 ignr! "); };
void func4() { print ("func4 ",1,7);print ("All about balance ",2,0); }; // map(var_four, 0, 255, 0, 1024)
void func5() { print("func5 ",1,7); menu.menuQuckAccesPrintManuallyValue(map(var_five, 0, 255, 0, 1024)); };








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