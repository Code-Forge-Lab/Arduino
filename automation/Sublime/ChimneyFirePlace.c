//

const int ButtonEngineTimer = A1 ;
const int ButtonEngineSpeed = A0 ;
const int LightSensorFireplace = A4;
//
const int PinTimer [4] = {6, 2, 3, 4};
const int PinSpeedLED = 5;
const int PinChimneyMotor = 10;
const int PinMotorSignalLED = 8;



//Dinamic variables
//Engine Chimny Turbine
float enginespeed = 0; //off 255
float enginespeedproc = 0; // off 100%
//
//Engine Chimny Turbine Speed
float enginetimer = 0; //off 7
float max_enginetimer =15;
float enginetimerproc = 0; // off 100%
//
int LightSensorFireplaceMax = 500;
// 
bool IsFireDroping = false;

//functions
void EngineSpeed ();
void EngineTimer ();
float ProcToValue (int procent , int baseValue ); //baseValue like 255 size
float BinAvailable(float inDecimal);
int DecToBinArr (int decimal , int *GetBinArr );
void UpdateTimerLED ();
int LightSensorFireplaceProcent();
void TimerLEDFullBlink ();


void setup() {
  // Display LED Output
  pinMode (PinTimer[0], OUTPUT);
  pinMode (PinTimer[1], OUTPUT);
  pinMode (PinTimer[2], OUTPUT);
  pinMode (PinSpeedLED, OUTPUT);
  //Analog Read
  //pinMode (ButtonEngineSpeed,INPUT);
  //pinMode (ButtonEngineTimer,INPUT);
  //SetUp Serial
  Serial.begin (9600);
  while (!Serial) {};// wait for serial port to connect. Needed for native USB port only

}

int counter = 0;
void loop() { //--------------------------------------------------

int btnspeed = digitalRead(ButtonEngineSpeed);
int btntimer = digitalRead(ButtonEngineTimer);
int eng_speed = 0;
 
 //0.reset 
    digitalWrite(PinMotorSignalLED,LOW);
  if (enginetimer ==0)
    {
    analogWrite (PinSpeedLED,0);  
    enginespeedproc = 0; 
    }
    
 //1.Give values  
if( enginetimer == 0) // enginetimer > 0 ||
{
  
  eng_speed =  0; //enginespeed;
  enginespeed = 0;
   
    
} 
else if (enginetimer >0 ) {
  EngineSpeed ();
  eng_speed =  enginespeed; //enginespeed;
  
  }
 if (btnspeed > 0 &&  enginetimer == 0) {
  
  digitalWrite(PinSpeedLED,HIGH);
  delay(300);
  digitalWrite(PinSpeedLED,LOW);
   delay(300);
  digitalWrite(PinSpeedLED,HIGH);
  delay(300);
  digitalWrite(PinSpeedLED,LOW);
  } 
//4 LED  Display 
  EngineTimer ();


  
//Sencors
  if (enginetimer == 0) {   // if timer doest interupt then use sencor control  
      int lightsencor =  LightSensorFireplaceProcent();// analogRead(LightSensorFireplace);
       //Serial.print ( "Light Level ");
       //Serial.print ( lightsencor);
       //Serial.println ( "");

          if (lightsencor <= 50) // Fire is start burning again
                 {
                    IsFireDroping = false;                    
                 } 
                 
                
                 
       
        if (lightsencor >= 50 && !IsFireDroping) // Light Level More then 50% And Fire Burning 
        {
          
          if (lightsencor == 100)
              IsFireDroping = true;
        
         if (lightsencor >=98)  //Speed Leave one procent always
            {eng_speed =  ProcToValue ((1.5) ,  (float)255 );
              Serial.print ( "Speed Limit Reached  -- " );
            }else
          eng_speed =  ProcToValue ((float)(100 - lightsencor) ,  (float)255 );
          
          Serial.print ( " MOTOR SPEED ");
          Serial.print ( eng_speed );
          Serial.print ( " - " );
          Serial.print ( 100 -  lightsencor );
          Serial.print ( "% [light: " );
          Serial.print (   lightsencor );
          Serial.println ( "%] " );
          digitalWrite(PinMotorSignalLED,HIGH);
           
          }else { // Fire Not Burning Or Fire Droping 
            eng_speed = 0;
           

                  Serial.print ( " ------------------------MOTOR WAIT 65%  ");                  
                      Serial.print ( "% [light: " );
                      Serial.print (   lightsencor );
                      Serial.println ( "%] " );
            }
            //delay(50);
    }
   

 
     
//2.Use Motors    
  analogWrite(PinChimneyMotor,eng_speed);

//3.Control Time Loop  
  delay(100);
  //10*100 = 1s[10],10*60 = 1min[600],10*600 = 10min[6000] 
  counter++; // Ten minutes = 10*60*10
  if (counter > 6000) {
    counter = 0;
    
     if (enginetimer >0){
        TimerLEDFullBlink ();
        enginetimer--;
        
     }
      if (enginetimer ==0) {
        eng_speed = 0;
        enginespeedproc = 0;
        }
        UpdateTimerLED ();
    }

   
}



void EngineSpeed () {
  int sensorValue =  digitalRead(ButtonEngineSpeed);
  if ( sensorValue > 0  ) {


    //Update
    enginespeedproc = enginespeedproc + 10.0f;


    if (enginespeedproc > 100)
      enginespeedproc = 0.0f;
      
    enginespeed = ProcToValue (enginespeedproc , 255.0f );
    //Wtite
    analogWrite (PinSpeedLED, 0);
    delay(130);
    analogWrite (PinSpeedLED, enginespeed);

    Serial.print ( "Engine Speed  ");
    Serial.print (enginespeed);
    Serial.print (" in ");
    Serial.print (enginespeedproc);
    Serial.print ("%");;
    Serial.println (".");

    while (digitalRead(ButtonEngineSpeed)) {
      delay(10);
    };
   // delay(400);
  }
}

