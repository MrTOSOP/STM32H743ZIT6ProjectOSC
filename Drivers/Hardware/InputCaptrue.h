#ifndef __IC_H
#define __IC_H
#include "stm32h743xx.h"                  // Device header

void Start_InputCaptrue(void);
float IC_GetFreq(void);
uint8_t IC_GetDuty(void);
#endif
