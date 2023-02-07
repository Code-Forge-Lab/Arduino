


class VoltMeter {
	private:
		uint8_t pin; // pin input of analogRead
		//Resistant of divider R1
		float R1; //  (+)....R1 ..Divider.. R2....(Ground)
		
		//Resistant of divider R2
		float R2;//  (+)....R1 ..Divider.. R2....(Ground) 
		float R_fix;
		byte fRb; 
		//Ignore Voltage of WHen if failing 
		float broken_voltage = 0.0;

		
		//// number of analog samples to take per reading
const static int   NUM_SAMPLES = 10;
		int sample_count;
		int sum;                    // sum of samples taken
		unsigned long clock_10msec = 0 ; 
	public:	
		float voltage;            // calculated voltage
		
		
	//Constructor  , Give Pin To Read in analogRead		// Use When Voltage is showing bad values in small range				
		VoltMeter (uint8_t pin_input = A0 ,              float broken_voltage = 0.0        ,  float R1 = 100000.0 , float R2 = 10000.0  ) { 
			pin = pin_input;
			sum = 0;
			sample_count = 0;
			voltage = 0.1;	 
			this->R1 = R1;
			this->R2 = R2;
			R_fix = R2;
			this->broken_voltage = broken_voltage;
		}
		

   public:void FixVltR (byte Rb) // works as potentiometer !
   {
   		R2 = R_fix * (Rb * 0.01);
   		fRb = Rb;
   }
   public: float getFixVltR () {return R2;};		
		// Always Call Then Use New Stored	 Values
		// 10mlSecond Delay
   public:float VoltageMeterUpdate (bool byCount = false) { 
			 // take a number of analog samples and add them up
			 if (!byCount && ((long)clock_10msec + 10UL) < millis())

             {
     			 clock_10msec= millis();

				 sum += analogRead(pin);
				 sample_count++;
				 
			 } else{
			 	//by count, placed to a external clock brach`

			 	 sum += analogRead(pin);
				 sample_count++;
			 }

			 if (sample_count >= NUM_SAMPLES) {
			 //Serial.println ("R_Fix R2: " + String (R2) +" fix value - " + String (fRb) + " , R1: " + String (R1));	
			 // calculate the voltage
			 // use 5.0 for a 5.0V ADC reference voltage
			 // 5.015V is the calibrated reference voltage
			 voltage = ((float)sum / (float)NUM_SAMPLES * 3.3) / 1024.0;
			 // send voltage for display on Serial Monitor
			 // voltage multiplied by 11 when using voltage divider that
			 // divides by 11. 11.132 is the calibrated voltage divide
			 // value
			
			
			
			 voltage =  voltage / (R2 / ( R1+R2 ) );
			 
			 //Limit Broken Voltage
				if (voltage >= broken_voltage)
						voltage=voltage /*- broken_voltage*/;
				else if  (voltage < broken_voltage)
						 voltage = 0.0;
						 	
			 //Serial.print("BROKEN: "+ String (voltage));
			// Serial.println (" V");
			 sample_count = 0;
			 sum = 0;
			}
			 return voltage;
		}
		
		
	};



	


//Connect resistors
//				   .__._A0 // Best Result
//				   |
//(+)....R1 ..^(Divider)^.. R2....(Ground) 	
	
/*	
	// How to use it 
	//Create Object
	VoltMeter Mune(A4,0.0,10000.0,5000.1);
	
	void loop {
		//Call function get voltage
		Mune.VoltageMeterUpdate ();
		//in voltage variable  are calculated volts
		Serial.println (String ("Volt")+Mune.voltage);
		};

*/	
