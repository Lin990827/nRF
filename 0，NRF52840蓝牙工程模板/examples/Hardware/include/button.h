#ifndef __BUTTON_H
#define __BUTTON_H
#include "app_button.h"
#include <stdbool.h>
#include <stdint.h>


#define BUTTON_MAX_NUM 4 /* 按键最大数量 */

/* 按键引脚定义 */
#define BUTTON1 11 /* 按键 1 */
#define BUTTON2 12 /* 按键 2 */
#define BUTTON3 24 /* 按键 3 */
#define BUTTON4 25 /* 按键 4 */

#define BUTTON_TIMER_POLLING_TIME    50 /* 按键定时器轮询时间 */
#define BUTTON_BETWEEN_MAX_TIME      400 /* 按键按下到释放的最大间隔时间（ms） */
#define BUTTON_DOUBLE_PRESS_MAX_TIME 400 /* 按键两次按下的最大间隔时间（ms） */
#define BUTTON_LONG_PRESS_TIME       3600 /* 按键长按持续的时间（ms） */
#define BUTTON_ERROR_WAIT_TIME       800 /* 按键错误状态等待的时间（ms） */


typedef enum
{
    BUTTON_NO_ACTION = 0, /* 按键无动作 */
    BUTTON_PRESS_1, /* 按键按下一次 */
    BUTTON_PRESS_2, /* 按键按下两次 */
    BUTTON_PRESS_3, /* 按键按下三次 */
    BUTTON_PRESS_4, /* 按键按下四次 */
    BUTTON_PRESS_5, /* 按键按下五次 */
    BUTTON_LONG_PRESS, /* 按键长按 */
    BUTTON_ERROR /* 按键错误 */
} button_status_t; /* 按键状态枚举 */


typedef struct {
    uint8_t         pin; /* 按键引脚号 */
    uint32_t        polling; /* 按键定时器轮询次数 */
    uint8_t         action; /* 按键动作（APP_BUTTON_PUSH 或 APP_BUTTON_RELEASE） */
    button_status_t status; /* 按键状态 */
    bool            trigger; /* 按键触发标志（true：按键触发   false：按键没触发） */
} button_control_t; /* 按键控制结构体 */


typedef struct {
    uint8_t             pin; /* 按键引脚号 */
    uint8_t             action; /* 按键动作（APP_BUTTON_PUSH 或 APP_BUTTON_RELEASE） */
    nrf_gpio_pin_pull_t pull; /* 引脚上下拉状态 */
} button_cfg_t; /* 按键配置结构体 */


typedef struct {
    uint8_t         pin; /* 按键引脚号 */
    uint8_t         action; /* 按键动作（APP_BUTTON_PUSH 或 APP_BUTTON_RELEASE） */
    button_status_t status; /* 按键状态 */
} button_use_t; /* 按键应用结构体 */


typedef void (*button_callback_handler_t)(button_use_t *button_use); /* 按键回调函数指针（用于上层应用处理） */


bool button_init(button_cfg_t *cfg, uint8_t num, uint16_t time); /* 按键初始化 */
bool button_register_callback(button_callback_handler_t function); /* 按键注册回调 */
#endif
