#ifndef _I2S_H
#define _I2S_H

#include "driver/i2s.h"
#include "driver/adc.h"
#include <math.h>

#define PI 3.14159265
#define SAMPLE_RATE 44100
#define AMPLITUDE 4500

#define I2S_BCK_IO      27
#define I2S_WS_IO       26
#define I2S_DO_IO       25

void I2SInit();
void generateSineWave(double freq, double duration);

#endif
