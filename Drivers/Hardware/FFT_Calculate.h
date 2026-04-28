#ifndef __FFT_H
#define __FFT_H
#include "stm32h743xx.h"
#include "ADC_DMA_TIM.h"
extern __attribute__((aligned(32))) float fft_input_buffer[ADC_BUFFER_SIZE];
extern __attribute__((aligned(32))) float fft_output_buffer[ADC_BUFFER_SIZE];
extern __attribute__((aligned(32))) float fft_magnitude[ADC_BUFFER_SIZE/2+1];
void calculate_fft(void);
void WIN_power_correction(float *fft_magnitude);
#define peaks_count  4
extern float fft_magnitude[];
typedef struct 
{
    float   magnitude;
    uint16_t    index;
} Peak_t;
void fft_init(void);
void find_top_peaks_arm(float *fft_magnitude, uint8_t topN, Peak_t peaks[]);
extern Peak_t top_peaks[peaks_count];
#endif
