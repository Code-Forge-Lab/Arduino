#include "nokia5110.h"
#include "myFunk.h"
//#include "E:\MyFolder\Script\Arduino\Projects\Library\myTest.h"
//#include "E:\MyFolder\Script\Arduino\Projects\Library\procent.h"
//Output
short int PIN_ControlRelayPumpPower = 13;
short int PIN_ControlWaterValvePower = 12;
short int PIN_EnableCurrent = 9; // 12 , 11 , 10 , 
short int PIN_EnableCurrentLED = 10;
//Input
short int PINRead_WellLevelSensor = A0;
//InputPOTS 
short int PINRead_HighestPointLevel_POT = A3 ;
short int PINRead_LowestPointLevel_POT = A4; 

// the setup routine runs once when you press reset:
   bool ReachedLowestPoint = false; // No Change Here
   float  LowestPointLevel = 150.0;
   float HighestPointLevel = 480.0;
   float MazaiVandensRatio = 1.04 ;  // Naudojamas nuspresti iki HighestPointLevel Spraga
   
   
void setup() {                
  // initialize the digital pin as an output.
  pinMode(PIN_ControlRelayPumpPower, OUTPUT);
  pinMode(PIN_ControlWaterValvePower, OUTPUT);   
  //Inpput
  pinMode(PINRead_WellLevelSensor, INPUT);
  //Inpput POTS
  pinMode(PINRead_HighestPointLevel_POT, INPUT);
  pinMode(PINRead_LowestPointLevel_POT, INPUT);

  SETUPLCD();
  //AllRowsSelect ();
   Serial.begin (9600);   
}




// the loop routine runs over and over again forever:
void loop() {
     //CheckCurrentEnableTransistorPassCurrentThrowWater
  
     digitalWrite(PIN_EnableCurrent, HIGH);
     analogWrite(PIN_EnableCurrentLED, 1);
     //ReadContent
     delay(10); //delayCheckWaterLevelStrart
     float WellLevelInput = analogRead(PINRead_WellLevelSensor);
     float PotHighestPointLevel = analogRead(PINRead_HighestPointLevel_POT);
     float PotLowestPointLevel  = analogRead(PINRead_LowestPointLevel_POT);
     HighestPointLevel = PotHighestPointLevel;
     LowestPointLevel = PotLowestPointLevel;
    //Make decision about highestpointlevel shoud not be lower then  lowestpointlevel
    
     delay(10); //delayCheckWaterLevelEnd
     digitalWrite(PIN_EnableCurrent, LOW);
     analogWrite(PIN_EnableCurrentLED, 0);


    
       
        //If WaterLEvel is Low
          if (WellLevelInput <=  LowestPointLevel &&  !ReachedLowestPoint ) {
                
                if ( PotHighestPointLevel + 10 > PotLowestPointLevel) //prottect for logical conflict
                {
                digitalWrite(PIN_ControlRelayPumpPower, HIGH);
                digitalWrite(PIN_ControlWaterValvePower, HIGH);
                };
                
                ReachedLowestPoint = true;
                //JustSayBool
                

             };
             //When WaterLEvel is Hight Then 
             if (WellLevelInput >=  HighestPointLevel &&  ReachedLowestPoint) {
                   if ( PotHighestPointLevel + 10 > PotLowestPointLevel) //prottect for logical conflict
                   {
                    digitalWrite(PIN_ControlRelayPumpPower, LOW);
                    digitalWrite(PIN_ControlWaterValvePower, LOW);
                   };
                    ReachedLowestPoint = false;
                   LcdXY(0,2); 
             }
          

       
         
         //Print on lcd start
         LcdXY(0,0);
         //Show better then floating numbers
         
         if  (WellLevelInput < LowestPointLevel){
            LcdWriteString ("                     "); LcdXY(0,0);LcdWriteStringInt ("Vanduo:",0); //Vandens not always zero but try to ignore it
         }
         else if (PotHighestPointLevel < PotLowestPointLevel) {
            LcdWriteString ("                     "); LcdXY(0,0);LcdWriteStringInt ("Vanduo:",WellLevelInput); //Vandens not always zero but try to ignore it
         }
         else{     
            LcdWriteString ("                     "); LcdXY(0,0);LcdWriteStringInt ("Vanduo:",WellLevelInput); //Show if minimum bound are reached
          }
          LcdXY(0,3);
         if (ReachedLowestPoint )
            LcdWriteString ("Siurbs: Ne     ");
         else if (PotHighestPointLevel < PotLowestPointLevel)
            LcdWriteString ("Siurbs: Klaida!");  
         else   
            LcdWriteString ("Siurbs: Taip  ");  
         LcdXY(0,4);

         ////////////////////////////////////////////////////////////////////////////////////////////////////
         if (PotHighestPointLevel+10 <= PotLowestPointLevel) LcdWriteString ("'H' per mazas!"); else LcdWriteString ("               ");// clear

         LcdXY(0,5);
         LcdWriteStringInt ("H:",PotHighestPointLevel);
         LcdWriteStringInt (",L:",PotLowestPointLevel);LcdWriteString ("   ");
        /////////////////////////////////////////////////////////////////////////////////////////////////////     
             //Say about Current State
              LcdXY(1,2); 
          
           if(PotHighestPointLevel < PotLowestPointLevel && PotHighestPointLevel == 0)
                LcdWriteString ("Vandens:Klaida");  
          else if ( (WellLevelInput >=  HighestPointLevel || HighestPointLevel / MazaiVandensRatio < WellLevelInput)  )
              LcdWriteString ("Vandens:Dauk  ");
          else if (WellLevelInput <  LowestPointLevel || PotHighestPointLevel < PotLowestPointLevel ) //&& ! WellLevelInput > LowestPointLevel 
              LcdWriteString ("Vandens:Nera ");   
          else if (HighestPointLevel / MazaiVandensRatio > WellLevelInput ) // jei daugiau nei puse //320 Lowest
              LcdWriteString ("Vandens:Mazai "); 
                 
              

         //Print on lcd end

        

         delay(800);

       

}







