#include "mono_file.h"

void setup() {
	
  delay(500);
  
  Serial.begin(115200);
  
  int isSDInit = SDInit();
  
  if(!isSDInit){
	 
	Serial.println("Error initializing SD card.");
	 
  }
  
  else{
	 
	  SDInfo();

    writeSineWave(SD_MMC, "/sine.wav", 261.63, 5.00);

    listDir(SD_MMC, "/", 0);
	  
  }

}

void loop() {
  delay(10000);
}