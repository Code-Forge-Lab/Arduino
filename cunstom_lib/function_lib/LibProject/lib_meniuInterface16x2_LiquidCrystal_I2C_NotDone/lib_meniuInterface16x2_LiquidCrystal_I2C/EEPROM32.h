#pragma once

#include <EEPROM.h>



// write to memory ones 
bool writeMemory(int16_t address, byte value) {

	if (EEPROM.read(address) != value) {
		EEPROM.write(address, value);
		//print("Wrote!");
		//delay(200);
		return true;
	}

	return false;
}

// write to memory ones 
bool writeMemory(int16_t address, bool value) {

	if (EEPROM.read(address) != value) {
		EEPROM.write(address, value);
		//print("Wrote!");
		//delay(200);
		return true;
	}

	return false;
}


// write to memory ones 
bool readMemoryBool(int16_t address) {


	return EEPROM.read(address);
};

byte byteReadMemory = 0; // reduce reading noise

// write to memory ones 
byte readMemoryByte(int16_t address) {

	return EEPROM.read(address);
};








// write to memory ones 
void readMemoryBool(int16_t address, bool* value) {

	*value = EEPROM.read(address);
};


// write to memory ones 
void readMemoryByte(int16_t address, byte * value) {

	*value = EEPROM.read(address);
};





/*  // to much memory usage

// if values in positive side
void writeEEPROM32(int_fast16_t address, unsigned long int value) {


	byte bitSum;

	for (int_fast16_t xByte = 4; xByte > 0; xByte--)
	{

		for (int xBit = 1; xBit != 9; ++xBit)
		{
			// get single bit from integer by moderating loops position
			bool __bit = bitRead(value, (xByte * 8) - xBit);
			//Serial.print(__bit);

			// write each bit to byte
			bitWrite(bitSum, xBit, __bit);

			// if reached 8 bit count  then push to memory by one byte for 4 times
			if (xBit == 8)
			{

				//Serial.println("  countBITS:" + String((xByte * 8) - xBit) + "address:[" + String(address) + "],byte:" + String(bitSum));
				EEPROM.write(address, bitSum);
				bitSum = 0;// reset              
				address += 1;
			}
		}

	}

}






// if values in positive side
unsigned long int readEEPROM32(int_fast16_t address) {


	signed long int bitSum;

	byte value;
	for (int_fast16_t xByte = 4; xByte > 0; xByte--)
	{
		value = EEPROM.read(address);
		for (int xBit = 1; xBit != 9; ++xBit)
		{
			// get single bit from integer by moderating loops position
			bool __bit = bitRead(value, xBit);
			//Serial.print(__bit);

			// write each bit to byte
			bitWrite(bitSum, (xByte * 8) - xBit, __bit);

			// if reached 8 bit count  then push to memory by one byte for 4 times
			if (xBit == 8)
			{

				//Serial.println("  countBITS:" + String((xByte * 8) - xBit) + "address:[" + String(address) + "],byte:" + String(value) + ",  bitSum:" + String(bitSum));
				         
				address += 1;
			}
		}

	}
	if (bitSum > 0 )
		return bitSum + 1; // always return 1 value lower
	else 
	   return bitSum;

}


*/


// Not supported
/*
void writeEEPROM32(int_fast16_t address, signed long int value) {


	byte bitSum;

	for (int_fast16_t xByte = 4; xByte > 0; xByte--)
	{

		for (int xBit = 1; xBit != 9; ++xBit)
		{
			// get single bit from integer by moderating loops position
			bool __bit = bitRead(value, (xByte * 8) - xBit);
			Serial.print(__bit);

			// write each bit to byte
			bitWrite(bitSum, xBit, __bit);

			// if reached 8 bit count  then push to memory by one byte for 4 times
			if (xBit == 8)
			{

				Serial.println("  countBITS:" + String((xByte * 8) - xBit) + "address:[" + String(address) + "],byte:" + String(bitSum));
				EEPROM.write(address, bitSum);
				bitSum = 0;// reset              
				address += 1;
			}
		}

	}

}

*/


//ERROR
//bitRead (value,64); ERROR: invalid operands of types 'double' and 'int' to binary 'operator>>'

/*  
  
 void writeEEPROM32 (int_fast16_t address, double  value ){


  byte bitSum;

	for (int_fast16_t xByte = 8; xByte > 0; xByte--)
	  {

			for (int xBit= 1; xBit != 9; ++xBit)
					{
						  // get single bit from integer by moderating loops position
						 bool __bit = bitRead(value, (xByte * 8) - xBit);
						 Serial.print( __bit);

						  // write each bit to byte
						  bitWrite(bitSum, xBit,__bit);

						 // if reached 8 bit count  then push to memory by one byte for 4 times
						 if ( xBit == 8 )
						   {

							  Serial.println("  countBITS:"+String((xByte * 8) - xBit)+"address:["+String(address)+"],byte:"+String(bitSum));
							  EEPROM.write (address,bitSum);
							  bitSum=0;// reset
							  address+=1;
						   }
					}

	   }

  }

  */