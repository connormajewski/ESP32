#include "i2c.h"

void I2SInit(){

  i2s_config_t i2s_config = {

    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = true

  };

  i2s_pin_config_t pin_config = {

    .bck_io_num = I2S_BCK_IO,
    .ws_io_num = I2S_WS_IO,
    .data_out_num = I2S_DO_IO,
    .data_in_num = I2S_PIN_NO_CHANGE

  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);

}

void generateSineWave(double freq, double duration){

  uint16_t sampleBuffer[256];

  double phase = 0.0;
  double sample;

  size_t bytes_written;

  double phaseIncrement = 2.0f * PI * freq / SAMPLE_RATE;

  int currentSample = 0;
  int totalSamples = SAMPLE_RATE * duration;

  while(currentSample < totalSamples){

    for(int i=0;i<256;i++){

      sample = sinf(phase);

      phase += phaseIncrement;

      if(phase > 2.0f * PI) phase -= 2.0f * PI;

      sampleBuffer[i] = (uint16_t)(AMPLITUDE * sample);

      currentSample++;

    }

    i2s_write(I2S_NUM_0, &sampleBuffer, sizeof(sampleBuffer), &bytes_written, portMAX_DELAY);

  }

}