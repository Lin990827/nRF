#ifndef __BEEP_H
#define __BEEP_H
#include "nrf.h"
#include "nrf_gpio.h"

#define BEEP							NRF_GPIO_PIN_MAP(0,27)	//�궨��BEEP�˿ں����ź�

void beep_init(void);	//BEEP��ʼ������
void beep_open(uint32_t pin_number);	//BEEP�򿪺���
void beep_close(uint32_t pin_number);	//BEEP�رպ���
void beep_toggle(uint32_t pin_number);	//BEEP��ƽ��ת����
#endif
