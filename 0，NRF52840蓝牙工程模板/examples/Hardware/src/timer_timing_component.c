#include "timer_timing_component.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_drv_timer.h"
#include "boards.h"

const nrf_drv_timer_t create_timer1 = NRF_DRV_TIMER_INSTANCE(1);	//������ʱ��1

/*******************************************************************************
  * @brief		��ʱ���¼�������
  * @param		event_type - ��ʱ���¼�
				p_context - ���ݵĲ���
  * @retval		None
  * @attention	None
*******************************************************************************/
static void timer_event_handler(nrf_timer_event_t event_type,void *p_context)
{
	static uint32_t i;	//���뺯������
	uint32_t led_num = ((i++) % LEDS_NUMBER);	//ȡ��ǰLED��
	
	switch(event_type)	//�ж϶�ʱ���¼�
	{
		case NRF_TIMER_EVENT_COMPARE0:	//�Ƚ�CC[0]ƥ���¼�
			bsp_board_led_invert(led_num);	//LED��ת
			break;
		default:break;
	}
}

/*******************************************************************************
  * @brief		��ʱ����ʱ�����ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_timing_component_init(void)
{
	uint32_t timer_tick;	//��ʱ���δ����
	ret_code_t error_code;	//�������
	nrf_drv_timer_config_t timer_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ�����ò�����Ĭ�����ò�����
	
	bsp_board_init(BSP_INIT_LEDS);	//���ÿ��������еĵ�
	error_code = nrf_drv_timer_init(&create_timer1, &timer_configuration_parameter, timer_event_handler);	//��ʱ����ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	timer_tick = nrf_drv_timer_ms_to_ticks(&create_timer1, 1000);	//��ʱ��(�Ժ���Ϊ��λ)ת��Ϊ��ʱ���δ�
	nrf_drv_timer_extended_compare(&create_timer1, NRF_TIMER_CC_CHANNEL0, timer_tick, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);	//ʹ�ܶ�ʱ���Ƚ�ͨ����ʹ�ܱȽ��жϣ����ô����ȽϼĴ���CC[n]�Ȳ���
	nrf_drv_timer_enable(&create_timer1);	//ʹ�ܶ�ʱ��
}
