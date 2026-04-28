#ifndef __ADC_DMA_TIM_H
#define __ADC_DMA_TIM_H
#include "stm32h743xx.h"
#define TIM6_PSC 19
#define TIM6_ARR 11
#define ADC_BUFFER_SIZE 1024
extern __attribute__((section(".dma_buffer"))) __attribute__((aligned(32))) uint16_t adc_buffer[ADC_BUFFER_SIZE];
void ADC_DMA_TIM_Start(void);
void ADC_DMA_Call_Back_Stop(void);
void ADC_DMA_Call_Back_Start(void);
#endif
