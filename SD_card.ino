#include "sd_read_write.h"
#include "SD_MMC.h"
#include <math.h>

#define SD_MMC_CMD 15  //Please do not modify it.
#define SD_MMC_CLK 14  //Please do not modify it.
#define SD_MMC_D0 2    //Please do not modify it.

#ifndef M_PI
#define M_PI (3.141592654)
#endif

double twopi = 2.00 * M_PI;

void setup() {
  delay(500);
  Serial.begin(115200);
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  Serial.print("SD_MMC Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  listDir(SD_MMC, "/", 0);

  Serial.println("Calling createMonoWAVFile()...");

  createMonoWAVFile(SD_MMC, "/test1.wav", 44100, 44100, 16);

  writeFile(SD_MMC, "/test.txt", "Hello World!");

  //writeFileFormatString(SD_MMC, "/test.txt", "Hello %d\n", 100);

  File file = SD_MMC.open("/test1.wav", FILE_APPEND);

  int sum = 0;
  int expectedSum = 0;

  int bufferSize = 256;

  uint16_t buffer[bufferSize];

  int j = 0;

  int duration = 5;

  double freq = 261.63;

  int i;

  int f = 1;

  Serial.println(freq);

  int s = 44100;
  double d = 10.5;

  for (i = 0; i < (s * d); i++) {

    if (i > (s * d) / 2 && f) {

      freq = 392.00;

      Serial.println(freq);

      f = 0;
    }

    buffer[j++] = (uint16_t)(sin(twopi * freq * ((double)i / 44100)) * 32767);


    if (j == bufferSize) {

      j = 0;

      size_t written = file.write((uint8_t*)&buffer, bufferSize * sizeof(uint16_t));

      sum += written;
      expectedSum += sizeof(uint16_t);

      Serial.printf("%d / %d expected samples written.\n\r", sum, expectedSum);
    }
  }

  if (j > 0) {

    file.write((uint8_t*)&buffer, j * sizeof(uint16_t));
  }

  file.close();

  editMonoWAVHeader(SD_MMC, "/test1.wav", (int) (s * d), 44100, 16);

  // Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\r\n", SD_MMC.usedBytes() / (1024 * 1024));
}

void loop() {
  delay(10000);
}