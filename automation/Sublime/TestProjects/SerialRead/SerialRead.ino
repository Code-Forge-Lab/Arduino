int data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println("No ");
    delay(1000);
        Serial.println("Yes ");
    delay(1000);
     Serial.println("HAHAH");
    delay(1000);
}