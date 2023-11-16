#ifndef __RNG_H
#define __RNG_H
#include "nrf_drv_rng.h"

#define RNG_CONFIGURATION_PARAMETER		1	//随机数配置参数选择（1：自定义参数   0：默认参数）
#define RNG_BUFFER_SIZE					16	//随机数缓冲区大小

void rng_init(void);	//随机数初始化
uint8_t get_random_number(uint8_t *buffer,uint8_t len);	//获取随机数
#endif
