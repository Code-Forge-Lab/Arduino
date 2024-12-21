#include <ESP.h>
#include <SPIFFS.h>

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Initialize the SPIFFS file system
  if (!SPIFFS.begin(true)) {
    Serial.println("Initialization failed!");
    return;
  }
  
  // size of the compiled program
  uint32_t program_size = ESP.getSketchSize();
  
  // size of the file system
 
  uint32_t file_system_size = SPIFFS.totalBytes();
  
  // used size of the file system
  uint32_t file_system_used = SPIFFS.usedBytes();
  
  // free size in the flash memory
  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;
  
  Serial.print("Program size: ");
  Serial.print(program_size);
  Serial.println(" bytes");
  
  Serial.print("File system size: ");
  Serial.print(file_system_size);
  Serial.println(" bytes");
  
  Serial.print("File system used: ");
  Serial.print(file_system_used);
  Serial.println(" bytes");
  
  Serial.print("Free space: ");
  Serial.print(free_size);
  Serial.println(" bytes");
}

void loop() {
  // Do Nothing Here
}