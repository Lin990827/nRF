#include "ppi_timer_application.h"
#include "boards.h"
#include "nrf_drv_timer.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);	//������ʱ��0
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(1);	//������ʱ��1
const nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(2);	//������ʱ��2

/*******************************************************************************
  * @brief		PPI֮��ʱ��Ӧ�ã��Ĵ�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_timer_application_register_init(void)
{
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Counter;	//��ʱ��0��������ģʽ
	NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//��ʱ��0��16λ��ʱ��
	NRF_TIMER0->PRESCALER = 9;	//��ʱ��0��Ԥ��Ƶֵ9-1
	
	/* SysClk = 16 Mhz   PRESCALER = 9 */
	/* ����ʱ�� = 0xFFFF / (SysClk / 2 ^ PRESCALER) = 65535 / 31250 = 2.097 sec */
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;	//��ʱ��1����ʱ��ģʽ
	NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//��ʱ��1��16λ��ʱ��
	NRF_TIMER1->PRESCALER = 9;	//��ʱ��1��Ԥ��Ƶֵ9-1
	NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);	//��ʱ��1����ݷ�ʽע��COMPARE[i]�¼���CLEAR����֮��Ŀ�ݷ�ʽ
	NRF_TIMER1->CC[0] = 0xFFFFUL;	//��ʱ��1�����ñȽϼĴ���
	
	/* SysClk = 16 Mhz   PRESCALER = 9 */
	/* ����ʱ�� = 0xFFFF / (SysClk / 2 ^ PRESCALER) = 32767 / 31250 = 1.048 sec */
	NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;	//��ʱ��2����ʱ��ģʽ
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//��ʱ��2��16λ��ʱ��
	NRF_TIMER2->PRESCALER = 9;	//��ʱ��2��Ԥ��Ƶֵ9-1
	NRF_TIMER2->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);	//��ʱ��2����ݷ�ʽע��COMPARE[i]�¼���CLEAR����֮��Ŀ�ݷ�ʽ
	NRF_TIMER2->CC[0] = 0x7FFFUL;	//��ʱ��2�����ñȽϼĴ���
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_TIMER1->EVENTS_COMPARE[0]);	//PPIͨ��0�¼��յ㣨��ʱ��1�Ƚ��¼���
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_TIMER0->TASKS_STOP);	//PPIͨ��0�����յ㣨��ʱ��0ֹͣ����
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_TIMER2->EVENTS_COMPARE[0]);	//PPIͨ��1�¼��յ㣨��ʱ��2�Ƚ��¼���
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_TIMER0->TASKS_START);	//PPIͨ��1�����յ㣨��ʱ��0��ʼ����
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos) | (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos);	//ͨ��ʹ��
	
	NRF_TIMER1->TASKS_START = 1;	//������ʱ��1
	NRF_TIMER2->TASKS_START = 1;	//������ʱ��2
}

/*******************************************************************************
  * @brief		��ȡPPI֮��ʱ��Ӧ�ã��Ĵ�����ֵ����
  * @param		None
  * @retval		��ʱ��������ֵ
  * @attention	�ڲ�������Counter�Ĵ�����ֵ�洢��CC[0]
*******************************************************************************/
uint32_t get_ppi_timer_application_register_value(void)
{
	NRF_TIMER0->TASKS_CAPTURE[0] = 1;	//��Counterֵ����CC[0]�Ĵ���
	NRF_TIMER0->TASKS_COUNT = 1;	//���������������޼�����ģʽ��
	return NRF_TIMER0->CC[0];
}

/*******************************************************************************
  * @brief		PPI֮��ʱ��Ӧ�ã�����⣩��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_timer_application_component_init(void)
{
	ret_code_t error_code;	//�������
	nrf_drv_timer_config_t timer0_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ��0���ò�����Ĭ�����ò�����
	nrf_drv_timer_config_t timer1_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ��1���ò�����Ĭ�����ò�����
	nrf_drv_timer_config_t timer2_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ��2���ò�����Ĭ�����ò�����
	nrf_ppi_channel_t ppi_channel1;	//PPIͨ��1
	nrf_ppi_channel_t ppi_channel2;	//PPIͨ��2
	
	timer0_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//������ģʽ
	error_code = nrf_drv_timer_init(&timer0, &timer0_configuration_parameter, NULL);	//��ʱ��0��ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	
	error_code = nrf_drv_timer_init(&timer1, &timer1_configuration_parameter, NULL);	//��ʱ��1��ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, 0xFFFFUL, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);	//ʹ�ܶ�ʱ��1�Ƚ�ͨ����ʹ�ܱȽ��жϣ����ô����ȽϼĴ���CC[n]�Ȳ���
	
	error_code = nrf_drv_timer_init(&timer2, &timer2_configuration_parameter, NULL);	//��ʱ��2��ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_timer_extended_compare(&timer2, NRF_TIMER_CC_CHANNEL0, 0x7FFFUL, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);	//ʹ�ܶ�ʱ��2�Ƚ�ͨ����ʹ�ܱȽ��жϣ����ô����ȽϼĴ���CC[n]�Ȳ���
	
	error_code = nrf_drv_ppi_init();	//��ʼ��PPIģ��
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//����δʹ�õ�PPIͨ��1
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, 
				 nrf_drv_timer_event_address_get(&timer1, NRF_TIMER_EVENT_COMPARE0), 
				 nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_STOP));	//����EEP�¼��յ㣨��ʱ��1�Ƚ��¼�����TEP�����յ㣨��ʱ��0ֹͣ����
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel1);	//ʹ��PPIͨ��1������PPI
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//����δʹ�õ�PPIͨ��2
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, 
				 nrf_drv_timer_event_address_get(&timer2, NRF_TIMER_EVENT_COMPARE0), 
				 nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_START));	//����EEP�¼��յ㣨��ʱ��1�Ƚ��¼�����TEP�����յ㣨��ʱ��2��ʼ����
	APP_ERROR_CHECK(error_code);	//������
	
	error_code = nrf_drv_ppi_channel_enable(ppi_channel1);	//ʹ��PPIͨ��1������PPI
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel2);	//ʹ��PPIͨ��2������PPI
	APP_ERROR_CHECK(error_code);	//������
	
	nrf_drv_timer_enable(&timer0);	//ʹ�ܶ�ʱ��0
	nrf_drv_timer_enable(&timer1);	//ʹ�ܶ�ʱ��1
	nrf_drv_timer_enable(&timer2);	//ʹ�ܶ�ʱ��2
}

/*******************************************************************************
  * @brief		��ȡPPI֮��ʱ��Ӧ�ã�����⣩ֵ����
  * @param		None
  * @retval		��ʱ��������ֵ
  * @attention	�ڲ�������Counter�Ĵ�����ֵ�洢��CC[0]
*******************************************************************************/
uint32_t get_ppi_timer_application_component_value(void)
{
	nrf_drv_timer_increment(&timer0);	//��ʱ��0����һ�μ���
	return nrf_drv_timer_capture(&timer0, NRF_TIMER_CC_CHANNEL0);	//����ʱ��0��ʱ��ֵ
}
