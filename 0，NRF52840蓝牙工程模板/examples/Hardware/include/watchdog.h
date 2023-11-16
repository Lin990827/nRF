#ifndef __WATCHDOG_H
#define __WATCHDOG_H
#include "nrf_drv_wdt.h"

extern nrf_drv_wdt_channel_id wdt_channel;	//看门狗通道

void watchdog_lfclk_register(void);	//看门狗时钟配置（寄存器）
void watchdog_config_register(void);	//看门狗配置（寄存器）
void watchdog_lfclk_component(void);	//看门狗内部LFCLK晶振（组件库）
void watchdog_config_component(void);	//看门狗配置（组件库）
#endif
