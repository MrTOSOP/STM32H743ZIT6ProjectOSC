#ifndef __Callback_solution_H
#define __Callback_solution_H
#include "stm32h743xx.h"
#include "adc.h"
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
extern volatile uint8_t adc_Completed_flag;
extern volatile uint8_t adc_half_Completed_flag;
#endif
