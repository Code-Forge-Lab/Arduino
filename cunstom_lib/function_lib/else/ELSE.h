
#include <Wire.h>
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


// Manage read write 8 bit memory 

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
   
 // read data 
        int getValue () {
                 value = readMemory (address);
                   return value;
            }
       

           // write data  manualy
        void setValue (int setdata  , bool write=true) {
            if (old_value != value) { // to protect from always write
                    writeMemory (address,setdata);
                    value = setdata;
                    old_value = value; // cach new value
                    Serial.print("setValue (int setdata  , bool write=true)");
                }     
        }    
         // auto write data
         void setValue ( bool write = true) {
             if (old_value != value) { // to protect from always write
                      writeMemory (address,value,write);
                      old_value = value;  // cach new value
                      Serial.print("setValue ( bool write = true)");
             }
         }
   
        // Increment value/data
        void addValue () {
             if (value < 254) {
                    ++value;    
                    Serial.print ("add: "+ String (value));            
             }
        }
        // Decrement value/data 
        void subValue () {
            if (value > 0) {
                   --value;                
                   Serial.print ("substract: "+ String (value));
            }
       }
};




