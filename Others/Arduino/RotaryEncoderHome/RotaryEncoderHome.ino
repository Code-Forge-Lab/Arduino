byte outputA = 5;
byte outputB = 6;
byte outputSET = 4;


void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
pinMode(outputA,INPUT);
pinMode(outputB,INPUT);
pinMode(outputSET,INPUT);
}

bool A = 0;
bool B = 0;
bool SET = 0;

bool A_State = 0;
bool B_State = 0;
bool A_Selective = 0;
bool B_Selective = 0;
bool R_State = 0;

int count = 0;
void loop() {
  // put your main code here, to run repeatedly:

A =! digitalRead (outputA);
B =! digitalRead (outputA);

SET =! digitalRead (outputSET);



if (SET) {
  Serial.print ("SET--");
  Serial.println (
   "State A" + String (A_State) + 
   ",B" + String (B_State) + 
   ",R" + String (R_State) +
   ",Selective A" + String (A_Selective) +
   ",Selective B" + String (B_Selective) 
   );
  
  }

if ((A || B) && !(A_Selective || B_Selective) )
    if (A ){
      A_Selective = true;
      }
    else {
      B_Selective = true;
      }

if (A && !A_State) {

  Serial.println (" A " + String (A));
  A_State = 1;
  R_State = 0;
  
}

 if (B && !B_State) {

  Serial.println (" B " + String (B));
  B_State = 1;
  R_State = 0;
  
}




if (A == 0 && B == 0 && !R_State){
 Serial.println (" Reset " + String (count) );
 R_State = 1;
 A_State = 0;
 B_State = 0;
 

Serial.println (",Selective A" + String (A_Selective) +
   ",Selective B" + String (B_Selective) 
   );
 
 if (B_Selective) 
 count--;
 if (A_Selective)
 count++;

 A_Selective = false;
 B_Selective = false;
}
 
}
