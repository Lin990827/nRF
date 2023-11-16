#include "gpiote_task_output_component.h"
#include "app_error.h"
#include "nrf_drv_gpiote.h"

/*******************************************************************************
  * @brief		GPIOTE������������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void gpiote_task_output_component_init(void)
{
	ret_code_t error_code;	//�������
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(13, &gpiote_out_config1);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(13);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_LOW;	//��������ʹ��GPIO OUT TASK��״̬�Ӹ߱�Ϊ��
	error_code = nrf_drv_gpiote_out_init(14, &gpiote_out_config2);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(14);	//ʹ��GPIOTE�����������
}
