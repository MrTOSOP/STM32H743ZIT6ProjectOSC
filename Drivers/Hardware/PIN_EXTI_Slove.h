#ifndef PIN_EXTI_SLOVE_H
#define PIN_EXTI_SLOVE_H

#include "stm32h7xx_hal.h"

extern volatile uint8_t Flag_GPIOF_PIN_6;
extern volatile uint8_t Flag_GPIOF_PIN_7;

/* 长按一次性标志,主循环消费后清零 */
extern volatile uint8_t Flag_LongPress_PIN_6;
extern volatile uint8_t Flag_LongPress_PIN_7;

void GPIOF_PIN_6_Function(void);   /* EXTI 调用 */
void GPIOF_PIN_7_Function(void);   /* EXTI 调用 */
void Keys_Tick(void);              /* TIM17 中调用(每 16.66ms 一次) */

#endif