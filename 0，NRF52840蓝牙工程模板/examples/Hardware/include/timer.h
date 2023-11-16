#ifndef __TIMER_H
#define __TIMER_H
#include "nrf.h"

typedef enum
{
	timer0 = 0,	//定时器0模块，基地址为0x40008000
	timer1,		//定时器1模块，基地址为0x40009000
	timer2,		//定时器2模块，基地址为0x4000A000
	timer3,		//定时器3模块，基地址为0x4001A000
	timer4,		//定时器4模块，基地址为0x4001B000
}Timer_Enum;	/*定时器号*/

void timer_timing_init(Timer_Enum timer);	//定时器定时初始化
void nrf_timer_delay_ms(Timer_Enum timer,uint32_t delay_ms);	//定时器延时
void timer_count_init(Timer_Enum timer);	//定时器计数初始化
uint32_t get_timer_count_value(Timer_Enum timer);	//获取定时器计数的值
#endif
