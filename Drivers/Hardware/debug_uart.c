#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <errno.h>
#include "usart.h"
/* 包含你的 UART 句柄声明（CubeMX 生成）*/

/* 注意：下面代码同时提供对 ARMCC/ARMCLANG 与 GCC 的支持 */

#if defined (__ARMCC_VERSION) || defined(__CC_ARM) || defined(__ARMCC__)

    /* Keil/ARM Compiler */

//    struct __FILE { int handle; };
    FILE __stdout;
    FILE __stdin;

    /* fputc - used by printf */
    int fputc(int ch, FILE *f)
    {
        uint8_t c = (uint8_t)ch;
        HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
        return ch;
    }

    /* fgetc - used by scanf (optional) */
    int fgetc(FILE *f)
    {
        uint8_t c = 0;
        HAL_UART_Receive(&huart1, &c, 1, HAL_MAX_DELAY);
        return c;
    }

    /* _sys_exit to avoid semihosting calls (safe infinite loop) */
    void _sys_exit(int return_code)
    {
        (void)return_code;
        while (1) {}
    }

#elif defined (__GNUC__)

    /* GCC (newlib) */
    /* Implement _write so that printf() uses it */
    int _write(int file, char *ptr, int len)
    {
        (void)file;
        HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
        return len;
    }

    /* Optionally implement _read for scanf */
    int _read(int file, char *ptr, int len)
    {
        (void)file;
        HAL_UART_Receive(&huart1, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
        return 1;
    }

#else

    /* Fallback: try implementing fputc */
    struct __FILE { int handle; };
    FILE __stdout;
    int fputc(int ch, FILE *f)
    {
        uint8_t c = (uint8_t)ch;
        HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
        return ch;
    }

#endif
