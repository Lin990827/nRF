#include "button.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "app_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NRF_LOG_MODULE_NAME button_test
#define NRF_LOG_LEVEL       4 /* 0:Off 1:Error 2:Warning 3:Info 4:Debug */
#define NRF_LOG_INFO_COLOR  4 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#define NRF_LOG_DEBUG_COLOR 8 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();


#define BUTTON_BETWEEN_CNT      (BUTTON_BETWEEN_MAX_TIME / BUTTON_TIMER_POLLING_TIME) /* 按键按下到释放的次数 */
#define BUTTON_DOUBLE_PRESS_CNT (BUTTON_DOUBLE_PRESS_MAX_TIME / BUTTON_TIMER_POLLING_TIME) /* 按键两次按下的次数 */
#define BUTTON_LONG_PRESS_CNT   (BUTTON_LONG_PRESS_TIME / BUTTON_TIMER_POLLING_TIME) /* 按键长按持续的次数 */
#define BUTTON_ERROR_WAIT_CNT   (BUTTON_ERROR_WAIT_TIME / BUTTON_TIMER_POLLING_TIME) /* 按键错误状态等待的次数 */


APP_TIMER_DEF(button_polling); /* 按键轮询定时器 */


static uint8_t                   button_num    = 0; /* 按键数量 */
static uint8_t                   button_cb_num = 0; /* 按键回调函数数量 */
static uint32_t                  time_out      = 0; /* 软件定时器超时次数 */
static app_button_cfg_t *        configuration = NULL; /* 按键配置结构体指针（nordic SDK 库自带） */
static button_control_t *        control       = NULL; /* 按键控制结构体指针 */
static button_callback_handler_t button_callback_handler[BUTTON_MAX_NUM]; /* 按键回调函数（用于上层应用处理） */


/**@brief 按键获取触发标志函数
 *
 * @param[in]  None
 * @param[out] None
 *
 * @retval     false - 按键没触发
 * @retval     true - 按键触发
 *
 * @note None
 * @attention None
 */
static bool button_get_trigger(void)
{
    for (uint8_t i = 0; i < button_num; i++)
        if (control->trigger == true)
            return true;
    return false;
}


/**@brief 按键获取控制函数
 *
 * @param[in]  pin - 要获取的对应引脚号
 * @param[out] None
 *
 * @retval     按键控制结构体指针
 *
 * @note None
 * @attention None
 */
static button_control_t *button_get_control(uint8_t pin)
{
    for (uint8_t i = 0; i < button_num; i++)
        if (control[i].pin == pin)
            return (control + i);
    return NULL;
}


/**@brief 按键事件调度处理函数
 *
 * @param[in]  *p_event_data - 调度事件的数据指针
 * @param[in]  event_size - 调度事件的数据大小
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void button_event_scheduler_handler(void *p_event_data, uint16_t event_size)
{
    for (uint8_t i = 0; i < button_cb_num; i++)
        if (button_callback_handler[i] != NULL) /* 对应的按键回调函数指针不为空 */
            button_callback_handler[i]((button_use_t *)p_event_data); /* 调用对应的按键回调函数 */
}


