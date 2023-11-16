#ifndef __TIMER_COUNT_COMPONENT_H
#define __TIMER_COUNT_COMPONENT_H
#include "nrf.h"

void timer_count_component_init(void);	//定时器计数组件初始化
uint32_t get_timer_count_component_value(void);	//获取组件定时器计数的值
#endif
