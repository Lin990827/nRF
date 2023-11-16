#include "exti.h"
#include "key.h"
#include "nrf_delay.h"
#include "led.h"

/*******************************************************************************
  * @brief		�жϳ�ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void exti_init(void)
{
	nrf_gpio_cfg_input(SW1, NRF_GPIO_PIN_PULLUP);	//����SW1 IO�˿�Ϊ��������״̬
	nrf_gpio_cfg_input(SW2, NRF_GPIO_PIN_PULLUP);	//����SW2 IO�˿�Ϊ��������״̬
	nrf_gpio_cfg_input(SW3, NRF_GPIO_PIN_PULLUP);	//����SW3 IO�˿�Ϊ��������״̬
	nrf_gpio_cfg_input(SW4, NRF_GPIO_PIN_PULLUP);	//����SW4 IO�˿�Ϊ��������״̬
	nrf_gpio_cfg_input(TOUCH, NRF_GPIO_PIN_PULLUP);	//����TOUCH IO�˿�Ϊ��������״̬
	
	NVIC_EnableIRQ(GPIOTE_IRQn);	//ʹ���ⲿ�ж�
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�͵�ƽ���¼�ģʽ�£��ܽ��½��ص�ʱ�����IN[n]�����¼�
							| (11 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;	//ʹ��IN[0]�¼��ж�
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�͵�ƽ���¼�ģʽ�£��ܽ��½��ص�ʱ�����IN[n]�����¼�
							| (12 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN1_Set << GPIOTE_INTENSET_IN1_Pos;	//ʹ��IN[1]�¼��ж�
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�͵�ƽ���¼�ģʽ�£��ܽ��½��ص�ʱ�����IN[n]�����¼�
							| (24 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN2_Set << GPIOTE_INTENSET_IN2_Pos;	//ʹ��IN[2]�¼��ж�
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�͵�ƽ���¼�ģʽ�£��ܽ��½��ص�ʱ�����IN[n]�����¼�
							| (25 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN3_Set << GPIOTE_INTENSET_IN3_Pos;	//ʹ��IN[3]�¼��ж�
	
	NRF_GPIOTE->CONFIG[4] = (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (26 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN4_Set << GPIOTE_INTENSET_IN4_Pos;	//ʹ��IN[4]�¼��ж�
	
}

#if 0
/*******************************************************************************
  * @brief		GPIO�¼��жϷ�����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void GPIOTE_IRQHandler(void)
{
	if((NRF_GPIOTE->EVENTS_IN[0] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))	//��CONFIG[0].PSEL��ָ�����������ɵ��¼�����IN[0]�¼��ж���ʹ��
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0;	//�����CONFIG[0].PSEL��ָ�����������ɵ��¼�
		nrf_delay_ms(20);	//����
		led_toggle(LED1);
	}
	if((NRF_GPIOTE->EVENTS_IN[1] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN1_Msk))	//��CONFIG[1].PSEL��ָ�����������ɵ��¼�����IN[1]�¼��ж���ʹ��
	{
		NRF_GPIOTE->EVENTS_IN[1] = 0;	//�����CONFIG[1].PSEL��ָ�����������ɵ��¼�
		nrf_delay_ms(20);	//����
		led_toggle(LED2);
	}
	if((NRF_GPIOTE->EVENTS_IN[2] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN2_Msk))	//��CONFIG[2].PSEL��ָ�����������ɵ��¼�����IN[2]�¼��ж���ʹ��
	{
		NRF_GPIOTE->EVENTS_IN[2] = 0;	//�����CONFIG[2].PSEL��ָ�����������ɵ��¼�
		nrf_delay_ms(20);	//����
		led_toggle(LED3);
	}
	if((NRF_GPIOTE->EVENTS_IN[3] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN3_Msk))	//��CONFIG[3].PSEL��ָ�����������ɵ��¼�����IN[3]�¼��ж���ʹ��
	{
		NRF_GPIOTE->EVENTS_IN[3] = 0;	//�����CONFIG[3].PSEL��ָ�����������ɵ��¼�
		nrf_delay_ms(20);	//����
		led_toggle(LED4);
	}
	if((NRF_GPIOTE->EVENTS_IN[4] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN4_Msk))	//��CONFIG[4].PSEL��ָ�����������ɵ��¼�����IN[4]�¼��ж���ʹ��
	{
		NRF_GPIOTE->EVENTS_IN[4] = 0;	//�����CONFIG[4].PSEL��ָ�����������ɵ��¼�
		nrf_delay_ms(20);	//����
		led_toggle(LED1);
		led_toggle(LED2);
		led_toggle(LED3);
		led_toggle(LED4);
	}
}
#endif
