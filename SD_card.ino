#include "mono_file.h"
#include "i2s.h"

void monoWAVTest(){

  Serial.println("TESTING mono_file SD functionaluty.");

  listDir(SD_MMC, "/", 0);

  Serial.println("Writing 5.wav.");

  writeSineWave(SD_MMC, "/5.wav", 261.63, 5.00);

  printMonoWAVData(SD_MMC, "/5.wav");

  Serial.println("Writing 10.wav.");

  writeSineWave(SD_MMC, "/10.wav", 392.00, 10.00);

  printMonoWAVData(SD_MMC, "/10.wav");

  Serial.println("Writing 7.wav.");

  writeSineWave(SD_MMC, "/7.wav", 329.23, 7.00);

  printMonoWAVData(SD_MMC, "/7.wav");

  listDir(SD_MMC, "/", 0);

  Serial.println("Testing speaker...");

  for(int i=3;i>0;i--){

    Serial.println(i);

  }

  generateSineWave(261.63, 1.00);
  generateSineWave(329.628, 1.00);
  generateSineWave(392, 1.00);
  generateSineWave(329.628, 1.00);
  generateSineWave(261.63, 1.00);

}

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

    monoWAVTest();
	  
  }

}

void loop() {
  
}