#include "stm32h743xx.h"
#include "Debug_Out.h"
#include "tim.h"
#include "Callback_solution.h"
#include "PIN_EXTI_Slove.h"
volatile uint8_t TIM17_EXTI_FLAG = 0;
volatile uint8_t TIM17_Count = 0;
void TIM17_EXTI_Function(void)
{
    if(__HAL_TIM_GET_FLAG(&htim17, TIM_FLAG_UPDATE) && __HAL_TIM_GET_IT_SOURCE(&htim17, TIM_IT_UPDATE))
    {
//TIM17的触发频率为5hz
        TIM17_EXTI_FLAG = 1;
        Keys_Tick();
    }
}

