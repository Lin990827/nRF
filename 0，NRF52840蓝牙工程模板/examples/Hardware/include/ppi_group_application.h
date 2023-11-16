#ifndef __PPI_GROUP_APPLICATION_H
#define __PPI_GROUP_APPLICATION_H
#include "nrf_drv_ppi.h"

extern nrf_ppi_channel_group_t ppi_channel_group;	//PPIͨ������

void ppi_group_application_register_init(void);	//PPI֮group����Ӧ�ã��Ĵ�������ʼ��
void ppi_group_application_component_init(void);	//PPI֮group����Ӧ�ã�����⣩��ʼ��
void ppi_event_manager_group_register_init(void);	//PPI�¼�����group���飨�Ĵ�������ʼ��
void ppi_event_manager_group_component_init(void);	//PPI�¼�����group���飨����⣩��ʼ��
#endif
