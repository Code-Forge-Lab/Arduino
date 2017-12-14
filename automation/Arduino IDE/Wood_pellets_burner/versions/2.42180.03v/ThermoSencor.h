// +5 > +TermoSensor- .._+(Microcontroler Read)-_.. > +R100k- >+GND

class ThermoSencor {
	int ThermistorPin;
	int Vo;
  float  AverageTimesRezult = 0.0; // begin set is equal -1
  float  AverageTimesCount =-1.0;
	float R1;
	float logR2, R2, T;
    const  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
	
	public:
	float Temperature = 0.0;
	
	 ThermoSencor (uint8_t ThermistorPin = A0, float R1_Thermistor = 10000 ) 
	 {
		this->ThermistorPin = ThermistorPin; 
		R1 = R1_Thermistor;
	 }
	  
	  //Accurante , Precise , Time Waisting themperature Test
	  void PreciseSencorUpdate () {
		 float sum ; 
		 int sum_count = 10;
		 for (int x = 0;x < sum_count; x++) {
		  Vo = analogRead(ThermistorPin);
		  R2 = R1 * (1023.0 / (float)Vo - 1.0);
		  logR2 = log(R2);
		  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
		  T = T - 273.15; //temperature
		  sum+=T;
		  delay (10);
		 }
		 Temperature = sum/sum_count;
	  }
	  
	  //No Time waisting, Inaccurate Fast Themperature Test 
	  void SingleImpreciseUpdate () {
				 Vo = analogRead(ThermistorPin);
				 R2 = R1 * (1023.0 / (float)Vo - 1.0);
				 logR2 = log(R2);
				 T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
				 T = T - 273.15;
				 Temperature = T;
		  };

       float RawValueUpdate () {
         Vo = analogRead(ThermistorPin);
         
         Temperature = Vo;

         return Temperature;
      };
                                              
      void InCustomTimeAverageUpdate (int updateAverageTimes = 10, float temperatureRatio = 25.7) { // 
         AverageTimesRezult = analogRead(ThermistorPin) + AverageTimesRezult;
         
         

         if (AverageTimesCount >= updateAverageTimes) // average is counted then return new temperature value
          {
            AverageTimesCount = 0; // reset to begining
            Temperature = (AverageTimesRezult / updateAverageTimes) / temperatureRatio;
            AverageTimesRezult = 0; // reset after give temerature ratio rezult
          };

         if ( AverageTimesCount == -1.0  ) // first time when program is booted give sum rezult
           {
            Temperature =  AverageTimesRezult / temperatureRatio   ;
           }

         AverageTimesCount++; /// first execute -1 statement 
      };
      
	};
