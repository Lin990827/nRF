#ifndef __CORE_TEMP_H
#define __CORE_TEMP_H
#include "nrf_temp.h"

void core_temp_init(void);	//内部温度初始化
float read_core_temp(void);	//读取内部温度
#endif
