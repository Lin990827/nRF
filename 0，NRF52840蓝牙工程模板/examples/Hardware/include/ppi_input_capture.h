#ifndef __PPI_INPUT_CAPTURE_H
#define __PPI_INPUT_CAPTURE_H
#include "nrf.h"
#include "nrf_drv_timer.h"

extern const nrf_drv_timer_t input_capture_timer0;	//������ʱ��0

void ppi_input_capture_init(void);	//PPI֮���벶���ʼ��
#endif
