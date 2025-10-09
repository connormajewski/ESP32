#ifndef _MONO_FILE_H
#define _MONO_FILE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "sd_read_write.h"

#define M_PI (3.141592654)

#ifndef M_TWO_PI
#define M_TWO_PI (2.00 * M_PI)
#endif

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

// WAV specific functions.

void createMonoWAVFile(fs::FS &fs, const char * path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample);
void writeSineWave(fs::FS &fs, const char * path, float frequency, float duration);
void editMonoWAVHeader(fs::FS &fs, const char * path, uint32_t num_samples, uint32_t sample_rate, uint16_t bits_per_sample);

#endif