void EngineTimer () {
  int sensorValue = digitalRead(ButtonEngineTimer);
  if ( sensorValue > 0  ) {

    float __size = sizeof(PinTimer) / sizeof(PinTimer[0]);
    float MaxPossibilities = BinAvailable( __size); // find out how much possible values have in binary
    enginetimer += 1.0f ;

    if (enginetimer > max_enginetimer)
      enginetimer = 0.0f;

    enginetimerproc = MaxPossibilities;

    //Display LED 
    UpdateTimerLED ();

    Serial.print ( "Engine Timer ");
    Serial.print (enginetimer);
    Serial.print (" in ");
    Serial.print (enginetimerproc);
    Serial.print ("%");;
    Serial.println (".");

    //while (digitalRead(ButtonEngineTimer)) {
    //  delay(10);
    //};

    delay(600);
  }
}

float ProcToValue (float procent , float baseValue ) {
  float a = (baseValue / 100) * procent;
  return a;
}

float BinAvailable(float inDecimal) {
  float base = 2;
  float CurrentBin = 1;
  for (float i = 0 ; i < inDecimal; i++) {
    CurrentBin = CurrentBin * base ;
    // Serial.print (CurrentBin);
    //Serial.print ("  :  ");

  }
  return (CurrentBin - 1 );
}

void UpdateTimerLED () {
  int binaryLED[9] ;
  //get lenght obinary array
  int __binsize =  DecToBinArr (enginetimer, binaryLED);//-------------------------------------------------------------------------------
;
int ledSteps = sizeof(PinTimer)/sizeof(PinTimer[0])-1; // array size of led
 for (int i = 0 ; i < __binsize;i++) {
   
         
       if (i >=__binsize-ledSteps-1 )       {
            if (binaryLED[i] == 1) 
            {           
              analogWrite(PinTimer[ledSteps],200);// fix dim light
              digitalWrite(PinTimer[ledSteps],HIGH);// fix dim light
            }else
            {
              analogWrite(PinTimer[ledSteps],0);
            }

            // increment stepper for light 
             /*
             Serial.print(">>>>>> ");
              Serial.print (ledSteps);;
               Serial.print(">>>>>> LED:: ");
                Serial.println(PinTimer[ledSteps]);
                */
            ledSteps--;
       }
       
    }
    
  for (int i = 0 ; i < __binsize;i++) {
         
              Serial.print (binaryLED[i]);;
               Serial.print("_");             
    }
     Serial.println("..."); 
  }

int LightSensorFireplaceProcent() {
   int lightsencor = analogRead(LightSensorFireplace);
       lightsencor = ((LightSensorFireplaceMax - lightsencor) /(float)LightSensorFireplaceMax)*100;
       if (lightsencor < 0)
          lightsencor = 0;
          
       return lightsencor;
  }

void TimerLEDFullBlink () {
  int __size = sizeof(PinTimer) / sizeof(PinTimer[0]);
   for (int i = 0; i < __size; ++i) {
    analogWrite(PinTimer[i],255);
    }
    delay(200);
     for (int i = 0; i < __size; ++i) {
    analogWrite(PinTimer[i],0);
    }
  }


//MATH
//Return binary array size
int DecToBinArr (int decimal , int *GetBinArr ) {
  int table[9] = {256,128,64,32,16,8,4,2,1}; // Posible values
  int bin[9] =   {0, 0, 0, 0 ,0, 0, 0 , 0 ,0};
 
  int __size = sizeof(table) / sizeof(table[0]) ;
  int __last_digit = decimal;

  // Find Out Where To Start  like from 16 
int MaxBasePos;
 for (int i = 0 ; i < __size; ++i )  {
      /*
       Serial.print ("GET ");
       Serial.print (table[__size-i-1]);
       Serial.print ( " >= ");
       Serial.print (__last_digit);
        Serial.println ( " ");
        */
       if (table[__size-i -1]  >=  __last_digit ) {
       MaxBasePos = __size- i -1; 
        /*
        Serial.print ( "--------MaxBasePos--------");
        Serial.println (MaxBasePos);
        */
        break;
  
        }
    }
//Get Values
 for (int i = MaxBasePos ; i < __size; ++i ) {
      /*
       Serial.println ("  ");
       Serial.print (i);
       Serial.print (".SET ");
       Serial.print (table[  i ] );
       Serial.print ( " - ");
       Serial.print (__last_digit);
       Serial.print ( " :: MaxBasePos ="); 
       Serial.print (  MaxBasePos );
       Serial.print ( " :: cBin =");
    */ 
        
    if ( __last_digit >= table[  i ]) {
      __last_digit = __last_digit -  table[ i ] ;
      bin[i+1] = 1;
     // Serial.print ( "----------------");
      }else {
        bin[i+1] = 0;
      }
      
     
      
     
  }
  
 //Serial.println ("");;
 for (int i = 0 ; i < __size+1;i++) {
    //Points Array From Each
     //  Serial.print (bin[i]);
    //   Serial.print ("_");
      GetBinArr[i] = bin[i];
         
  }
  //Serial.println ("");;
  
   
  
      return __size +1;
}