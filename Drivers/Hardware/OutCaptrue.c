#include "stm32h743xx.h"                  // Device header
#include "tim.h"
void TIM12_OC_PWM_Start(void)
{
    HAL_TIM_Base_Start(&htim12);
    HAL_TIM_OC_Init(&htim12);
    HAL_TIM_OC_Start(&htim12,TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&htim12,TIM_CHANNEL_2);
}
