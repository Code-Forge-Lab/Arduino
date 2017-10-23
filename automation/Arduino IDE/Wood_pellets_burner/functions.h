

 




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
public:
   int8_t address;
   int8_t value;  

        int8_t getValue () {
                 value = readMemory (address);
                   return value;
            }
        int8_t setValue (int8_t setdata , bool write=true) {
            writeMemory (address,setdata, write);
        }    

};