/**@brief 按键事件处理函数
 *
 * @param[in]  pin_no - 触发事件的按键引脚
 * @param[in]  button_action - 按键动作（APP_BUTTON_RELEASE 或 APP_BUTTON_PUSH）
 *
 * @retval     None
 *
 * @note 支持的最多按键按下次数，再按下会变成按键错误。
 * @note 长按与点按处理不同的地方在于，点按是以按键松开为判断依据，而长按是以按下持续时间为判断依据.
 * @attention None
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    button_control_t *handler; /* 处理的按键控制 */

    if (button_get_trigger() == false) {
        APP_ERROR_CHECK(app_timer_start(button_polling, APP_TIMER_TICKS(BUTTON_TIMER_POLLING_TIME), NULL)); /* 开始按键轮询定时器 */
        NRF_LOG_DEBUG("Start button_polling timer.");
    }
    handler          = button_get_control(pin_no); /* 获取按键控制结构体指针 */
    handler->trigger = true; /* 按键触发 */
    switch (handler->status) /* 判断按键状态 */
    {
        case BUTTON_NO_ACTION: /* 按键无动作 */
        case BUTTON_PRESS_1: /* 按键按下一次 */
        case BUTTON_PRESS_2: /* 按键按下两次 */
        case BUTTON_PRESS_3: /* 按键按下三次 */
        case BUTTON_PRESS_4: /* 按键按下四次 */
            NRF_LOG_DEBUG("1.%s pin:%d   polling:%d   time_cnt:%d.", __func__, handler->pin, handler->polling, time_out);
            if (handler->action == APP_BUTTON_PUSH && button_action == APP_BUTTON_RELEASE) /* 按键按下并且按键动作释放 */
            {
                if (time_out - handler->polling <= BUTTON_BETWEEN_CNT) /* 当前软件定时器超时次数 - 按键定时器轮询次数 <= 按键按下到释放两者间的次数 */
                {
                    handler->status++; /* 对应按键状态增加 */
                    NRF_LOG_DEBUG("2.%s pin:%d   polling:%d   time_cnt:%d.", __func__, handler->pin, handler->polling, time_out);
                } else /* 超过按键按下到释放两者间的次数 */
                    handler->status = BUTTON_ERROR; /* 按键错误 */
            }
            handler->action  = button_action; /* 按键控制动作赋值为按模动作 */
            handler->polling = time_out; /* 按键控制轮询次数为当前定时器超时次数 */
            break;
        case BUTTON_PRESS_5: /* 按键按下五次 */
        case BUTTON_LONG_PRESS: /* 按键长按 */
        case BUTTON_ERROR: /* 按键错误 */
        default:
            NRF_LOG_DEBUG("3.%s pin:%d   polling:%d   time_cnt:%d.", __func__, handler->pin, handler->polling, time_out);
            handler->status  = BUTTON_ERROR; /* 按键错误 */
            handler->action  = button_action; /* 按键控制动作赋值为按模动作 */
            handler->polling = time_out; /* 按键控制轮询次数为当前定时器超时次数 */
            break;
    }
    NRF_LOG_DEBUG("4.%s Error schedule.", __func__);
    button_use_t use = { pin_no, button_action, BUTTON_ERROR }; /* 应用的按键 */
    APP_ERROR_CHECK(app_sched_event_put(&use, sizeof(button_use_t), button_event_scheduler_handler)); /* 事件调度 */
}


/**@brief 按键轮询处理（计时器到期时执行的函数）函数
 *
 * @param[in]  *p_context - 用户数据指针（未使用）
 * @param[out] None
 *
 * @retval     按键控制结构体指针
 *
 * @note None
 * @attention None
 */
static void button_polling_handler(void *p_context)
{
    bool         event_flag = false;
    uint8_t      i;
    button_use_t use; /* 按键应用 */

    time_out++; /* 软件定时器超时次数增加 */
    for (i = 0; i < button_num; i++) {
        switch (control[i].status) /* 判断按键控制状态 */
        {
            case BUTTON_NO_ACTION: /* 按键无动作 */
                if (time_out - control[i].polling >= BUTTON_LONG_PRESS_CNT &&
                    control[i].action == APP_BUTTON_PUSH) /* 软件定时器超时次数 - 按键定时器轮询次数 >= 按键长按持续的次数，并且按键控制动作为按下 */
                {
                    control[i].status = BUTTON_LONG_PRESS; /* 按键控制状态赋值为按键长按 */
                    use.status        = BUTTON_LONG_PRESS; /* 按键应用状态赋值为按键长按 */
                    use.pin           = control[i].pin; /* 按键应用引脚赋值为按键控制引脚 */
                    APP_ERROR_CHECK(app_sched_event_put(&use, sizeof(button_use_t), button_event_scheduler_handler)); /* 事件调度 */
                    NRF_LOG_DEBUG("1.%s pin:%d   long press.", __func__, use.pin);
                }
                break;
            case BUTTON_PRESS_1: /* 按键按下一次 */
            case BUTTON_PRESS_2: /* 按键按下两次 */
            case BUTTON_PRESS_3: /* 按键按下三次 */
            case BUTTON_PRESS_4: /* 按键按下四次 */
            case BUTTON_PRESS_5: /* 按键按下五次 */
                if (time_out - control[i].polling >= BUTTON_DOUBLE_PRESS_CNT) /* 软件定时器超时次数 - 按键定时器轮询次数 >= 按键两次按下的次数 */
                {
                    NRF_LOG_DEBUG("2.%s pin:%d   status:%d.", __func__, control[i].pin, control[i].status);
                    use.pin    = control[i].pin; /* 按键应用引脚赋值为按键控制引脚 */
                    use.status = control[i].status; /* 按键应用状态赋值为按键控制状态 */
                    APP_ERROR_CHECK(app_sched_event_put(&use, sizeof(button_use_t), button_event_scheduler_handler)); /* 事件调度 */
                    event_flag         = true;
                    control[i].status  = BUTTON_NO_ACTION; /* 按键控制状态赋值为按键无动作 */
                    control[i].trigger = false; /* 按键触发赋值为按键没触发 */
                    control[i].polling = time_out; /* 按键控制轮询次数赋值为当前定时器超时次数 */
                }
                break;
            case BUTTON_LONG_PRESS: /* 按键长按 */
            case BUTTON_ERROR: /* 按键错误 */
                if (time_out - control[i].polling >= BUTTON_ERROR_WAIT_CNT &&
                    control[i].action == APP_BUTTON_RELEASE) /* 软件定时器超时次数 - 按键定时器轮询次数 >= 按键错误状态等待的次数，并且按键控制动作为释放 */
                {
                    NRF_LOG_DEBUG("3.%s button error.", __func__);
                    event_flag         = true;
                    control[i].status  = BUTTON_NO_ACTION; /* 按键控制状态赋值为按键无动作 */
                    control[i].trigger = false; /* 按键触发赋值为按键没触发 */
                    control[i].polling = time_out; /* 按键控制轮询次数赋值为当前定时器超时次数 */
                }
                break;
            default:
                break;
        }
    }
    if (event_flag == true && button_get_trigger() == false) {
        NRF_LOG_DEBUG("Stop button_polling timer.");
        APP_ERROR_CHECK(app_timer_stop(button_polling));
    }
}


