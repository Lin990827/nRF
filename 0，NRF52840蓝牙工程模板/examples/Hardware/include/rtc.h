#ifndef __RTC_H
#define __RTC_H
#include "nrf.h"

void start_lfclk_register(void);	//�����ڲ�LFCLK�����ܣ��Ĵ�����
void rtc0_config_register(void);	//RTC0���ã��Ĵ�����
void start_lfclk_component(void);	//�����ڲ�LFCLK�����ܣ�����⣩
void rtc0_config_component(void);	//RTC0���ã�����⣩
#endif
