
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

 




int readMemory (int address ) {
    int getdata = EEPROM.read(address);    
    if (getdata == 255) { // in memory all bits are on == (255) / else i use 254 to separate this false
        getdata = 0;
            EEPROM.write(address,getdata);
        }

         return getdata;
}


int writeMemory (int address , int setdata , bool write = true) {
     
          if (write) // boolean control
            EEPROM.write(address,setdata);
        delay (20); // 5ml seconds it takes to write in memory
         return setdata;
}



class  controls
{
private:
     int old_value; // it's for changed value 
    
public:
     int address;
     int value;   
 
      // contructor 
      controls (int address) {
          this->address = address;
        getValue (); // initiliaze value from EEPROM
      }
   

        int getValue () {
                 value = readMemory (address);
                   return value;
            }
       

           // set with argument 
        void setValue (int setdata  , bool write=true) {
            if (old_value != value) { // to protect from always write
                    writeMemory (address,setdata);
                    value = setdata;
                    old_value = value; // cach new value
                    Serial.print("setValue (int setdata  , bool write=true)");
                }     
        }    
         // aut set value
         void setValue ( bool write = true) {
             if (old_value != value) { // to protect from always write
                      writeMemory (address,value,write);
                      old_value = value;  // cach new value
                      Serial.print("setValue ( bool write = true)");
             }
         }
   
        // Add by one 
        void addValue () {
             if (value < 254) {
                    ++value;    
                    Serial.print ("add: "+ String (value));            
             }
        }
     // substract value by one 
        void subValue () {
            if (value > 0) {
                   --value;                
                   Serial.print ("substract: "+ String (value));
            }
       }
};




