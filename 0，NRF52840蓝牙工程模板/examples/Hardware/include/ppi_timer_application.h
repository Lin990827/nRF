#ifndef __PPI_TIMER_APPLICATION_H
#define __PPI_TIMER_APPLICATION_H
#include "nrf.h"

void ppi_timer_application_register_init(void);	//PPI֮��ʱ��Ӧ�ã��Ĵ�������ʼ��
uint32_t get_ppi_timer_application_register_value(void);	//��ȡPPI֮��ʱ��Ӧ�ã��Ĵ�����ֵ
void ppi_timer_application_component_init(void);	//PPI֮��ʱ��Ӧ�ã�����⣩��ʼ��
uint32_t get_ppi_timer_application_component_value(void);	//��ȡPPI֮��ʱ��Ӧ�ã�����⣩ֵ
#endif
