
	
void myF () {}; // dummy function 
  void printMenuFunc (String text , controls* EEPROM  ,String n="", void (*functionPointer)() = myF  , bool testmode = false , String valueGreaterOrLessCondition = "Less" ,int valueGreaterOrLess=-1) {
	  
	  int8_t __set; 
	  int8_t __up;
	  int8_t __down;
	  int  testmode_value; 
	  String wrongStatment =  "";
//  lcd.noBlink();      
		  if ( testmode ) // not save value in EEPROM that was changed here 
			  testmode_value = EEPROM->getValue() ; 
			   
  delay(500);
  while ( true ) 
  {
			
		  

		  
				  
		  if (   __up  ) { // Boundry value not exeed
				 
				 if ( ( valueGreaterOrLess == -1 || valueGreaterOrLessCondition == "Greater"))
					  EEPROM->addValue();
				 else if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() < valueGreaterOrLess) {
					  EEPROM->addValue();

					  
				  } else  if ( EEPROM->getValue() > valueGreaterOrLess )   // if value to hight when valuegreaterorLess then equlize to minimum 
						   {   EEPROM->setValue (valueGreaterOrLess );   wrongStatment="";}
							 
				  
					 //or
					if (valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() < valueGreaterOrLess && valueGreaterOrLess !=-1)
					   { EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}   

					 if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1)
					{ EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}           
			 }
		 
	
			 if (  __down  ) { // Boundry value not exeed
				if ( ( valueGreaterOrLess == -1 || valueGreaterOrLessCondition == "Less"))
					  EEPROM->subValue();
				 else if (valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() > valueGreaterOrLess) {
					  EEPROM->subValue();
				
				  }
				 else  if ( EEPROM->getValue() > valueGreaterOrLess )   // if value to hight when valuegreaterorLess then equlize to minimum 
						  {   EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";   }; 
					   

				if (valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1)
					{ EEPROM->setValue (valueGreaterOrLess ); wrongStatment="";}
					//or
					 
			 }

							  
							 
				display.clearDisplay();
				display.print (text);
				display.setCursor(0,1);
//                display.dim(true);
				display.print (n+EEPROM->getValue()+ String (wrongStatment) );
				
		  functionPointer();
//     delay (100); // SLEEP COMMAND         
	 if (__set >=1) {
		 if (( valueGreaterOrLessCondition == "Less" && EEPROM->getValue() > valueGreaterOrLess && valueGreaterOrLess !=-1   ) ||
			   valueGreaterOrLessCondition == "Greater" && EEPROM->getValue() < valueGreaterOrLess && valueGreaterOrLess !=-1)
				{
				   wrongStatment = "?";
				}
				else  break; //end loop life then set button is pressed            
	 }
  } /////// END
		
		if ( testmode ) { // not save value in EEPROM that was changed here 
			 EEPROM->setValue(testmode_value  );  // return original state
			 functionPointer();
		} 
   /////////////////////////////////////////  
			
		EEPROM->setValue ();
   //     lcd.noBlink();
		
	 //   pinMode (13,OUTPUT);
	 //   delay(30);
	 //   digitalWrite(13,HIGH);
	 //   delay(30);
	 //   digitalWrite(13,LOW);
	  //  pinMode (13,INPUT);
	}



bool ArgueAgree (String text = "Ar tikrai taip?"  ) {
	  
	  int8_t __set; 
	  int8_t __up;
	  int8_t __down;
	  bool  condition = false;

			   
  delay(500);
  while ( true ) 
  {
	   
		
			  
		
			
		  // if no rotory encoder but only push buttons
		   __up = digitalRead(BUTTON_UP);
		   __down = digitalRead(BUTTON_DOWN); 
		   delay (100);         
		  

			__set = digitalRead(BUTTON_SET) ;     
		  
		
		  if (   __up  && !condition   ) 
					condition = true;
			 
		 
	
		  if (  __down && condition ) 
					condition = false;
			 

		  if ( __set )                            
				return condition; // return desided condition
				
				display.clearDisplay();
				display.print (text);
				display.setCursor(0,1);
			  
			  if (condition) // if true condition
				  display.print ("taip");
			  else // not true codition
				  display.print ("ne");
	
		  
   
	}
}

void sey (String lineA = "Issaugoti nauji", String lineB = "Nustatimai",int timeLastHold=3400) {
	display.clearDisplay();
	display.setCursor(0,0);
	display.print (lineA);
   delay (500);
   display.setCursor(0,1);
   display.print (lineB);
   delay(timeLastHold);
  }



/*
  byte  findLiquidCrystal_I2C () {
 
 
 
  byte error, address;
  int nDevices;
 
//  Serial.println("Scanning...");/
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.
	Wire.beginTransmission(address);
	error = Wire.endTransmission();
	
	if (error == 0)
	{
//      Serial.print("I2C device found at address 0x");/
	  if (address<16)
//        Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
 
	  nDevices++;
	}
	
  }

  
//  if (nDevices == 0)
//    return false; //    Serial.println("No I2C devices found\n");/
//  else
//    return true; //    Serial.println("done\n");/
 
  return  nDevices;
}
 */ 

/**
	void f () {
	  
 
 
  byte error, address;
  int nDevices;
 
//  Serial.println("Scanning...");/
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.
	Wire.beginTransmission(address);
	error = Wire.endTransmission();
 
	if (error == 0)
	{
//      Serial.print("I2C device found at address 0x");/
	  if (address<16)
//        Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
 
	  nDevices++;
	}
	else if (error==4)
	{
//      Serial.print("Unknown error at address 0x");/
	  if (address<16)
//        Serial.print("0");/
//      Serial.println(address,/HEX);
	}    
  };**/
