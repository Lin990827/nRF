#ifndef __UART_H
#define __UART_H
#include <stdio.h>
#include "nrf_uart.h"
#include "nrf_uarte.h"
#include "app_uart.h"

#define UART_RX_BUFFER_SIZE				256	//宏定义串口接收缓冲区
#define UART_TX_BUFFER_SIZE				256	//宏定义串口发送缓冲区

void uart_init(void);	//串口初始化
void uart_event_handle(app_uart_evt_t * p_app_uart_event);	//串口事件处理
void uart_loop_test(void);	//串口环回测试
#endif
