

 




int readMemory (int address ) {
    int getdata = EEPROM.read(address);    
    if (getdata == 255) {
        getdata = 0;
            EEPROM.write(address,getdata);
        }

         return getdata;
}


int writeMemory (int address , int setdata , bool write = true) {
     
          if (write) // boolean control
            EEPROM.write(address,setdata);
        
         return setdata;
}



struct  controls
{
private:
   int8_t old_value; // it's for changed value 
public:
   int8_t address;
   int8_t value;  
 
      // contructor 
      controls () {
        value = readMemory (address); // initiliaze value from EEPROM
      }
   

        int8_t getValue () {
                 value = readMemory (address);
                   return value;
            }
        
           // set with argument 
        void setValue (int8_t setdata  , bool write=true) {
                 writeMemory (address,setdata, write);
                 value = setdata;
        }    
         // aut set value
         void setValue ( bool write = true) {
               
                writeMemory (address,value,write);
         }
   
        // Add by one 
        void addValue () {
             if (value < 254) {
                    ++value;                
             }
        }
     // substract value by one 
        void subValue () {
            if (value > 1) {
                   --value;                
            }
       }
};




