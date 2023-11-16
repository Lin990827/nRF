#include "gpiote_event_component.h"
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_gpiote.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

#ifdef BSP_BUTTON_0	//BSP_BUTTON_0�걻����
#define PIN_IN							BSP_BUTTON_0	//�궨������ܽ�
#endif

#ifndef PIN_IN	//PIN_IN��δ������
#error "Please indicate input pin"	//����һ�����������Ϣ
#endif

#ifdef BSP_LED_0	//BSP_LED_0�걻����
#define PIN_OUT							BSP_LED_0	//�궨������ܽ�
#endif

#ifndef PIN_OUT	//PIN_OUT��δ������
#error "Please indicate output pin"	//����һ�����������Ϣ
#endif

/*******************************************************************************
  * @brief		GPIO�¼�������
  * @param		pin - �������¼�������
				action - ���´������¼��Ĳ���
  * @retval		None
  * @attention	ֻ֧�־�̬���ã��Է�ֹ���������Զ��干����Դ
*******************************************************************************/
static void pin_event_handler(nrfx_gpiote_pin_t pin,nrf_gpiote_polarity_t action)
{
	nrf_delay_ms(10);	//����
	if(nrf_gpio_pin_read(PIN_IN) == 0)	//��������Ϊ�͵�ƽ
	{
		nrf_drv_gpiote_out_toggle(PIN_OUT);	//������ŷ�ת
	}
}

/*******************************************************************************
  * @brief		GPIOTE�¼������ʼ������
  * @param		None
  * @retval		None
  * @attention	���ù���:����PIN_IN���ţ����PIN_OUT���ţ�����GPIOTE�����ű仯ʱ�ж�
*******************************************************************************/
void gpiote_event_component_init(void)
{
	ret_code_t error_code;	//�������
	nrf_ppi_channel_t ppi_channel;	//PPIͨ��
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(PIN_OUT, &gpiote_out_config);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(PIN_OUT);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(PIN_IN, &gpiote_in_config, pin_event_handler);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(PIN_IN, true);	//ʹ��GPIOTE���������¼�
	
	error_code = nrf_drv_ppi_init();	//��ʼ��PPIģ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel);	//����δʹ�õ�PPIͨ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel, nrf_drv_gpiote_in_event_addr_get(PIN_IN), 
				 nrf_drv_gpiote_out_task_addr_get(PIN_OUT));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel);	//ʹ��PPIͨ��������PPI
	APP_ERROR_CHECK(error_code);	//������
}
