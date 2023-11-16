#include "rtc.h"
#include "led.h"
#include "boards.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

/*
fRTC[KHz] = 32.768 / (PRESCALER + 1)
*/

#define LFCLK_FREQUENCY					32768UL	//�궨��LFCLKƵ��Ϊ32768HZ
#define RTC0_FREQUENCY					8UL	//�궨��RTC0Ƶ��Ϊ8HZ
#define RTC0_COMPARE_COUNT				3UL	//�궨��RTC0�Ƚϴ���
#define RTC0_PRESCALER					(LFCLK_FREQUENCY / RTC0_FREQUENCY) - 1	//�궨��RCT0Ԥ��Ƶֵ

#ifdef BSP_LED_0	//BSP_LED_0�걻����
#define TICK_EVENT_OUTPUT				BSP_LED_0	//�궨��TICK�¼�����ܽ�
#endif

#ifndef TICK_EVENT_OUTPUT	//TICK_EVENT_OUTPUT��δ������
#error "Please indicate tick event output pin"	//����һ�����������Ϣ
#endif

#ifdef BSP_LED_1	//BSP_LED_1�걻����
#define COMPARE_EVENT_OUTPUT			BSP_LED_1	//�궨��COMPARE[0]�Ƚ��¼�����ܽ�
#endif

#ifndef COMPARE_EVENT_OUTPUT	//COMPARE_EVENT_OUTPUT��δ������
#error "Please indicate compare event output pin"	//����һ�����������Ϣ
#endif

const nrf_drv_rtc_t rtc0 = NRF_DRV_RTC_INSTANCE(0);	//����RTC0

/*******************************************************************************
  * @brief		�����ڲ�LFCLK�����ܣ��Ĵ���������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void start_lfclk_register(void)
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
  * @brief		RTC0���ã��Ĵ���������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rtc0_config_register(void)
{
	NRF_RTC0->PRESCALER = RTC0_PRESCALER;	//RTC0Ԥ��Ƶֵ
	#if 0
	NRF_RTC0->CC[0] = RTC0_FREQUENCY;	//RTC0����ֵ0
	NRF_RTC0->EVTENSET = RTC_EVTENSET_TICK_Msk | RTC_EVTENSET_COMPARE0_Msk;	//ʹ��TICK�¼���COMPARE[0]�Ƚ��¼�
	NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk | RTC_INTENSET_COMPARE0_Msk;	//ʹ��TICK�¼��жϡ�CMOPARE[0]�Ƚ��¼��ж�
	#endif
	NRF_RTC0->TASKS_TRIGOVRFLW = 1;	//�����������
	NRF_RTC0->EVTENSET = RTC_EVTENSET_OVRFLW_Msk;	//ʹ������¼�
	NRF_RTC0->INTENSET = RTC_INTENSET_OVRFLW_Msk;	//ʹ������¼��ж�
	
	NVIC_EnableIRQ(RTC0_IRQn);	//ʹ��RTC0�ж�
	NRF_RTC0->TASKS_START = 1;	//����RTC0������
}

#if 0
/*******************************************************************************
  * @brief		RTC0�жϷ�����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void RTC0_IRQHandler(void)
{
	#if 0
	//uint32_t rtc0_cc0;	//���¸�ֵCC[0]�����Ŵ����Ƚ��¼�
	
	if((NRF_RTC0->EVENTS_TICK != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_TICK_Msk) != 0))	//TICK�¼���������ʹ��TICK�¼�
	{
		NRF_RTC0->EVENTS_TICK = 0;	//���TICK�¼�
		led_toggle(LED1);
	}
	if((NRF_RTC0->EVENTS_COMPARE[0] != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0))	//COMPARE[0]�Ƚ��¼���������ʹ��COMPARE[0]�Ƚ��¼�
	{
		NRF_RTC0->EVENTS_COMPARE[0] = 0;	//���COMPARE[0]�Ƚ��¼�
		led_open(LED2);
		//rtc0_cc0 = NRF_RTC0->COUNTER;
		//NRF_RTC0->CC[0] = rtc0_cc0 + RTC0_FREQUENCY;
		//led_toggle(LED2);
	}
	#endif
	if((NRF_RTC0->EVENTS_OVRFLW != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_OVRFLW_Msk) != 0))	//����¼���������ʹ������¼�
	{
		NRF_RTC0->EVENTS_OVRFLW = 0;	//���TICK�¼�
		NRF_RTC0->TASKS_TRIGOVRFLW = 1;	//���´����������
		led_toggle(LED1);
	}
}
#endif

#if 1
/*******************************************************************************
  * @brief		RTCʵ��������
  * @param		int_type - �ж�����
  * @retval		None
  * @attention	None
*******************************************************************************/
static void rtc_handler(nrfx_rtc_int_type_t int_type)
{
	#if 0
	if(int_type == NRF_DRV_RTC_INT_TICK)	//�ж�����ΪTICK�¼��ж�
	{
		nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
	}
	else if(int_type == NRF_DRV_RTC_INT_COMPARE0)	//�ж�����ΪCOMPARE[0]�Ƚ��¼�
	{
		nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
	}
	#endif
	if(int_type == NRF_DRV_RTC_INT_OVERFLOW)	//�ж�����Ϊ����¼�
	{
		nrf_gpio_pin_toggle(LED1);
		nrf_rtc_task_trigger(rtc0.p_reg, NRF_RTC_TASK_TRIGGER_OVERFLOW);	//���´�������ж�
	}
}

/*******************************************************************************
  * @brief		�����ڲ�LFCLK�����ܣ�����⣩����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void start_lfclk_component(void)
{
	ret_code_t error_code;	//�������
	
	error_code = nrf_drv_clock_init();	//��ʼ��nrf_drv_clockģ��Ĺ��ܡ���ʼ����ģ�鴦���µ�״̬��������ʱ�ӣ�
	APP_ERROR_CHECK(error_code);	//���������
	nrf_drv_clock_lfclk_request(NULL);	//����LFCLK
}

/*******************************************************************************
  * @brief		RTC0���ã�����⣩����
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rtc0_config_component(void)
{
	ret_code_t error_code;	//�������
	nrfx_rtc_config_t rtc0_configuration_parameter = NRF_DRV_RTC_DEFAULT_CONFIG;	//RTC0���ò�����Ĭ�����ò�����
	
	rtc0_configuration_parameter.prescaler = RTC0_PRESCALER;	//����RTC0Ԥ��Ƶֵ
	error_code = nrf_drv_rtc_init(&rtc0, &rtc0_configuration_parameter, rtc_handler);	//RTC0��ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	
	nrf_drv_rtc_tick_enable(&rtc0, true);	//ʹ��RTC0��TICK����
	
	#if 0
	error_code = nrf_drv_rtc_cc_set(&rtc0, 0, RTC0_FREQUENCY, true);	//ѡ��RTC0�Ƚ�ͨ��
	APP_ERROR_CHECK(error_code);	//���������
	#endif
	
	nrf_drv_rtc_overflow_enable(&rtc0, true);	//ʹ������¼��ж�
	nrf_rtc_task_trigger(rtc0.p_reg, NRF_RTC_TASK_TRIGGER_OVERFLOW);	//��������ж�
	
	nrf_drv_rtc_enable(&rtc0);	//ʹ��RTC0����ʵ��
}
#endif
