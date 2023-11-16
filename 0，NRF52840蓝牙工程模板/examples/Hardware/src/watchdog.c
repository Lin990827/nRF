#include "watchdog.h"
#include "nrf_drv_clock.h"

nrf_drv_wdt_channel_id wdt_channel;	//���Ź�ͨ��

/*******************************************************************************
  * @brief		���Ź��ڲ�LFCLK���񣨼Ĵ���������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_lfclk_register(void)
{
	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);	//LFCLKʱ��Դ��32.768KHz����������LFXO��
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;	//ʧ��LFCLK�¼�
	NRF_CLOCK->TASKS_LFCLKSTART = 1;	//����LFCLK����
	while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)	//�ȴ�ʧ��LFCLK�¼����
	{
		
	}
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;	//ʧ��LFCLK�¼�
}

/*******************************************************************************
  * @brief		���Ź����ã��Ĵ���������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_config_register(void)
{
	NRF_WDT->CRV = 65536;	//��������װ��ֵ
	NRF_WDT->CONFIG = WDT_CONFIG_SLEEP_Msk;	//CPU˯��ʱ�����ֿ��Ź�
	NRF_WDT->RREN = WDT_RREN_RR0_Msk;	//ʹ��RR[0]ͨ��
	
	NRF_WDT->EVENTS_TIMEOUT = WDT_EVENTS_TIMEOUT_EVENTS_TIMEOUT_Msk;	//ʹ�ܿ��Ź���ʱ����ʱ�¼�
	NRF_WDT->INTENSET = WDT_INTENSET_TIMEOUT_Msk;	//ʹ�ܿ��Ź���ʱ����ʱ�¼��¼��ж�
	
	NVIC_EnableIRQ(WDT_IRQn);	//ʹ�ܿ��Ź��ж�
	NRF_WDT->TASKS_START = WDT_TASKS_START_TASKS_START_Msk;	//�������Ź�
}

/*******************************************************************************
  * @brief		���Ź��¼�������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
static void wdt_event_handler(void)
{
}

/*******************************************************************************
  * @brief		���Ź��ڲ�LFCLK��������⣩����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_lfclk_component(void)
{
	ret_code_t error_code;	//�������
	
	error_code = nrf_drv_clock_init();	//��ʼ��nrf_drv_clockģ��Ĺ��ܡ���ʼ����ģ�鴦���µ�״̬��������ʱ�ӣ�
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_clock_lfclk_request(NULL);	//����LFCLK
}

/*******************************************************************************
  * @brief		���Ź����ã�����⣩����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_config_component(void)
{
	ret_code_t error_code;	//�������
	nrf_drv_wdt_config_t watchdog_configuration_parameter = NRF_DRV_WDT_DEAFULT_CONFIG;	//���Ź����ò�����Ĭ�����ò�����
	
	error_code = nrf_drv_wdt_init(&watchdog_configuration_parameter, wdt_event_handler);	//���Ź���ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	error_code = nrf_drv_wdt_channel_alloc(&wdt_channel);	//���俴�Ź�ͨ��
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_wdt_enable();	//�������Ź�
}
