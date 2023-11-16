#include "timer_count_component.h"
#include "nrf_drv_timer.h"

const nrf_drv_timer_t create_timer0 = NRF_DRV_TIMER_INSTANCE(0);	//������ʱ��0

/*******************************************************************************
  * @brief		��ʱ���¼�������
  * @param		event_type - ��ʱ���¼�
				p_context - ���ݵĲ���
  * @retval		None
  * @attention	None
*******************************************************************************/
static void timer_event_handler(nrf_timer_event_t event_type,void *p_context)
{
}

/*******************************************************************************
  * @brief		��ʱ�����������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_count_component_init(void)
{
	ret_code_t error_code;	//�������
	nrf_drv_timer_config_t timer_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ�����ò�����Ĭ�����ò�����
	
	timer_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//������ģʽ
	error_code = nrf_drv_timer_init(&create_timer0, &timer_configuration_parameter, timer_event_handler);	//��ʱ����ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_timer_enable(&create_timer0);	//ʹ�ܶ�ʱ��
}

/*******************************************************************************
  * @brief		��ȡ�����ʱ��������ֵ����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
uint32_t get_timer_count_component_value(void)
{
	nrf_drv_timer_increment(&create_timer0);	//����һ�μ���
	return nrf_drv_timer_capture(&create_timer0, NRF_TIMER_CC_CHANNEL0);	//�����ʱ��ֵ
}
