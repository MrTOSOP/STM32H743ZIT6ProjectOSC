#ifndef __debug_out_h
#define __debug_out_h
#include "stm32h743xx.h"
void Print_ADC_Value(void);
void Print_out_FFT_fft_magnitude(void);
void Plot_WaveformSimple(uint16_t* adc_buffer, uint32_t size, uint32_t plot_color, float vref, uint8_t adc_bits);
void LCD_Show_Freq_Duty(void);
void LCD_Show_FFT_Freq_magnitude(void);
void LCD_Show_Basic(void);
void LCD_Show_Flags(void);
void LCD_Show_Numbers_test(void);
#endif

