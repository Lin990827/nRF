#include "ppi_gpiote_application.h"
#include "boards.h"
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
  * @brief		PPI֮GPIOTEӦ�ã��Ĵ�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_gpiote_application_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//����������������
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�͵�ƽ���¼�ģʽ�£��ܽ��½��ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPIͨ��0�¼��յ�
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPIͨ��0�����յ�
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos);	//ͨ��ʹ��
}

/*******************************************************************************
  * @brief		PPI֮GPIOTEӦ�ã�����⣩��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_gpiote_application_component_init(void)
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
	error_code = nrf_drv_gpiote_in_init(PIN_IN, &gpiote_in_config, NULL);	//NRF��ʼ��GPIOTE��������
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
