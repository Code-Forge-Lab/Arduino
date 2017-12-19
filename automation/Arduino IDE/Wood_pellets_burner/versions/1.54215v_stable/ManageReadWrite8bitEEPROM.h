
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
           delay (70); // 5ml seconds it takes to write in memory
         return setdata;
}


// Manage read write 8 bit memory 

class  controls
{
private:
     unsigned  int old_value; // it's for changed value 
     unsigned  int address;
     unsigned  int value; 
     unsigned  int defaultdata;  
public:
     
 
      // contructor 
      controls (unsigned int address , unsigned int __defaultdata = 0 ) {  // unsigned int data = 999
          this->address =address;
          readValue (); // initiliaze value from EEPROM
          old_value = 65535; // FIX BUG when selected value equal 0 and default created old_value also equal 0 , ignore to write 

          defaultdata = __defaultdata; // default datata for the future
      }
   
 // read data 
         unsigned  int readValue () { // read directly from memory
                 value = readMemory (address);
                   return value;
            }
       
           unsigned int getValue () { // read from RAM 
                  return value;
           }     

            unsigned int getAddress () { // read from RAM 
                  return address;
           }  
            
           // write data  manualy
        void setValue (int setdata  ) {
            if (old_value != value) { // to protect from always write
                    writeMemory (address,setdata,true);                
                    value = setdata;
//                   
                    Serial.println("setValue (int setdata) old_value: "+String(old_value) + ", value:"+value + String(", condition: ") + (old_value != value) );
                    old_value = value; // cach new value
                }     
        }    
         // auto write data
         void setValue () {
             if (old_value != value   ) { // to protect from always write                                       
                      writeMemory (address,this->value,true);
  
                      Serial.println("setValue () old_value: "+String(old_value) + ", value:"+value);
                      old_value = value;  // cach new value
             } else 
                      Serial.println("ERROR:setValue () old_value: "+String(old_value) + ", value:"+value);
         }  
   
        // Increment value/data
        void addValue () {
             if (value < 254) {
                    ++value;    
                    Serial.println ("add: "+ String (value));            
             }
        }
        // Decrement value/data 
        void subValue () {
            if (value > 0) {
                   --value;                
                   Serial.println ("substract: "+ String (value));
            }
       }

       void setDataDefault () {
                    writeMemory (address,defaultdata,true);                
                    value = defaultdata;
        }
};




