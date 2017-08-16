/* DRY /= Dont Reapet Yourself
///////Nokia 5110
Pins
1:Reset 
2:CE    - Chip enable           //Digital
3:DC    - Data command "choise" //Digital
4:DIN   - Data in               //Digital  
5:CLK   - clock                 //Digital
6:VCC   -                       //Power
7:LIGHT -                       //GRD
8:GRD	-                       //GRD
///////////////
1:RST (RESET) -> Arduino       pin 3
2:CE or CS -> Arduino          pin 4
3:DC -> Arduino                pin 5
4:DIN (MOSI) -> Arduino        pin 6
5:CLK (SCLK) -> Arduino        pin 7
6:VCC -> Arduino               pin 3.3V
7:LIGHT -> Arduino GND (I am going to be using it on)
8:GND -> Arduino GND

7.12 percent).
Estimated memory use: 511 bytes (of a 2048 byte maximum, 24.95 percent)

*/
//includes
#include "ASCII.h"
//macros
#define RST 3
#define CE 4
#define DC 5
#define DIN 6
#define CLK 7
//
#define RowHeigh 6
#define CollumnWidth 84
//Functions
void MovePixel();
void LcdWriteData (byte cmd);
void LcdWriteCmd (byte cmd);
void LcdWriteChar (char character);
void ClearScreen();
// INPORTANT !!
	void SETUPLCD(){ // Place this function in void setup() {  SETUPLCD(); }
	pinMode(RST,OUTPUT);
	pinMode(CE,OUTPUT);
	pinMode(DC,OUTPUT);
	pinMode(DIN,OUTPUT);
	pinMode(CLK,OUTPUT);
	digitalWrite(RST,LOW); // essential part of nokia display to reset with digital hight low signals 
	digitalWrite(RST,HIGH);
	
	LcdWriteCmd(0x21); // LCD Extend command
	LcdWriteCmd(0xBE); // set LCD Vop (Contrast) 1:B8 , 2:BF // Background BlackCollorStrenght
	LcdWriteCmd(0x04); // set temp coefficent
	LcdWriteCmd(0x14); // LCD bias mode 1:40  1:0x14 , 2:0x10 // Text Opacity 
	LcdWriteCmd(0x20); // LCD set basic command
	LcdWriteCmd(0x0C);//Vertical Stripes  //LcdWriteCmd(0x09); // LCD all segments  is on 
    //Data 
   // LcdWriteData (0x55); LcdWriteData (0xAA); Write Verticly  // Sending HEX 55
 	ClearScreen();
}
//Set Position of LCD
void LcdXY(int x , int y) {
	LcdWriteCmd(0x80 | x);
	LcdWriteCmd(0x40 | y);
}

// Write Text
void LcdWriteString (char *character){
	while(*character) LcdWriteChar(*character++);
} // LcdWriteString ("HELLO");

void LcdWriteStringInt (char * character,int number, char *character2 = "") { // String with int value 
	char string[5];
	LcdWriteString ( character);
//							var  ,StrRep()      Flot/Int              
    LcdWriteString(dtostrf(number    ,0   ,0       ,   string)); //from stdlib
    LcdWriteString (character2);
}//LcdWriteStringFloat ("Voltage: ",12); or LcdWriteStringFloat ("Voltage: ",12 , "%");

void LcdWriteStringFloat (char *character,float number, char *character2 = "") { // String with float value
	char string[5];
	LcdWriteString (character);
//							var  ,StrRep()      Flot/Int              
    LcdWriteString(dtostrf(number    ,0   ,2       ,   string)); //from stdlib
    LcdWriteString (character2);
} // LcdWriteStringFloat ("Voltage: ",55.12); or LcdWriteStringFloat ("Voltage: ",55.12 ,"%");


 void LcdWriteChar (char character){
    for ( int i = 0; i < 5; i++) 
    	LcdWriteData(ASCII[character - 0x20][i]); // Write 5 pixel psace character
        LcdWriteData(0x00); // Adding space 
   }; // cdWriteChar (0x41);

\

void LcdWriteData (byte cmd) 
{
   digitalWrite (DC, HIGH); // DC pin is low for commands
   digitalWrite (CE, LOW);
   shiftOut(DIN,CLK,MSBFIRST,cmd);  // transmit serial data // MSBFIRST = Most Significant Bits First
   digitalWrite(CE,HIGH);
}

void LcdWriteCmd (byte cmd) 
{
   digitalWrite (DC, LOW); // DC pin is low for commands
   digitalWrite (CE, LOW);
   shiftOut(DIN,CLK,MSBFIRST,cmd); 
   digitalWrite(CE,HIGH);
}

void ClearScreen(){
	LcdXY(0 , 0);
	for (int i = 0; i < 504; ++i) //84*6 =504
	{
		LcdWriteChar (0x20);
	}
}


// // Some Images 
// void Square (){

// 	for (int x = 0; x < 8; ++x)
// 	{
// 		LcdWriteData (0xFE); 
// 	}
// }

 void AllRowsSelect (){

	for (int x = 0; x < 600; ++x)
 	{
 		LcdWriteData (0xFE); 
 		//delay(10);
 	}
 }