#ifndef __UART_H
#define __UART_H
#include <stdio.h>
#include "nrf_uart.h"
#include "nrf_uarte.h"
#include "app_uart.h"

#define UART_RX_BUFFER_SIZE				256	//�궨�崮�ڽ��ջ�����
#define UART_TX_BUFFER_SIZE				256	//�궨�崮�ڷ��ͻ�����

void uart_init(void);	//���ڳ�ʼ��
void uart_event_handle(app_uart_evt_t * p_app_uart_event);	//�����¼�����
void uart_loop_test(void);	//���ڻ��ز���
#endif
