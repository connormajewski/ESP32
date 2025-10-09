#include "mono_file.h"
#include "i2s.h"

void setup() {
	
  delay(500);
  
  Serial.begin(115200);
  
  int isSDInit = SDInit();
  
  if(!isSDInit){
	 
	  Serial.println("Error initializing SD card.");
	 
  }
  
  else{
	 
	  SDInfo();

    Serial.println("Initializing I2C.");

    I2SInit();

    // writeSineWave(SD_MMC, "/sine.wav", 261.63, 5.00);

    // listDir(SD_MMC, "/", 0);
	  
  }

}

void loop() {
  generateSineWave(261.63, 1.00);
  generateSineWave(329.628, 1.00);
  generateSineWave(392, 1.00);
  generateSineWave(329.628, 1.00);
  generateSineWave(261.63, 1.00);
}