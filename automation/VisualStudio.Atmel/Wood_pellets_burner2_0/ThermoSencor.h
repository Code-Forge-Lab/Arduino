// +5 > +TermoSensor- .._+(Microcontroler Read)-_.. > +R100k or same as termosensor- >+GND

class ThermoSencor {
	int ThermistorPin;
	int Vo;
	float  AverageTimesRezult = 0.0; // begin set is equal -1
	float  AverageTimesCount = -1.0;
	bool   AverageTimesGetAverage = false;
	float R1;
	float logR2, R2, T;
	const  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


public:
	float temperature = 0.0;

	ThermoSencor(uint8_t ThermistorPin = A0, float R1_Thermistor = 10000)
	{
		this->ThermistorPin = ThermistorPin;
		R1 = R1_Thermistor;
	}
	// Convert value get from sensor 
	float tempConvert(float convertionValue) {
		Vo = convertionValue;
		R2 = R1 * (1023.0 / (float)Vo - 1.0);
		logR2 = log(R2);
		T = (1.0 / (c1 + c2 * logR2 + c3 * logR2*logR2*logR2));
		return(T - 273.15); // celcius
	}


	float tempReadRaw() {

		return tempConvert(analogRead(ThermistorPin));

	}


	bool InCustomTimeAverageUpdate(int updateAverageTimes = 10) { // return success of completed cycle of average value

		AverageTimesRezult = analogRead(ThermistorPin) + AverageTimesRezult;



		if (AverageTimesCount >= updateAverageTimes) // average is counted then return new temperature value
		{
			AverageTimesCount = 0; // reset to beginning



			temperature = tempConvert((float)AverageTimesRezult / (float)updateAverageTimes);


			AverageTimesRezult = 0; // reset after give temperature ratio result
			AverageTimesGetAverage = true;
		};

		if (AverageTimesCount == -1.0) // first time when program is booted give sum result
		{
			// Temperature =  AverageTimesRezult / temperatureRatio   ;
			temperature = tempReadRaw();
		}

		AverageTimesCount++; /// first execute -1 statement 
		return AverageTimesGetAverage;
	};

};