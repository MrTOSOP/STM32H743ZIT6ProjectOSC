#include "stm32h743xx.h"
#include "gpio.h"
#include "Debug_Out.h"
#include "PIN_EXTI_Slove.h"

/* ========= 时间参数(单位:TIM17 tick,1 tick ≈ 16.66ms) ========= */
#define DEBOUNCE_TICKS      2     /* 消抖确认:~33ms */
#define LONG_PRESS_TICKS    60    /* 长按阈值:~1.0s,想要 1.5s 就改成 90 */
/* ================================================================== */

/* 短按标志(0/1 翻转) */
volatile uint8_t Flag_GPIOF_PIN_6 = 0;
volatile uint8_t Flag_GPIOF_PIN_7 = 0;

/* 长按事件标志(主循环里读到后自行清零) */
volatile uint8_t Flag_LongPress_PIN_6 = 0;
volatile uint8_t Flag_LongPress_PIN_7 = 0;

/* 按键状态机 */
typedef enum {
    KEY_IDLE = 0,        /* 空闲,等 EXTI */
    KEY_DEBOUNCE,        /* 消抖确认中 */
    KEY_PRESSED,         /* 已确认按下,计时中 */
    KEY_LONG_FIRED       /* 长按已触发,等释放 */
} KeyState_t;

typedef struct {
    KeyState_t   state;
    uint16_t     hold_ticks;
    GPIO_TypeDef *port;
    uint16_t     pin;
} KeyInfo_t;

static KeyInfo_t Key6 = { KEY_IDLE, 0, GPIOF, GPIO_PIN_6 };
static KeyInfo_t Key7 = { KEY_IDLE, 0, GPIOF, GPIO_PIN_7 };

/* ---------------- 用户事件回调 ---------------- */
static void Key6_OnShortPress(void)
{
    Flag_GPIOF_PIN_6++;
    if (Flag_GPIOF_PIN_6 == 2) Flag_GPIOF_PIN_6 = 0;
    LCD_Show_Flags();
}

static void Key6_OnLongPress(void)
{
    Flag_LongPress_PIN_6 = 1;   /* 主循环里检测到后做事并清零 */
    LCD_Show_Flags();
}

static void Key7_OnShortPress(void)
{
    Flag_GPIOF_PIN_7++;
    if (Flag_GPIOF_PIN_7 == 2) Flag_GPIOF_PIN_7 = 0;
    LCD_Show_Flags();
}

static void Key7_OnLongPress(void)
{
    Flag_LongPress_PIN_7 = 1;
    LCD_Show_Flags();
}
/* ---------------------------------------------- */

/* 单个按键的状态推进,在 TIM17 周期里被调用 */
static void Key_Scan(KeyInfo_t *k,
                     void (*on_short)(void),
                     void (*on_long)(void))
{
    uint8_t pressed = (HAL_GPIO_ReadPin(k->port, k->pin) == GPIO_PIN_RESET);

    switch (k->state)
    {
        case KEY_IDLE:
            /* 等 EXTI 把状态推到 DEBOUNCE */
            break;

        case KEY_DEBOUNCE:
            if (pressed) {
                if (++k->hold_ticks >= DEBOUNCE_TICKS) {
                    k->state = KEY_PRESSED;        /* 消抖通过,确认按下 */
                    /* hold_ticks 继续累计,作为按下持续时间 */
                }
            } else {
                k->state = KEY_IDLE;               /* 是抖动,丢弃 */
                k->hold_ticks = 0;
            }
            break;

        case KEY_PRESSED:
            if (pressed) {
                if (++k->hold_ticks >= LONG_PRESS_TICKS) {
                    k->state = KEY_LONG_FIRED;
                    if (on_long) on_long();        /* 长按立即触发 */
                }
            } else {
                /* 还没到长按阈值就松手 → 短按 */
                if (on_short) on_short();
                k->state = KEY_IDLE;
                k->hold_ticks = 0;
            }
            break;

        case KEY_LONG_FIRED:
            if (!pressed) {                        /* 等用户松手 */
                k->state = KEY_IDLE;
                k->hold_ticks = 0;
            }
            break;
    }
}

/* ============ EXTI 入口:仅启动状态机,不做判断 ============ */
void GPIOF_PIN_6_Function(void)
{
    if (Key6.state == KEY_IDLE) {
        Key6.state      = KEY_DEBOUNCE;
        Key6.hold_ticks = 0;
    }
}

void GPIOF_PIN_7_Function(void)
{
    if (Key7.state == KEY_IDLE) {
        Key7.state      = KEY_DEBOUNCE;
        Key7.hold_ticks = 0;
    }
}

/* ============ TIM17 周期入口:推进所有按键状态机 ============ */
void Keys_Tick(void)
{
    Key_Scan(&Key6, Key6_OnShortPress, Key6_OnLongPress);
    Key_Scan(&Key7, Key7_OnShortPress, Key7_OnLongPress);
}

