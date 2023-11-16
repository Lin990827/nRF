#include "beep.h"

/*******************************************************************************
  * @brief		BEEP��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_init(void)
{
	nrf_gpio_cfg_output(BEEP);	//����BEEP IO�˿�Ϊ���״̬
}

/*******************************************************************************
  * @brief		BEEP�򿪺���
  * @param		pin_number - BEEP�˿ں����ź�
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_open(uint32_t pin_number)
{
	nrf_gpio_pin_set(pin_number);	//IO�˿ں����ź�����ߵ�ƽ
}

/*******************************************************************************
  * @brief		BEEP�رպ���
  * @param		pin_number - BEEP�˿ں����ź�
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_close(uint32_t pin_number)
{
	nrf_gpio_pin_clear(pin_number);	//IO�˿ں����ź�����͵�ƽ
}

/*******************************************************************************
  * @brief		BEEP��ƽ��ת����
  * @param		pin_number - BEEP�˿ں����ź�
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_toggle(uint32_t pin_number)
{
	nrf_gpio_pin_toggle(pin_number);	//IO�˿ں����źŵ�ƽ��ת��������ߵ�ƽ��������͵�ƽ��
}
