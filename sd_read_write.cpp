#include "sd_read_write.h"

int SDInit(){

	SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
	
	if(!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
		Serial.println("SD card could not be mounted.");
		return 0;
	}
	
	uint8_t cardType = SD_MMC.cardType();
	
	if(cardType == CARD_NONE){
	
		Serial.println("No SD_MMC card attatched.");
		return 0;
		
	}
	
	Serial.println("SD card successfully mounted.");
	
	return 1;
	
}

void SDInfo(){
	
	uint8_t cardType = SD_MMC.cardType();

	Serial.printf("-------------------------\nSD_MMC card type: ");
	
	if(cardType == CARD_MMC) Serial.println("MMC.");
	else if(cardType == CARD_SDSC) Serial.println("SDSC.");
	else if(cardType == CARD_SDHC) Serial.println("SDHC.");
	else Serial.println("Unknown.");
	
	uint64_t cardSize = SD_MMC.cardSize() / (CHUNK_SIZE * CHUNK_SIZE);
	
	Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (CHUNK_SIZE * CHUNK_SIZE));
	Serial.printf("Used space: %lluMB\r\n-------------------------\n", SD_MMC.usedBytes() / (CHUNK_SIZE * CHUNK_SIZE));
	
}


void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

void writeFileFormatString(fs::FS &fs, const char * path, const char * message, ...){

    char * traversal;
    char * string;

    unsigned int i;
    double d;

    va_list arg;
    va_start(arg, message);

    for(int j=0;message[j] != '\0';j++){

      if(message[j] == '%'){

        j++;

        if(message[j] == 'd'){

          i = va_arg(arg, int);

          if(i < 0){

            i = -i;

            writeFile(fs, path, "-");

          }

          writeFile(fs, path, intToString(i,10));

        }

      }

      else{

        writeFile(fs, path, (char *)message[j]);

      }

    }

    va_end(arg);

}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void createMonoWAVFile(fs::FS &fs, const char *path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample) {

  struct MonoWAVHeader header;

  int num_channels = 1;

  memcpy(header.riff, "RIFF", 4);
  header.chunk_size = 36 + (num_samples * num_channels * (bits_per_sample / 8));
  memcpy(header.wave, "WAVE", 4);
  memcpy(header.fmt, "fmt ", 4);
  header.subchunk1_size = 16;
  header.audio_format = 1;
  header.num_channels = num_channels;
  header.sample_rate = sample_rate;
  header.byte_rate = sample_rate * num_channels * (bits_per_sample / 8);
  header.block_align = num_channels * (bits_per_sample / 8);
  header.bits_per_sample = bits_per_sample;
  memcpy(header.data, "data", 4);
  header.subchunk2_size = num_samples * num_channels * (bits_per_sample / 8);

  File file = fs.open(path, FILE_WRITE);

  if (!file) {

    Serial.println("File could not be created.");

    return;
  }

  size_t headerSize = sizeof(header);
  size_t written = file.write((uint8_t *)&header, headerSize);


  if (written == headerSize) {

    Serial.print((uint16_t)written);
    Serial.println(" bytes written successfullyy.");

  }

  else {

    Serial.println("Data could not be written to file.");

    return;
  }

  file.close();
}

void editMonoWAVHeader(fs::FS &fs, const char * path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample){

  File file = fs.open(path, "r+");

  if(!file){

    return;

  }

  uint32_t subChunk2Size = num_samples * (bits_per_sample / 8);
  uint32_t chunkSize = 36 + subChunk2Size;

  file.seek(4);
  file.write((uint8_t *)&chunkSize, 4);

  file.seek(40);
  file.write((uint8_t *)&subChunk2Size, 4);

  file.close();

}

char * intToString(unsigned int num, int base){

  static char nums[] = "0123456789";

  static char buffer[50];

  char * ptr;

  ptr = &buffer[49];

  *ptr = '\0';

  do{

      *--ptr = nums[num % base];

      num /= base;

  } while(num != 0);

  return ptr;

}