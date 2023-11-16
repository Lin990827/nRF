#include "main.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "app_timer.h"
#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "bsp_btn_ble.h"
#include "button.h"
#include "fds.h"
#include "led.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_gpiote.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "sensorsim.h"
#include "user_ble.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define NRF_LOG_MODULE_NAME main
#define NRF_LOG_LEVEL       4 /* 0:Off 1:Error 2:Warning 3:Info 4:Debug */
#define NRF_LOG_INFO_COLOR  4 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#define NRF_LOG_DEBUG_COLOR 8 /* 0:Default 1:Black 2:Red 3:Green 4:Yellow 5:Blue 6:Magenta 7:Cyan 8:White */
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();


APP_TIMER_DEF(sys_timer); /* 创建系统定时器ID */


/**@brief 基本模块初始化函数
 *
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 *
 * @note 上电可以初始化的内容
 * @attention None
 */
static void basic_module_init(void)
{
    nrf_delay_ms(400); /* 等待电源稳定 */

    APP_ERROR_CHECK(NRF_LOG_INIT(app_timer_cnt_get)); /* LOG 初始化 */
    NRF_LOG_DEFAULT_BACKENDS_INIT(); /* 默认 LOG 接口初始化 */
    NRF_LOG_DEBUG("Enter %s .", __func__);

    APP_ERROR_CHECK(app_timer_init()); /* 软件定时器初始化（里面会初始化时钟模块） */

    APP_SCHED_INIT(SCHEDULE_EVENT_MAX_SIZE, SCHEDULE_QUEUE_MAX_SIZE); /* 调度器初始化 */

    APP_ERROR_CHECK(nrf_pwr_mgmt_init()); /* 电源管理初始化 */

    NRF_LOG_DEBUG("Exit %s .", __func__);
}


/**@brief BLE 模块初始化函数
 *
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void ble_module_init(void)
{
    APP_ERROR_CHECK(nrf_drv_clock_init()); /* 初始化时钟 */
    nrf_drv_clock_lfclk_request(NULL); /* 请求LFCLK */
    nrf_drv_clock_hfclk_request(NULL); /* 请求HFCLK */

    ble_stack_init(); /* BLE 协议栈初始化 */
    gap_init(); /* GAP 初始化 */
    gatt_init(); /* GATT 初始化 */
    connect_param_init(); /* 连接参数初始化 */

    service_init(); /* 服务初始化 */
    peer_manager_init(); /* 设备管理初始化 */
}


/**@brief 调度监测函数
 *
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void schedule_monitor(void)
{
    static uint8_t timer    = 0;
    static uint8_t schedule = 0;

    if (app_timer_op_queue_utilization_get() > timer) {
        timer = app_timer_op_queue_utilization_get();
        NRF_LOG_DEBUG("Max operation queue utilization: %d.", timer);
    }
    if (app_sched_queue_utilization_get() > schedule) {
        schedule = app_sched_queue_utilization_get();
        NRF_LOG_DEBUG("Max observed queue utilization: %d.", schedule);
    }
}


/**@brief 事件调度初始化函数
 *
 * @param[in]  *p_context - 要传入的参数
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
static void sys_timer_timeout_handler(void *p_context)
{
    static uint32_t timerout = 0;

    timerout++;
    if (timerout % 50 == 0)
        led_toggle(LED1);
}


/**@brief 主函数
 *
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
int main(void)
{
    basic_module_init();
    ble_module_init();

    advertis_init(); /* 广播初始化 */
    led_init(); /* LED 初始化 */

    button_cfg_t button[] = {
        { BUTTON1, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP },
        { BUTTON2, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP },
        { BUTTON3, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP },
        { BUTTON4, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP },
    }; /* 按键配置 */
    if (button_init(button, sizeof(button) / sizeof(button_cfg_t), 50) == true) /* 按键初始化成功 */
    {
        NRF_LOG_DEBUG("Button initialize successful.");
    }

    APP_ERROR_CHECK(app_timer_create(&sys_timer, APP_TIMER_MODE_REPEATED, sys_timer_timeout_handler)); /* 创建系统定时器 */
    APP_ERROR_CHECK(app_timer_start(sys_timer, APP_TIMER_TICKS(1), NULL)); /* 开启系统定时器，1ms */
    start_advertis(); /* 开启广播 */

    while (1) {
        app_sched_execute(); /* 执行所有计划的事件 */
        schedule_monitor();
        if (NRF_LOG_PROCESS() == false) {
            nrf_pwr_mgmt_run(); /* 运行电源管理功能 */
        }
    }
}
