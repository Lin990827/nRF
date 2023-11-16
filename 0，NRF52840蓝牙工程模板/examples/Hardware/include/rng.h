#ifndef __RNG_H
#define __RNG_H
#include "nrf_drv_rng.h"

#define RNG_CONFIGURATION_PARAMETER		1	//��������ò���ѡ��1���Զ������   0��Ĭ�ϲ�����
#define RNG_BUFFER_SIZE					16	//�������������С

void rng_init(void);	//�������ʼ��
uint8_t get_random_number(uint8_t *buffer,uint8_t len);	//��ȡ�����
#endif
