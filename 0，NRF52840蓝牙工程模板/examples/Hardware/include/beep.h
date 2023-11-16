#ifndef __BEEP_H
#define __BEEP_H
#include "nrf.h"
#include "nrf_gpio.h"

#define BEEP							NRF_GPIO_PIN_MAP(0,27)	//宏定义BEEP端口和引脚号

void beep_init(void);	//BEEP初始化函数
void beep_open(uint32_t pin_number);	//BEEP打开函数
void beep_close(uint32_t pin_number);	//BEEP关闭函数
void beep_toggle(uint32_t pin_number);	//BEEP电平翻转函数
#endif
