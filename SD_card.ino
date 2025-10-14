#include <TFT_eSPI.h>
#include <SPI.h>

#include "mono_file.h"
#include "i2s.h"
#include "gui.h"

TFT_eSPI tft = TFT_eSPI();

void playTest(char * path){

  Serial.printf("Playing %s\n", path);

  for(int i=3;i>0;i--){

    Serial.println(i);

  }

  playMonoWAVFile(SD_MMC, path);

}

void monoWAVTest(){

  Serial.println("TESTING mono_file SD functionality.");

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

  // Serial.println("Testing sine wave generation...");

  // for(int i=3;i>0;i--){

  //   Serial.println(i);

  // }

  // generateSineWave(261.63, 1.00, 1.00);
  // generateSineWave(329.628, 1.00, 0.8);
  // generateSineWave(392, 1.00, 0.6);
  // generateSineWave(329.628, 1.00, 0.4);
  // generateSineWave(261.63, 1.00, 0.2);

  // Serial.println("Testing playing from file.");

  // int i;

  // for(i=3;i>0;i--){

  //   Serial.println(i);

  //   delay(1000);

  // }

  // Serial.println("Playing 5.wav");

  // playMonoWAVFile(SD_MMC, "/5.wav");

  // Serial.println("Playing 10.wav");

  // playMonoWAVFile(SD_MMC, "/10.wav");

  // Serial.println("Playing 7.wav");

  // playMonoWAVFile(SD_MMC, "/7.wav");

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

    Serial.println("Initializing I2S.");

    I2SInit();

    listDir(SD_MMC, "/", 0);

    Serial.println("CALLING getDirFilePaths");

    // std::vector<String> filepaths = getDirFilePaths(SD_MMC, "/");

    Serial.println("Initializing TFT.");
  
    tft.init();
    tft.setRotation(0);
    
    tft.setTextSize(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
    monoWAVTest();

    int i = 24;

    tft.drawString("Files", 10, 10, 2);

    for(const auto &filepath: filepaths){

      tft.drawString(filepath, 10, i, 2);

      i += 14;

    }

  } 

}

void loop() {
  
}