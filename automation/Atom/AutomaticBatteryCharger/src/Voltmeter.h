

class VoltMeter {
	private:
		uint8_t pin; // pin input of analogRead
		//Resistant of divider R1
		float R1; //  (+)....R1 ..Divider.. R2....(Ground) 
		//Resistant of divider R2
		float R2;//  (+)....R1 ..Divider.. R2....(Ground) 
		//Ignore Voltage of WHen if failing 
		float broken_voltage = 0.0;
		
		//// number of analog samples to take per reading
const static int   NUM_SAMPLES = 10;
		int sample_count;
		int sum;                    // sum of samples taken
	public:	
		float voltage;            // calculated voltage
		
		
	//Constructor  , Give Pin To Read in analogRead		// Use When Voltage is showing bad values in small range				
		VoltMeter (uint8_t pin_input = A0 ,              float broken_voltage = 0.0        ,  float R1 = 100000.0 , float R2 = 10000.0  ) { 
			pin = pin_input;
			sum = 0;
			sample_count = 0;
			voltage = 0.0;	 
			this->R1 = R1;
			this->R2 = R2;
			this->broken_voltage = broken_voltage;
		}
		
		
		
		// Always Call Then Use New Stored	 Values
		// 100mlSecond Delay
		float VoltageMeterUpdate () { 
			 // take a number of analog samples and add them up
			 while (sample_count < NUM_SAMPLES) {
				 sum += analogRead(pin);
				 sample_count++;
				 delay(10);
			 }
			 // calculate the voltage
			 // use 5.0 for a 5.0V ADC reference voltage
			 // 5.015V is the calibrated reference voltage
			 voltage = ((float)sum / (float)NUM_SAMPLES * 5.15) / 1024.0;
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
	VoltMeter Mune(A4,10000.0,5000.1);
	
	void loop {
		//Call function get voltage
		Mune.VoltageMeterUpdate ();
		//in voltage variable  are calculated volts
		Serial.println (String ("Volt")+Mune.voltage);
		};

*/	
