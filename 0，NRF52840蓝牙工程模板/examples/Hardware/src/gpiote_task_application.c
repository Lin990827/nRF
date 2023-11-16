#include "gpiote_task_application.h"
#include "nrf.h"

/*******************************************************************************
  * @brief		GPIOTE����Ӧ�ó�ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void gpiote_task_application_init(void)
{
	NVIC_EnableIRQ(GPIOTE_IRQn);	//ʹ���ⲿ�ж�
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (13 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (14 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
}
