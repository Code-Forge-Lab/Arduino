 #include "Arduino.h"
 #include <Servo.h>
//Object from servo lib



class WindTurbineBreakServo {
//obj  
  Servo WindTurbineBreakServoOOP;
//vars
int deagree;
bool reached;
public: //Constructor 
  WindTurbineBreakServo(){
      deagree = 93; //83 ~
      reached = false;
  }
private:
  
public:
  void turn (int val = 0);
//Attach Servo to Pin in setup()
  void IncludeWindTurbineBreakServoPin (int pin);
  //ControlFunctions 
  void ServoBreakON ();
  void ServoBreakOFF ();

};

void WindTurbineBreakServo::IncludeWindTurbineBreakServoPin(int pin) {
     WindTurbineBreakServoOOP.attach(pin);
}

void WindTurbineBreakServo::turn (int val ){
   WindTurbineBreakServoOOP.write(val);
   delay(10*1000);  
 };

void WindTurbineBreakServo::ServoBreakON(){
    
    if (not reached) 
        {    
          for (int i = 0; i < deagree; ++i)
          {
            WindTurbineBreakServoOOP.write(i);
            delay (120);
          }
         
         // now back little bit 
         //  WindTurbineBreakServo::turn (deagree );
         //delay (3000);
          // Successfuly Debug Flickerink like HELL 
         for (int i = deagree; i > (deagree  - deagree / 5 ); --i)
         {
           WindTurbineBreakServoOOP.write(i);
           delay (170);
         }
       }
       reached = true;
}

void WindTurbineBreakServo::ServoBreakOFF(){
    
      if (reached)
        {  
           for (int i = deagree  - deagree / 5 ; i > 0; --i)
           {
             WindTurbineBreakServoOOP.write(i);
             delay (125);
           }
        } 
        reached = false; 

}

