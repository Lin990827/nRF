#ifndef __USER_BLE_H
#define __USER_BLE_H
#include "app_timer.h"
#include "app_util.h"

/* 协议栈相关宏 */
#define BLE_CONNECTION_CONFIG      1 /* 连接的配置 */
#define BLE_EVENT_HANDLER_PRIORITY 3 /* BLE 事件处理优先级 */

/* GAP 相关宏 */
#define BLE_DEVICE_NAME         "Nordic_Template" /* 设备名字 */
#define MIN_CONNECTION_INTERVAL MSEC_TO_UNITS(100, UNIT_1_25_MS) /* 最小连接间隔，以1.25毫秒为单位 */
#define MAX_CONNECTION_INTERVAL MSEC_TO_UNITS(200, UNIT_1_25_MS) /* 最大连接间隔，以1.25毫秒为单位 */
#define SLAVE_LATENCY           0 /* 从机连接事件数的延迟时间 */
#define CONNECT_SUP_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_1_25_MS) /* 连接监控超时时间，以1.25毫秒为单位 */

/* 广播相关宏 */
#define ADVERTIS_INTERVAL 300 /* 广播间隔（单位625us） */

/* 连接参数相关宏 */
#define FIRST_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(20000) /* 初始化事件(连接或开始通知)到第一次调用sd_ble_gap_conn_param_update的时间 */
#define NEXT_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000) /* 第一次之后调用sd_ble_gap_conn_param_update之间的时间间隔(以时钟周期数为单位) */
#define MAX_CONN_PARAMS_UPDATE_COUNT   3 /* 放弃协商前的尝试次数 */

void ble_stack_init(void); /* BLE 协议栈初始化 */
void gap_init(void); /* GAP 初始化 */
void gatt_init(void); /* GATT 初始化 */
void advertis_init(void); /* 广播初始化 */
void service_init(void); /* 服务初始化 */
void connect_param_init(void); /* 连接参数初始化 */
void peer_manager_init(void); /* 设备管理初始化 */

void start_advertis(void); /* 开启广播 */
void stop_advertis(void); /* 停止广播 */
#endif
