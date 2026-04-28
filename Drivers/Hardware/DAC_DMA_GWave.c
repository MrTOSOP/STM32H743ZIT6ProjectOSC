#include "stm32h743xx.h"                  // Device header
#include "dac.h"
#include "math.h"
#include "dma.h"
#include "tim.h"
#include "DAC_DMA_GWave.h"
#include "DAC_Buffer.h"
#define dac_psc 9
#define dac_arr  1

void Wave_Value_G(void)
{
    TIM7->ARR = dac_arr;
    TIM7->PSC = dac_psc;
    HAL_DAC_Stop_DMA(&hdac1,DAC_CHANNEL_1);
    HAL_DAC_Stop(&hdac1,DAC_CHANNEL_1);
    HAL_DAC_Stop(&hdac1,DAC_CHANNEL_2);
    
    HAL_TIM_Base_Start(&htim7);
    HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1,(uint32_t*)Sin_WAVE_Buffer_4095,WAVE_Buffer_4095,DAC_ALIGN_12B_R);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_2);//Èý½Ç²¨
}
