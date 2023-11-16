#ifndef __PPI_GROUP_APPLICATION_H
#define __PPI_GROUP_APPLICATION_H
#include "nrf_drv_ppi.h"

extern nrf_ppi_channel_group_t ppi_channel_group;	//PPI通道分组

void ppi_group_application_register_init(void);	//PPI之group分组应用（寄存器）初始化
void ppi_group_application_component_init(void);	//PPI之group分组应用（组件库）初始化
void ppi_event_manager_group_register_init(void);	//PPI事件管理group分组（寄存器）初始化
void ppi_event_manager_group_component_init(void);	//PPI事件管理group分组（组件库）初始化
#endif
