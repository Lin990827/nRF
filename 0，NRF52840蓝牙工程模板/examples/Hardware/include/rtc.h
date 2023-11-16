#ifndef __RTC_H
#define __RTC_H
#include "nrf.h"

void start_lfclk_register(void);	//启动内部LFCLK晶振功能（寄存器）
void rtc0_config_register(void);	//RTC0配置（寄存器）
void start_lfclk_component(void);	//启动内部LFCLK晶振功能（组件库）
void rtc0_config_component(void);	//RTC0配置（组件库）
#endif
