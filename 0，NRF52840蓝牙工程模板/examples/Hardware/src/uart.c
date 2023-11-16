#include "uart.h"
#include "pca10056.h"
#include <stdbool.h>
#include "led.h"

/*******************************************************************************
  * @brief		���ڳ�ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void uart_init(void)
{
	ret_code_t error_code;	//�������
	/*UARTͨ�Žṹ������Χ�豸���������ýṹ��*/
	const app_uart_comm_params_t uart_configuration_parameter = {RX_PIN_NUMBER,	//RX���ű��
																 TX_PIN_NUMBER,	//TX���ű��
																 RTS_PIN_NUMBER,	//RTS���ű�ţ���������������ʱʹ��
																 CTS_PIN_NUMBER,	//CTS���ű�ţ�����������������ʱʹ��
																 APP_UART_FLOW_CONTROL_DISABLED,	//��ʹ���������ƣ������������ã����ʹ���������ƣ�ϵͳ�����õ͹���UARTģʽ������CTS�źţ�
																 false,	//��żУ�飨TRUEΪżУ�飬FALSEΪ��żУ�飩
																 #if defined(UART_PRESENT)
																 NRF_UART_BAUDRATE_460800	//�����ʣ�460800
																 #else
																 NRF_UARTE_BAUDRATE_460800	//�����ʣ�460800
																 #endif
															};
	
	/*��ʹ��FIFO��UARTʱ�ڵ����û�ʵ���а�ȫ��ʼ��UARTģ��*/
	APP_UART_FIFO_INIT(&uart_configuration_parameter,	//UARTͨ�Žṹָ��
					   UART_RX_BUFFER_SIZE,	//���ջ�������С
					   UART_TX_BUFFER_SIZE,	//���ͻ�������С
					   uart_event_handle,	//�¼�������
					   APP_IRQ_PRIORITY_LOWEST,	//�ж����ȼ�
					   error_code);	//UART��ʼ�������ķ���ֵ
	APP_ERROR_CHECK(error_code);	//������
}

/*******************************************************************************
  * @brief		�����¼�������
  * @param		p_app_uart_event - UART�¼���ָ��
  * @retval		None
  * @attention	��app_uartģ���з����¼�ʱ�������ô˻ص�������֪ͨӦ�ó�����¼�
*******************************************************************************/
void uart_event_handle(app_uart_evt_t * p_app_uart_event)
{
	uint8_t data;	//���ڽ��յ�����
	
	if(p_app_uart_event->evt_type == APP_UART_COMMUNICATION_ERROR)	//UART�¼�����Ϊ���չ����з���ͨ�Ŵ��󣬴���洢��app_uart_evt_t.data.error_communication��
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_communication);	//���ô�������
	}
	else if(p_app_uart_event->evt_type == APP_UART_FIFO_ERROR)	//UART�¼�����Ϊapp_uartģ��ʹ�õ�FIFOģ����󣬴���洢��app_uart_evt_t.data.error_code��
	{
		APP_ERROR_HANDLER(p_app_uart_event->data.error_code);	//���ô�������
	}
	else if(p_app_uart_event->evt_type == APP_UART_DATA_READY)	//UART�¼�����Ϊ���������¼���������FIFO�п��ã�����ʹ��app_uart_get��ȡ
	{
		app_uart_get(&data);	//��ȡ��������
		printf("%c",data);
	}
	else if(p_app_uart_event->evt_type == APP_UART_TX_EMPTY)	//UART�¼�����Ϊ������������¼�
	{
		led_toggle(LED1);
	}
}

/*******************************************************************************
  * @brief		���ڻ��ز��Ժ���
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void uart_loop_test(void)
{
	while(1)
	{
		uint8_t  data;	//���ڽ�������
		
		while(app_uart_get(&data) != NRF_SUCCESS);	//��ȡ�������ݳɹ�
		while(app_uart_put(data) != NRF_SUCCESS);	//���ʹ������ݳɹ�
		if(data == 'q' || data == 'Q')
		{
			printf("�˳����ز���\r\n");
			while(1)
			{
				//ϵͳ����
			}
		}
	}
}
