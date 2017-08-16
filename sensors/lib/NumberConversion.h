
float BinAvailable(float inDecimal) {
  float base = 2;
  float CurrentBin = 1;
  for (float i = 0 ; i < inDecimal; i++) {
    CurrentBin = CurrentBin * base ;
    // Serial.print (CurrentBin);
    //Serial.print ("  :  ");

  }
  return (CurrentBin - 1 );
}

int DecToBinArr (int decimal , int *GetBinArr ) {
  int table[9] = {256,128,64,32,16,8,4,2,1}; // Posible values
  int bin[9] =   {0, 0, 0, 0 ,0, 0, 0 , 0 ,0};
 
  int __size = sizeof(table) / sizeof(table[0]) ;
  int __last_digit = decimal;

  // Find Out Where To Start  like from 16 
int MaxBasePos;
 for (int i = 0 ; i < __size; ++i )  {
      /*
       Serial.print ("GET ");
       Serial.print (table[__size-i-1]);
       Serial.print ( " >= ");
       Serial.print (__last_digit);
        Serial.println ( " ");
        */
       if (table[__size-i -1]  >=  __last_digit ) {
       MaxBasePos = __size- i -1; 
        /*
        Serial.print ( "--------MaxBasePos--------");
        Serial.println (MaxBasePos);
        */
        break;
  
        }
    }
//Get Values
 for (int i = MaxBasePos ; i < __size; ++i ) {
      /*
       Serial.println ("  ");
       Serial.print (i);
       Serial.print (".SET ");
       Serial.print (table[  i ] );
       Serial.print ( " - ");
       Serial.print (__last_digit);
       Serial.print ( " :: MaxBasePos ="); 
       Serial.print (  MaxBasePos );
       Serial.print ( " :: cBin =");
    */ 
        
    if ( __last_digit >= table[  i ]) {
      __last_digit = __last_digit -  table[ i ] ;
      bin[i+1] = 1;
     // Serial.print ( "----------------");
      }else {
        bin[i+1] = 0;
      }
      
     
      
     
  }
  
 //Serial.println ("");;
 for (int i = 0 ; i < __size+1;i++) {
    //Points Array From Each
     //  Serial.print (bin[i]);
    //   Serial.print ("_");
      GetBinArr[i] = bin[i];
         
  }