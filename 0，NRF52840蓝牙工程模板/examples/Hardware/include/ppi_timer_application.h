#ifndef __PPI_TIMER_APPLICATION_H
#define __PPI_TIMER_APPLICATION_H
#include "nrf.h"

void ppi_timer_application_register_init(void);	//PPI之定时器应用（寄存器）初始化
uint32_t get_ppi_timer_application_register_value(void);	//获取PPI之定时器应用（寄存器）值
void ppi_timer_application_component_init(void);	//PPI之定时器应用（组件库）初始化
uint32_t get_ppi_timer_application_component_value(void);	//获取PPI之定时器应用（组件库）值
#endif
