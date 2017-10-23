
class ShutdownTimeOut  {
	//CountingDown
	
	float TimeOutValue;
	
	public:
	//Tell if reached end point ==0
	float TimeOutReached;
	//Counting Down
	int TimeOutCount;
	
	ShutdownTimeOut (int TimeOutCount = 20)
	{
		this->TimeOutCount = TimeOutCount;
		TimeOutValue = TimeOutCount;
		TimeOutReached=false;
	}
	
	void CountDownUpdate() {
		 	if (TimeOutCount > 0){
		 		TimeOutCount--;
				TimeOutReached=false;
			 }
		 	
		 	if (TimeOutCount == 0 )
		 	{
				TimeOutReached =true;
		 	}
	}
		//Reset To Start Point 
		void Reset (){
			TimeOutCount = TimeOutValue;
		}
	};
	
	
	
	//Exsample
	/*
	 *if (lux.Temperature >90 ) {
		 digitalWrite (luxPin,HIGH);
		 lux_timeout.Reset (); // if success then continue doing positive things
		 }else {
		 lux_timeout.CountDownUpdate ();
		 
		 if (lux_timeout.TimeOutReached) //negative  point  reached ==0 sadly
		 digitalWrite (luxPin,LOW);
	 }
	 **/