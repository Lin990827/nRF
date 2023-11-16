#include "uart.h"
#include "pca10056.h"
#include <stdbool.h>
#include "led.h"

/*******************************************************************************
  * @brief		串口初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void uart_init(void)
{
	ret_code_t error_code;	//错误代码
	/*UART通信结构保存外围设备的配置设置结构体*/
	const app_uart_comm_params_t uart_configuration_parameter = {RX_PIN_NUMBER,	//RX引脚编号
																 TX_PIN_NUMBER,	//TX引脚编号
																 RTS_PIN_NUMBER,	//RTS引脚编号，仅在启用流控制时使用
																 CTS_PIN_NUMBER,	//CTS引脚编号，仅在流量控制启用时使用
																 APP_UART_FLOW_CONTROL_DISABLED,	//不使用流量控制（流量控制设置，如果使用流量控制，系统将采用低功耗UART模式，基于CTS信号）
																 false,	//无偶校验（TRUE为偶校验，FALSE为无偶校验）
																 #if defined(UART_PRESENT)
																 NRF_UART_BAUDRATE_460800	//波特率：460800
																 #else
																 NRF_UARTE_BAUDRATE_460800	//波特率：460800
																 #endif
															};
	
	/*在使用FIFO和UART时在单个用户实例中安全初始化UART模块*/
	APP_UART_FIFO_INIT(&uart_configuration_parameter,	//UART通信结构指针
					   UART_RX_BUFFER_SIZE,	//接收缓冲区大小
					   UART_TX_BUFFER_SIZE,	//发送缓冲区大小
					   uart_event_handle,	//事件处理函数
					   APP_IRQ_PRIORITY_LOWEST,	//中断优先级
					   error_code);	//UART初始化函数的返回值
	APP_ERROR_CHECK(error_code);	//错误检查
}

/*******************************************************************************
  * @brief		串口事件处理函数
  * @param		p_app_uart_event - UART事件的指针
  * @retval		None
  * @attention	在app_uart模块中发生事件时，将调用此回调函数以通知应用程序该事件
*******************************************************************************/
void uart_event_handle(app_uart_evt_t * p_app_uart_event)
{
	uint8_t data;	//串口接收的数据
	
	if(p_app_uart_event->evt_type == APP_UART_COMMUNICATION_ERROR)	//UART事件类型为接收过程中发生通信错误，错误存储在app_uart_evt_t.data.error_communication中
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_communication);	//调用错误处理函数
	}
	else if(p_app_uart_event->evt_type == APP_UART_FIFO_ERROR)	//UART事件类型为app_uart模块使用的FIFO模块错误，错误存储在app_uart_evt_t.data.error_code中
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_code);	//调用错误处理函数
	}
	else if(p_app_uart_event->evt_type == APP_UART_DATA_READY)	//UART事件类型为接收数据事件，数据在FIFO中可用，可以使用app_uart_get获取
	{
		app_uart_get(&data);	//获取串口数据
		printf("%c",data);
	}
	else if(p_app_uart_event->evt_type == APP_UART_TX_EMPTY)	//UART事件类型为发送数据完成事件
	{
		led_toggle(LED1);
	}
}

/*******************************************************************************
  * @brief		串口环回测试函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void uart_loop_test(void)
{
	while(1)
	{
		uint8_t  data;	//串口接收数据
		
		while(app_uart_get(&data) != NRF_SUCCESS);	//获取串口数据成功
		while(app_uart_put(data) != NRF_SUCCESS);	//发送串口数据成功
		if(data == 'q' || data == 'Q')
		{
			printf("退出环回测试\r\n");
			while(1)
			{
				//系统运行
			}
		}
	}
}
