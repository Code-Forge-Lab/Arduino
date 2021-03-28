bool isMenuActive; // save menu is active 
bool manualReapetEach1sec; // allow print each second


bool b1=false;

// the loop function runs over and over again until power down or reset
byte count_buttonUp = 0;
byte count_buttonDown = 0;
byte count_buttonDownUp = 255; // first time in booting, glitch push button?

byte state_btnPswrd = 0;
const  byte pswdLenght = 4;
const  byte	array_pswd[pswdLenght] = {2, 2, 1, 2};  //matching
bool pswMatching = false; // 0 = unfinished tipping a password, 1= correct
byte psw_correctScore = 0; // counts how much was matching points in user inputed password, compared as from array_pswd staticly saved one;
byte psw_incorrectScore = 0; // count how much was incorrectly inputed a password for later turn on short alarm
// byte array_inputedpswd[pswdLenght] = {0, 0, 0, 0} ; //  // buggy stuff, worked simple vars BELOW. Reason, cant add count_buttonDownUp, always give random result
     

byte arr0 = 0;
byte arr1 = 0;
byte arr2 = 0;
byte arr3 = 0;

String p1;
String p2;


void userSetDefault() {
	 var_one = 10;
	 var_two = 22;
	 var_three = 33;
	 var_four = 44;
	 var_five = 55;
	 //var_six = 66;
	 
	  alarm_totalTime = 60; // 1s*60 = 60s
	  alarm_strenghtWarning	= 50	;  // when password doesn't inputed correctly
	  alarm_strenghFullBlast	= 255	;  // when someone passed throw laser or password was incorrect to many times
	  alarm_delay = 10;
	
};




/*
void func1() { print("func1 " + String(var_one)); menu.menuPrintManuallyValue(map(var_one, 0, 255, 0, 1024)); };
void func2() { print("func2 " + String(var_two)); };
void func3() { print("func3 ignore! "); menu.menuPrintManuallyValue(333);/ *not works,good* / };
void func4() { print ("func4 "); }; // map(var_four, 0, 255, 0, 1024)
void func5() { print("func5 " + String(var_five)); menu.menuQuckAccesPrintManuallyValue(map(var_five, 0, 255, 0, 1024)); };
	*/ 

//strength



void func1() { print("how long alarm  \nwill work "); };
void func2() { print("gives strength turn\non alarm for 1 second"); };
void func3() { print("gives strength turn on\nalarm but\npowerfully "+ String (alarm_totalTime)+"sec");};
void func4() { print ("wait to turn on alarm \n in beginning from \nlaser"); }; // map(var_four, 0, 255, 0, 1024)
void func5() { print("func5 " + String(var_five)); menu.menuQuckAccesPrintManuallyValue(map(var_five, 0, 255, 0, 1024)); };
void func6() {print("time where be on light\n after laser passed");};

void func6Default() {
	userSetDefault();
	print("HEY DEFAULT");
	delay(2000);
};



String booleans (bool val) { if (val) return "true "; else return "false";     };

void printbool(bool statement) {
	menu.print__(statement);
}


void printeach_1sec(String text) {

	if ( manualReapetEach1sec)
		menu.print__(text);

};



