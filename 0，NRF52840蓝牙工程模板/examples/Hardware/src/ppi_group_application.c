#include "ppi_group_application.h"
#include "boards.h"
#include "nrf_drv_gpiote.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

/*******************************************************************************
  * @brief		PPI֮group����Ӧ�ã��Ĵ�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_group_application_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//����������������
	nrf_gpio_cfg_input(BSP_BUTTON_1, NRF_GPIO_PIN_PULLUP);	//����������������
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_1 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_1 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPIͨ��0�¼��յ�
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[2]);	//PPIͨ��0�����յ�
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[1]);	//PPIͨ��1�¼��յ�
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[3]);	//PPIͨ��1�����յ�
	NRF_PPI->CHG[0] = 0x03;	//��PPIͨ��0��1
}

nrf_ppi_channel_group_t ppi_channel_group;	//PPIͨ������

/*******************************************************************************
  * @brief		PPI֮group����Ӧ�ã�����⣩��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_group_application_component_init(void)
{
	ret_code_t error_code;	//�������
	nrf_ppi_channel_t ppi_channel1;	//PPIͨ��1
	nrf_ppi_channel_t ppi_channel2;	//PPIͨ��2
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_0, &gpiote_out_config1);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_0);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_1, &gpiote_out_config2);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_1);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_in_config_t gpiote_in_config1 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config1.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config1, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//ʹ��GPIOTE���������¼�
	
	nrf_drv_gpiote_in_config_t gpiote_in_config2 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config2.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config2, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//ʹ��GPIOTE���������¼�
	
	error_code = nrf_drv_ppi_init();	//��ʼ��PPIģ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//����δʹ�õ�PPIͨ��1
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_0), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_0));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//����δʹ�õ�PPIͨ��2
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_1), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_1));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_group_alloc(&ppi_channel_group);	//����δ��ʹ�õ�PPIͨ������
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel1, ppi_channel_group);	//��ָ����PPIͨ��������ͨ������
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel2, ppi_channel_group);	//��ָ����PPIͨ��������ͨ������
	APP_ERROR_CHECK(error_code);	//������
}

/*******************************************************************************
  * @brief		PPI�¼�����group���飨�Ĵ�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_event_manager_group_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//����������������
	nrf_gpio_cfg_input(BSP_BUTTON_1, NRF_GPIO_PIN_PULLUP);	//����������������
	nrf_gpio_cfg_input(BSP_BUTTON_2, NRF_GPIO_PIN_PULLUP);	//����������������
	nrf_gpio_cfg_input(BSP_BUTTON_3, NRF_GPIO_PIN_PULLUP);	//����������������
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_1 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_1 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_GPIOTE->CONFIG[4] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_2 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]�¼�
	
	NRF_GPIOTE->CONFIG[5] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ�ߵ�ƽ���¼�ģʽ�£��ܽ������ص�ʱ�����IN[n]�����¼�
							| (BSP_BUTTON_3 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//�¼�ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ����ģʽ����POLARITY�����ʲô����´���IN[n]
	
	NRF_GPIOTE->CONFIG[6] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_2 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_GPIOTE->CONFIG[7] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (BSP_LED_3 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_PPI->CH[2].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[4]);	//PPIͨ��2�¼��յ�
	NRF_PPI->CH[2].TEP = (uint32_t)(&NRF_PPI->TASKS_CHG[0].EN);	//PPIͨ��2�����յ�
	NRF_PPI->FORK[2].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[6]);	//PPIͨ��2�������յ�
	NRF_PPI->CH[3].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[5]);	//PPIͨ��3�¼��յ�
	NRF_PPI->CH[3].TEP = (uint32_t)(&NRF_PPI->TASKS_CHG[0].DIS);	//PPIͨ��3�����յ�
	NRF_PPI->FORK[3].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[7]);	//PPIͨ��3�������յ�
	NRF_PPI->CHEN = (PPI_CHEN_CH2_Enabled << PPI_CHEN_CH2_Pos) | (PPI_CHEN_CH3_Enabled << PPI_CHEN_CH3_Pos);	//ͨ��ʹ��
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPIͨ��0�¼��յ�
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[2]);	//PPIͨ��0�����յ�
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[1]);	//PPIͨ��1�¼��յ�
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[3]);	//PPIͨ��1�����յ�
	NRF_PPI->CHG[0] = 0x03;	//��PPIͨ��0��1
}

/*******************************************************************************
  * @brief		PPI�¼�����group���飨����⣩��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_event_manager_group_component_init(void)
{
	ret_code_t error_code;	//�������
	nrf_ppi_channel_t ppi_channel1;	//PPIͨ��1
	nrf_ppi_channel_t ppi_channel2;	//PPIͨ��2
	nrf_ppi_channel_t ppi_channel3;	//PPIͨ��3
	nrf_ppi_channel_t ppi_channel4;	//PPIͨ��4
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_0, &gpiote_out_config1);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_0);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_1, &gpiote_out_config2);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_1);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_in_config_t gpiote_in_config1 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config1.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config1, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//ʹ��GPIOTE���������¼�
	
	nrf_drv_gpiote_in_config_t gpiote_in_config2 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config2.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config2, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//ʹ��GPIOTE���������¼�
	
	nrf_drv_gpiote_out_config_t gpiote_out_config3 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_2, &gpiote_out_config3);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_2);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_out_config_t gpiote_out_config4 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//��������ʹ��GPIO OUT TASK�л�����״̬
	error_code = nrf_drv_gpiote_out_init(BSP_LED_3, &gpiote_out_config4);	//NRF��ʼ��GPIOTE�������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_out_task_enable(BSP_LED_3);	//ʹ��GPIOTE�����������
	
	nrf_drv_gpiote_in_config_t gpiote_in_config3 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config3.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_2, &gpiote_in_config3, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_2, true);	//ʹ��GPIOTE���������¼�
	
	nrf_drv_gpiote_in_config_t gpiote_in_config4 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��κθ���
	gpiote_in_config4.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_3, &gpiote_in_config4, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_3, true);	//ʹ��GPIOTE���������¼�
	
	error_code = nrf_drv_ppi_init();	//��ʼ��PPIģ��
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel3);	//����δʹ�õ�PPIͨ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel3, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_2), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_2));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel3, nrf_drv_gpiote_out_task_addr_get(BSP_LED_2));	//���������Fork�˵㵽PPIͨ��֮��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel3, (uint32_t)nrf_ppi_task_group_enable_address_get(ppi_channel_group));	//���������Fork�˵㵽PPIͨ��֮��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel3);	//ʹ��PPIͨ��������PPI
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel4);	//����δʹ�õ�PPIͨ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel4, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_3), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_3));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel4, (uint32_t)nrf_ppi_task_group_disable_address_get(ppi_channel_group));	//���������Fork�˵㵽PPIͨ��֮��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel4);	//ʹ��PPIͨ��������PPI
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//����δʹ�õ�PPIͨ��1
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_0), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_0));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//����δʹ�õ�PPIͨ��2
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_1), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_1));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_group_alloc(&ppi_channel_group);	//����δ��ʹ�õ�PPIͨ������
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel1, ppi_channel_group);	//��ָ����PPIͨ��������ͨ������
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel2, ppi_channel_group);	//��ָ����PPIͨ��������ͨ������
	APP_ERROR_CHECK(error_code);	//������
}
