#include "Files\Library\frequency.h"
//obj-c
Average avr;

  


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print ("Serial begin ...");
 pinMode(A0, INPUT);

 
}

int c = 0;
int c_max = 600;

void loop() {
int total = avr.FotoElectricAveragePerTime(1000);

 // // Program per second 

  if(c >= c_max ) {
       Serial.print ("Average,");
       Serial.println (total); 
   
   }


 c++;
 if (c > c_max)
 c = 0;
 delay(1);
}

