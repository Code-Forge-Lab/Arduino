class buttons {
private:
bool *btn;
bool btnPressSingleTime;
bool btnPressedSingleTime;
bool btnPressedSingleTimeReadOnly;
byte *pinRead;
byte invertDigitalRead;
bool isclock_01sec ;
unsigned long clock_01sec;
unsigned long clock_waitUserPressOffBtn;
unsigned long clock_add;


int cnt = 0;
public:
	buttons (bool &BTN/*button variable that change at demand*/,byte &PinRead /*what kind of pin to read*/ , bool InvertDigitalRead = true/*Read input statement invertecly !*/,bool PressSingleTime = false /*Disable fast pulses and leave single press*/ ,unsigned long CLOCK_ADD = 0UL /*add time  away to slower the speed of pulses*/) {
	btn = &BTN;
	clock_add = CLOCK_ADD;
	pinRead = &PinRead;
	invertDigitalRead = InvertDigitalRead;
	btnPressSingleTime = !PressSingleTime;
	clock_01sec = 0;
   }

public:void scaning (){
	
	//unsigned long now;   
	//unsigned long then;


		if (digitalRead(*pinRead) == !invertDigitalRead) { // invertDigitalRead == HIGH in esp8266
				
					//	if (btnPressedSingleTime == false ) {
					//					//Serial.println(String(cnt) + "button pressed now");
					//			now = millis();
					//			cnt++;
		
					//	}
						
							 if (   millis() > ((long)clock_01sec + (long)(60UL + clock_add)) ) // pulse timer
							 {
								 
							
										  if (btnPressSingleTime && millis() > (long)(clock_waitUserPressOffBtn + 1200UL) ){						
														*btn = true;
														 btnPressedSingleTime = false;// reset single time press a button
														Serial.println(" Btn_Pulse ");

										  } else {

													if (!btnPressedSingleTime){ // press single time to avoid it
															*btn = true;	
															 btnPressedSingleTimeReadOnly = true;
														//	then = millis();

													//		Serial.print("Btn press " + String (then - now));
							
														}	
						
													   btnPressedSingleTime = true;
								
												 }
										
										
									clock_01sec = millis(); // reset each  100ml_seconds  time
								}
						//Serial.print("Btn");
					}else {
						 clock_waitUserPressOffBtn = millis();
						 btnPressedSingleTime = false;// reset single time press a button
					}
				

		
}

public: void endScaning () {

	*btn = false;
	 btnPressedSingleTimeReadOnly = false;
	}

// read one time whet button was pressed
public: bool onlyReadPressedSignleTime (){

 return btnPressedSingleTimeReadOnly;
};



};