/**@brief 按键初始化函数
 *
 * @param[in]  *cfg - 要初始化的按键配置结构体指针
 * @param[in]  num - 要初始化的按键数量
 * @param[in]  time - 要初始化的按键消抖时间
 *
 * @retval     false - 初始化失败
 * @retval     true - 初始化成功
 *
 * @note None
 * @attention None
 */
bool button_init(button_cfg_t *cfg, uint8_t num, uint16_t time)
{
    if (cfg == NULL || num < 1) /* 按键配置结构体指针为空，或者按键数小于 1 */
    {
        NRF_LOG_ERROR("The button configuration structure pointer is empty, or the number of keys is less than 1 !!!");
        return false;
    }

    configuration = (app_button_cfg_t *)malloc(num * sizeof(app_button_cfg_t)); /* 为按键配置结构体指针（nordic SDK 库自带）分配内存 */
    if (configuration == NULL) {
        NRF_LOG_ERROR("The button configuration struct pointer (nordic SDK library) failed to allocate memory!!!");
        return false;
    }

    control = (button_control_t *)malloc(num * sizeof(button_control_t)); /* 为按键控制结构体指针分配内存 */
    if (control == NULL) {
        NRF_LOG_ERROR("The button control structure pointer failed to allocate memory!!!");
        return false;
    }

    button_num = num; /* 按键数赋值 */
    for (uint8_t i = 0; i < num; i++) /* 初始化按键控制 */
    {
        control[i].pin     = cfg[i].pin;
        control[i].polling = 0;
        control[i].action  = APP_BUTTON_RELEASE;
        control[i].status  = BUTTON_NO_ACTION;
        control[i].trigger = false;

        memcpy(&(configuration[i]), &(cfg[i]), sizeof(cfg[0]));
        configuration[i].button_handler = button_event_handler;
    }
    APP_ERROR_CHECK(app_button_init(configuration, num, APP_TIMER_TICKS(time))); /* 初始化按键（nordic SDK 库自带） */
    APP_ERROR_CHECK(app_button_enable()); /* 使能按键（nordic SDK 库自带） */
    APP_ERROR_CHECK(app_timer_create(&button_polling, APP_TIMER_MODE_REPEATED, button_polling_handler)); /* 创建按键轮询定时器 */
    return true;
}


/**@brief 按键注册回调函数
 *
 * @param[in]  function - 注册的按键回调函数
 * @param[out] None
 *
 * @retval     false - 注册失败
 * @retval     true - 注册成功
 *
 * @note None
 * @attention None
 */
bool button_register_callback(button_callback_handler_t function)
{
    if (function == NULL) {
        NRF_LOG_ERROR("The registered keystroke callback function is empty!!!");
        return false;
    }
    if (button_cb_num < BUTTON_MAX_NUM) /* 注册的按键回调函数不超过按键数 */
    {
        button_callback_handler[button_cb_num++] = function; /* 按键回调函数赋值 */
        return true;
    }
    return false;
}
