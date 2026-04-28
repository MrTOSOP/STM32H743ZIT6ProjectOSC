#include "stm32h743xx.h"                  // Device header
#include "tim.h"
void Start_InputCaptrue(void)
{
    TIM5->PSC = 1;
    HAL_TIM_Base_Start(&htim5);
    HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&htim5, TIM_CHANNEL_2);
}



float IC_GetFreq(void)
{
    return (120000000.0f/(__HAL_TIM_GET_COMPARE(&htim5, TIM_CHANNEL_1) + 1));
}

uint8_t IC_GetDuty(void)
{
    return ((TIM5->CCR2)+1)*100/((TIM5->CCR1)+1);
}
