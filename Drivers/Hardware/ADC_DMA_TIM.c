#include "stm32h743xx.h"
#include "ADC_DMA_TIM.h"
#include "adc.h"
#include "tim.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma.h"
#include "stdio.h"
__attribute__((section(".dma_buffer"))) __attribute__((aligned(32))) uint16_t adc_buffer[ADC_BUFFER_SIZE];


void ADC_DMA_TIM_Start(void)
{
    TIM6->PSC = TIM6_PSC;
    TIM6->ARR = TIM6_ARR;
    
    HAL_ADC_Stop_DMA(&hadc3);
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc3,(uint32_t*)adc_buffer,ADC_BUFFER_SIZE);
    HAL_Delay(50);
}

void ADC_DMA_Call_Back_Start(void)
{
    HAL_ADC_Start_DMA(&hadc3,(uint32_t*)adc_buffer,ADC_BUFFER_SIZE);
}
void ADC_DMA_Call_Back_Stop(void)
{
    HAL_ADC_Stop_DMA(&hadc3);
}

