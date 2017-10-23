#include "Arduino.h" 



class Anemometer 
{
public:  
	Anemometer () {
		FotoElectric_GoThrow=false;
		FotoElectric_Times=0;
		FotoElectric_AveragePerSelectedTime=0;
		FotoElectric_PassiveCounter=0;
    FotoElectric_ActiveCounter=0;
    FotoElectric_MinimumThreshold = 200;
	};
private: //private vars
bool FotoElectric_GoThrow;
int  FotoElectric_Times;
int  FotoElectric_PassiveCounter; // Count Until Get Selected Time if something happend
int  FotoElectric_ActiveCounter; 

public: // vars
 int  FotoElectric_AveragePerSelectedTime; // Give average per second
 int  FotoElectric_MinimumThreshold ; // Say What is minimu bit value of Input A0!
public: //methods	
// Keep Wathing Passing finger one second cycle at defoult
int FotoElectricAveragePerTime(int seconds = 1000 , int AnalogRead = A0) {
    int input = analogRead(AnalogRead);
    //int seconds = 5000; // 10sec
    //Reset always 
    
    if (input > FotoElectric_MinimumThreshold  &&  FotoElectric_GoThrow) { // Singlan strenght of 1023
        FotoElectric_GoThrow = false;
       // Serial.println (",");
       // Serial.print ("Wass true ---- 1111");
       
      
       if ( FotoElectric_Times == 1) { // each cycle does old revoliution conflict 
            //Serial.println ("HOLLO -");
          }
          FotoElectric_Times++;
      } else {
          // while not passed same finger then wait until happen then give bool to reset in true mode
          if ( input <FotoElectric_MinimumThreshold) // Singlan strenght of 1023
              FotoElectric_GoThrow = true;
         //Serial.println (",");
         //Serial.print ("Wass false ---- 0_0_0_0");      
      } 

      // if counting started then timer is triggerd // this  function counter
      if (FotoElectric_Times > 0){
          FotoElectric_PassiveCounter++;
        } 


      // if average limit was reached then calculate and return an rezult
      if (FotoElectric_PassiveCounter>= seconds) { // seconds var control distance
          FotoElectric_AveragePerSelectedTime = FotoElectric_Times;
           // Serial.println ("Times -");
           // Serial.println (FotoElectric_Times);

            FotoElectric_Times   =0;
            FotoElectric_PassiveCounter =0;
            FotoElectric_ActiveCounter = 0; // also counter then all ok
            
      }
      FotoElectric_ActiveCounter++;
      if (FotoElectric_ActiveCounter > seconds * 3)
        // reset damm one to zero 
      {
        FotoElectric_AveragePerSelectedTime = 0; 
        FotoElectric_ActiveCounter = 0; // also counter not ok
      }  

 // else if not average then return last average 
 return FotoElectric_AveragePerSelectedTime ;
 }
};