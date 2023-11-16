#include "gpiote_port_application.h"
#include "boards.h"
#include "nrf_drv_gpiote.h"
#include "led.h"

/*******************************************************************************
  * @brief		GPIO�¼�������
  * @param		pin - �������¼�������
				action - ���´������¼��Ĳ���
  * @retval		None
  * @attention	ֻ֧�־�̬���ã��Է�ֹ���������Զ��干����Դ
*******************************************************************************/
static void pin_event_handler(nrfx_gpiote_pin_t pin,nrf_gpiote_polarity_t action)
{
	if(pin == BUTTON_1)	//�¼���SW1����
		led_toggle(LED_1);	//��תLED1
	else if(pin == BUTTON_2)	//�¼���SW2����
		led_toggle(LED_2);	//��תLED2
	else if(pin == BUTTON_3)	//�¼���SW3����
		led_toggle(LED_3);	//��תLED3
	else if(pin == BUTTON_4)	//�¼���SW4����
		led_toggle(LED_4);	//��תLED4
}

/*******************************************************************************
  * @brief		GPIOTE�˿�Ӧ�ó�ʼ������
  * @param		None
  * @retval		None
  * @attention	���ù���:����PIN_IN���ţ����PIN_OUT���ţ�����GPIOTE�����ű仯ʱ�ж�
*******************************************************************************/
void gpiote_port_application_init(void)
{
	ret_code_t error_code;	//�������
	
	nrf_gpio_cfg_output(LED_1);	//����LED1���
	nrf_gpio_cfg_output(LED_2);	//����LED2���
	nrf_gpio_cfg_output(LED_3);	//����LED3���
	nrf_gpio_cfg_output(LED_4);	//����LED4���
	
	led_close(LED1);	//�ر�LED1
	led_close(LED2);	//�ر�LED2
	led_close(LED3);	//�ر�LED3
	led_close(LED4);	//�ر�LED4
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);	//����������ʹ��GPIO IN��PORT EVENT���ߵ���ת��
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	
	/*SW1*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config, pin_event_handler);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//ʹ��GPIOTE���������¼�
	
	/*SW2*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config, pin_event_handler);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//ʹ��GPIOTE���������¼�
	
	/*SW3*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_2, &gpiote_in_config, pin_event_handler);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_2, true);	//ʹ��GPIOTE���������¼�
	
	/*SW4*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_3, &gpiote_in_config, pin_event_handler);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_3, true);	//ʹ��GPIOTE���������¼�
}
