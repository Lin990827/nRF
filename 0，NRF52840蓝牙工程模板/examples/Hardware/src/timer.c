#include "timer.h"
#include "uart.h"

/*******************************************************************************
  * @brief		��ʱ����ʱ��ʼ������
  * @param		timer - Ҫ��ʼ���Ķ�ʱ����
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_timing_init(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//��ʱ���ṹ��ָ��
	
	//����16MHz����
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)	//�ȴ��ⲿ��������
	{
		//�����κδ����ȴ��ɼ����
	}
	switch(timer)	//�жϳ�ʼ���Ķ�ʱ����
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//��ʱ��0ģ��
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//��ʱ��1ģ��
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//��ʱ��2ģ��
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//��ʱ��3ģ��
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//��ʱ��4ģ��
		default:		nrf_timer = 0;			break;	//û�ж�ʱ��ģ��
	}
	if(nrf_timer == 0)
	{
		//�����κδ���
	}
	nrf_timer->MODE = TIMER_MODE_MODE_Timer;	//��ʱ��ģʽ
	nrf_timer->PRESCALER = 10;	//��ʱ��Ԥ��Ƶֵ��10-1   ����31250Hz��ʱ��Ƶ��=> 1 tick = 32 us
	nrf_timer->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//16λ��ʱ��
}

/*******************************************************************************
  * @brief		��ʱ����ʱ����
  * @param		timer - Ҫ��ʼ���Ķ�ʱ����
				delay_ms - ��ʱ��msʱ��
  * @retval		None
  * @attention	None
*******************************************************************************/
void nrf_timer_delay_ms(Timer_Enum timer,uint32_t delay_ms)
{
	NRF_TIMER_Type *nrf_timer;	//��ʱ���ṹ��ָ��
	
	switch(timer)	//�жϳ�ʼ���Ķ�ʱ����
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//��ʱ��0ģ��
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//��ʱ��1ģ��
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//��ʱ��2ģ��
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//��ʱ��3ģ��
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//��ʱ��4ģ��
		default:		nrf_timer = 0;			break;	//û�ж�ʱ��ģ��
	}
	nrf_timer->TASKS_CLEAR = 1;	//�嶨ʱ��
	nrf_timer->CC[0] = delay_ms * 31;	//32����λ������31.25�õ�����
	nrf_timer->CC[0] += delay_ms / 4;	//���ñȽϼĴ���ֵ
	nrf_timer->TASKS_START = 1;	//������ʱ��
	while(nrf_timer->EVENTS_COMPARE[0] == 0)	//�ȴ��Ƚ�CC[0]ƥ���¼���������Ƚ�CC[0]ƥ���¼���1
	{
		//�����κδ���
	}
	nrf_timer->EVENTS_COMPARE[0] = 0;	//�Ƚ�CC[0]ƥ���¼���0
	nrf_timer->TASKS_STOP = 1;	//ֹͣ��ʱ��
}

/*******************************************************************************
  * @brief		��ʱ��������ʼ������
  * @param		timer - Ҫ��ʼ���Ķ�ʱ����
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_count_init(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//��ʱ���ṹ��ָ��
	
	switch(timer)	//�жϳ�ʼ���Ķ�ʱ����
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//��ʱ��0ģ��
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//��ʱ��1ģ��
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//��ʱ��2ģ��
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//��ʱ��3ģ��
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//��ʱ��4ģ��
		default:		nrf_timer = 0;			break;	//û�ж�ʱ��ģ��
	}
	if(nrf_timer == 0)
	{
		//�����κδ���
	}
	nrf_timer->MODE = TIMER_MODE_MODE_Counter;	//������ģʽ
	nrf_timer->BITMODE = TIMER_BITMODE_BITMODE_24Bit;	//24λ��ʱ��
	nrf_timer->TASKS_START = 1;	//������ʱ��
}

/*******************************************************************************
  * @brief		��ȡ��ʱ��������ֵ����
  * @param		timer - Ҫ��ʼ���Ķ�ʱ����
  * @retval		��ʱ��������ֵ
  * @attention	�ڲ�������Counter�Ĵ�����ֵ�洢��CC[0]
*******************************************************************************/
uint32_t get_timer_count_value(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//��ʱ���ṹ��ָ��
	
	switch(timer)	//�жϳ�ʼ���Ķ�ʱ����
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//��ʱ��0ģ��
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//��ʱ��1ģ��
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//��ʱ��2ģ��
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//��ʱ��3ģ��
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//��ʱ��4ģ��
		default:		nrf_timer = 0;			break;	//û�ж�ʱ��ģ��
	}
	if(nrf_timer == 0)
	{
		//�����κδ���
	}
	nrf_timer->TASKS_CAPTURE[0] = 1;	//��Counterֵ����CC[0]�Ĵ���
	nrf_timer->TASKS_COUNT = 1;	//���������������޼�����ģʽ��
	return nrf_timer->CC[0];
}
