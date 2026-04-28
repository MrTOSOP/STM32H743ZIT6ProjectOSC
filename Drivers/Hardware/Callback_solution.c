#include "stm32h743xx.h"
#include "ADC_DMA_TIM.h"
#include "adc.h"
#include "stdio.h"
#include "Debug_Out.h"
#include "FFT_Calculate.h"
#include "PIN_EXTI_Slove.h"
volatile uint8_t adc_Completed_flag = 0;
volatile uint8_t adc_half_Completed_flag = 0;
/*
回调函数
*/
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC3)
    {
        adc_half_Completed_flag=1;
        /*
        采集到的波形输出
        */
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC3)
    {
        
        /*
        FFT处理后的输出fft_magnitude
        */
        adc_Completed_flag=1;
        //重启ADC+DMA
        ADC_DMA_Call_Back_Start();
    }
}

