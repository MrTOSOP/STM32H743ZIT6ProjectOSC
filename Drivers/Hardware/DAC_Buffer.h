#ifndef __DAC_Buffer_H
#define __DAC_Buffer_H
#include "stm32h743xx.h"                  // Device header
#define WAVE_Buffer_4095 4096
#define WAVE_Buffer_1024 1024
#define WAVE_Buffer_255 255
extern const uint16_t Sin_WAVE_Buffer_255[WAVE_Buffer_255];
extern const uint16_t Sin_WAVE_Buffer_4095[WAVE_Buffer_4095];
extern const uint16_t Sin_WAVE_Buffer_1024[WAVE_Buffer_1024];
extern const uint16_t sawtooth_wave_4095[WAVE_Buffer_4095];
#endif
