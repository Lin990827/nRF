#ifndef __TIMER_H
#define __TIMER_H
#include "nrf.h"

typedef enum
{
	timer0 = 0,	//��ʱ��0ģ�飬����ַΪ0x40008000
	timer1,		//��ʱ��1ģ�飬����ַΪ0x40009000
	timer2,		//��ʱ��2ģ�飬����ַΪ0x4000A000
	timer3,		//��ʱ��3ģ�飬����ַΪ0x4001A000
	timer4,		//��ʱ��4ģ�飬����ַΪ0x4001B000
}Timer_Enum;	/*��ʱ����*/

void timer_timing_init(Timer_Enum timer);	//��ʱ����ʱ��ʼ��
void nrf_timer_delay_ms(Timer_Enum timer,uint32_t delay_ms);	//��ʱ����ʱ
void timer_count_init(Timer_Enum timer);	//��ʱ��������ʼ��
uint32_t get_timer_count_value(Timer_Enum timer);	//��ȡ��ʱ��������ֵ
#endif
