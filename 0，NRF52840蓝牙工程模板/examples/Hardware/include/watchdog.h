#ifndef __WATCHDOG_H
#define __WATCHDOG_H
#include "nrf_drv_wdt.h"

extern nrf_drv_wdt_channel_id wdt_channel;	//���Ź�ͨ��

void watchdog_lfclk_register(void);	//���Ź�ʱ�����ã��Ĵ�����
void watchdog_config_register(void);	//���Ź����ã��Ĵ�����
void watchdog_lfclk_component(void);	//���Ź��ڲ�LFCLK��������⣩
void watchdog_config_component(void);	//���Ź����ã�����⣩
#endif
