bool buttonRelease (int btn ) {
       bool pressed = false; 
         while (digitalRead(btn) > 0)
         {
              pressed = true;
              delay(50);
              Serial.println("Button Pressed: "+ String(btn));
          }
             if (pressed) {
                 Serial.println("Button Released");
                 pinMode (13,OUTPUT);
                  delay(30);
                  digitalWrite(13,HIGH);
                  delay(30);
                  digitalWrite(13,LOW);
                 pinMode (13,INPUT);
             }
        return pressed;
  }
