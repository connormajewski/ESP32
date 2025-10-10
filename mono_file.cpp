#include "mono_file.h"

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

void writeSineWave(fs::FS &fs, const char * path, float freq, float duration){

  createMonoWAVFile(fs, path, 44100, 44100, 16);

  File file = fs.open(path, FILE_APPEND);

  int sum = 0;
  int expectedSum = 0;

  int bufferSize = 256;

  uint16_t buffer[bufferSize];

  int j = 0;

  int i;

  int f = 1;

  Serial.println(freq);

  int s = 44100;

  for (i = 0; i < (s * duration); i++) {

    // double root = (sin(M_TWO_PI * freq * ((double)i / 44100)));
    // double third = (sin(M_TWO_PI * 329.628 * ((double)i / 44100)));
    // double fifth = (sin(M_TWO_PI * 392.00 * ((double)i / 44100)));

    // buffer[j++] = (uint16_t)(((root + third + fifth) / 3.0) * 32767);

    buffer[j++] = (uint16_t)(sin(M_TWO_PI * freq * ((double)i / 44100)) * 32767);

    if (j == bufferSize) {

      j = 0;

      size_t written = file.write((uint8_t*)&buffer, bufferSize * sizeof(uint16_t));

    }

  }

  if (j > 0) {

    file.write((uint8_t*)&buffer, j * sizeof(uint16_t));
  }

  file.close();

  editMonoWAVHeader(fs, path, (int) (s * duration), 44100, 16);

}

void printMonoWAVData(fs::FS &fs, const char * path){

  File file = fs.open(path, FILE_READ);

  if(!file){

    Serial.printf("%s could not be opened.\n", path);

    return;

  }

  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t fileSize;


  file.seek(22);
  file.read((uint8_t *)&numChannels, 2);

  file.seek(24);
  file.read((uint8_t *)&sampleRate, 4);

  file.seek(40);
  file.read((uint8_t *)&fileSize, 4);

  Serial.printf("\nFILE INFO:\n\nNUM CHANNELS: %d\nSAMPLE RATE: %d\nDuration: %.2fs\n\n", numChannels, sampleRate, (double) (fileSize / sampleRate / 2));

  file.close();

}

void playMonoWAVFile(fs::FS &fs, const char * path){

  File file = fs.open(path, "r");

  if(!file){

    Serial.printf("%s could not be opened.\n", path);

    return;

  }

  Serial.printf("Opened %s\n", path);

  file.close();

}