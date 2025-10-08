#ifndef __SD_READ_WRITE_H
#define __SD_READ_WRITE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "Arduino.h"
#include "FS.h"

struct __attribute__((packed)) MonoWAVHeader {

  char riff[4];
  uint32_t chunk_size;
  char wave[4];
  char fmt[4];
  uint32_t subchunk1_size;
  uint16_t audio_format;
  uint16_t num_channels = 1;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  char data[4];
  uint32_t subchunk2_size;

};

// General IO functions.

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void writeFileFormatString(fs::FS &fs, const char * path, const char * message, ...);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);

// writeFile specific helper functions

char * intToString(unsigned int num, int base);

// WAV specific functions.

void createMonoWAVFile(fs::FS &fs, const char * path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample);
void writeSineWave(fs::FS &fs, const char * path, float frequency, float duration);
void editMonoWAVHeader(fs::FS &fs, const char * path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample);

#endif