void printeach_1sec(byte text) { // allow to print a only each second.

	if ( manualReapetEach1sec)
		menu.print__(text);

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


void set_inputedpswd (byte index, byte value) {
	
	
	switch (index) {
		
		case 0:
		arr0 = value;
		break;
		
		
		case 1:
		arr1=value;
		break;
		
		
		case 2:
		arr2=value;
		break;
		
		
		case 3:
		arr3=value;
		break;
		
	}
}

byte get_inputedpswd (byte index) {
	
	
	switch (index) {
		
		case 0:
		return arr0 ;
		break;
		
		
		case 1:
		return arr1;
		break;
		
		
		case 2:
		return arr2;
		break;
		
		
		case 3:
		return arr3;
		break;
		
	}
}


void clearInputArray (){
	for (byte i = 0; i < pswdLenght; i++) //clear user input array
	{
		set_inputedpswd(i,0);
		
	}
	
}

void countInputs () { // reset to zero if is full of try and errors from password inputs
	//Serial.println("+++add:count_buttonDownUp" + String(count_buttonDownUp));
	count_buttonDownUp = count_buttonDownUp + byte(1); // count each pressed buttons; //bugg reacted
	
	if (count_buttonDownUp >= pswdLenght)  { // detect when user pushed time to high
		
		clearInputArray ();
		count_buttonDownUp = 0;
		psw_correctScore = 0;
		
		Serial.println("===Clear:count_buttonDownUp\n");
	}
	
}


void clearRuntimeVars () {
		
		 clearInputArray ();
		 display.clear();
		 pswMatching = 0; // password is marching, reset after 10s
		 psw_correctScore = 0;
		 b1 = false;
		 count_buttonDownUp = 255;
		 Serial.println("Exit and clear vars");
	
	};

void collectPasswordRuntime () {
	
	 if ( b1 && millis() > (long)clock_btnPswTimeout  ) // reset to state zero 
	 {
		 clearRuntimeVars ();
	 }
	 
	 
	 
	 if (  ( btnUp.onlyReadPressedSignleTime() || btnDown.onlyReadPressedSignleTime()) ) //  // if button up or down then activate timer to work and while pressing, reset with millis()
	 {
		 clock_btnPswTimeout = millis() + 5000UL; // reset each  10 seconds  time
		 //Serial.println( millis() < (long)clock_btnPswTimeout );
		 countInputs ();
		 manualReapetEach1sec = true;
	 }

	 if ( 	 millis() < (long)clock_btnPswTimeout ) // timer when to work
	 
	 {
		 
		 if (btnUp.onlyReadPressedSignleTime())
		 {
			 //array_inputedpswd[count_buttonDownUp] = 2;
			 set_inputedpswd (count_buttonDownUp, 2);
			 Serial.println("[]btnUp:"+String(count_buttonDownUp) + "-->2");
			 
		 }
		 
		 if (btnDown.onlyReadPressedSignleTime())
		 {
			 //array_inputedpswd[count_buttonDownUp] = 1;
			 set_inputedpswd (count_buttonDownUp, 1);
			 Serial.println("[]btnDown:"+String(count_buttonDownUp)+ "-->1");
			 
		 }
		 
		 if (manualReapetEach1sec) // do timing 1s
		 {		for (byte i = 0; i < pswdLenght; i++) // prining out
			 {
				 p1 +=String (array_pswd[i])+",";
			 }
			 display.print ("Paswd:" + p1 +"\n");
			 
			 
			 for (byte i = 0; i < pswdLenght; i++)
			 {   p2 +=String (get_inputedpswd(i))+",";
				 
				  
				 if (count_buttonDownUp == (pswdLenght - 1)  // if is in full length to compare a password
						&& ( btnUp.onlyReadPressedSignleTime() || btnDown.onlyReadPressedSignleTime()) ){ //compare password if is that correct
					 
					 if (array_pswd[i] == get_inputedpswd(i)) // checks if if password and user inputed attempt matched
								psw_correctScore++;
							
							
						if (psw_correctScore == pswdLenght ) // if score correct, they should be the same as required combination of length of the password ~4
							{pswMatching = true;
							 psw_incorrectScore = 0; // reset incorrect attempts	
							} else {
								psw_incorrectScore++;
							}
							
				 }
				 
			 }
			 
						
				
			 display.print ("Input:" + p2 +"\n");
			 display.print ("pwd score: "+ String (psw_correctScore) + " of "+ String (pswdLenght) + " \n");
			 display.print ("pwd bad score: "+ String (psw_incorrectScore) +  " \n");
			 display.print ("pwd marched: " + booleans (pswMatching ) + " \n");
			 p1.clear();
			 p2.clear();
			 b1 = true;
		 }
		 // if (  ( btnUp.onlyReadPressedSignleTime() || btnDown.onlyReadPressedSignleTime()) )
		 
		 
	 }
	 
 }
 
 
	
	
