#include "rtc.h"
#include "led.h"
#include "boards.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

/*
fRTC[KHz] = 32.768 / (PRESCALER + 1)
*/

#define LFCLK_FREQUENCY					32768UL	//宏定义LFCLK频率为32768HZ
#define RTC0_FREQUENCY					8UL	//宏定义RTC0频率为8HZ
#define RTC0_COMPARE_COUNT				3UL	//宏定义RTC0比较次数
#define RTC0_PRESCALER					(LFCLK_FREQUENCY / RTC0_FREQUENCY) - 1	//宏定义RCT0预分频值

#ifdef BSP_LED_0	//BSP_LED_0宏被定义
#define TICK_EVENT_OUTPUT				BSP_LED_0	//宏定义TICK事件输出管脚
#endif

#ifndef TICK_EVENT_OUTPUT	//TICK_EVENT_OUTPUT宏未被定义
#error "Please indicate tick event output pin"	//生成一个编译错误消息
#endif

#ifdef BSP_LED_1	//BSP_LED_1宏被定义
#define COMPARE_EVENT_OUTPUT			BSP_LED_1	//宏定义COMPARE[0]比较事件输出管脚
#endif

#ifndef COMPARE_EVENT_OUTPUT	//COMPARE_EVENT_OUTPUT宏未被定义
#error "Please indicate compare event output pin"	//生成一个编译错误消息
#endif

const nrf_drv_rtc_t rtc0 = NRF_DRV_RTC_INSTANCE(0);	//声明RTC0

/*******************************************************************************
  * @brief		启动内部LFCLK晶振功能（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void start_lfclk_register(void)
{
	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);	//LFCLK时钟源，32.768KHz晶体振荡器（LFXO）
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;	//失能LFCLK事件
	NRF_CLOCK->TASKS_LFCLKSTART = 1;	//启动LFCLK任务
	while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)	//等待失能LFCLK事件完成
	{
		
	}
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;	//失能LFCLK事件
}

/*******************************************************************************
  * @brief		RTC0配置（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rtc0_config_register(void)
{
	NRF_RTC0->PRESCALER = RTC0_PRESCALER;	//RTC0预分频值
	#if 0
	NRF_RTC0->CC[0] = RTC0_FREQUENCY;	//RTC0捕获值0
	NRF_RTC0->EVTENSET = RTC_EVTENSET_TICK_Msk | RTC_EVTENSET_COMPARE0_Msk;	//使能TICK事件、COMPARE[0]比较事件
	NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk | RTC_INTENSET_COMPARE0_Msk;	//使能TICK事件中断、CMOPARE[0]比较事件中断
	#endif
	NRF_RTC0->TASKS_TRIGOVRFLW = 1;	//触发溢出任务
	NRF_RTC0->EVTENSET = RTC_EVTENSET_OVRFLW_Msk;	//使能溢出事件
	NRF_RTC0->INTENSET = RTC_INTENSET_OVRFLW_Msk;	//使能溢出事件中断
	
	NVIC_EnableIRQ(RTC0_IRQn);	//使能RTC0中断
	NRF_RTC0->TASKS_START = 1;	//启动RTC0计数器
}

#if 0
/*******************************************************************************
  * @brief		RTC0中断服务函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void RTC0_IRQHandler(void)
{
	#if 0
	//uint32_t rtc0_cc0;	//重新赋值CC[0]，接着触发比较事件
	
	if((NRF_RTC0->EVENTS_TICK != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_TICK_Msk) != 0))	//TICK事件产生并且使能TICK事件
	{
		NRF_RTC0->EVENTS_TICK = 0;	//清除TICK事件
		led_toggle(LED1);
	}
	if((NRF_RTC0->EVENTS_COMPARE[0] != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0))	//COMPARE[0]比较事件产生并且使能COMPARE[0]比较事件
	{
		NRF_RTC0->EVENTS_COMPARE[0] = 0;	//清除COMPARE[0]比较事件
		led_open(LED2);
		//rtc0_cc0 = NRF_RTC0->COUNTER;
		//NRF_RTC0->CC[0] = rtc0_cc0 + RTC0_FREQUENCY;
		//led_toggle(LED2);
	}
	#endif
	if((NRF_RTC0->EVENTS_OVRFLW != 0) && ((NRF_RTC0->INTENSET & RTC_INTENSET_OVRFLW_Msk) != 0))	//溢出事件产生并且使能溢出事件
	{
		NRF_RTC0->EVENTS_OVRFLW = 0;	//清除TICK事件
		NRF_RTC0->TASKS_TRIGOVRFLW = 1;	//重新触发溢出任务
		led_toggle(LED1);
	}
}
#endif

#if 1
/*******************************************************************************
  * @brief		RTC实例处理函数
  * @param		int_type - 中断类型
  * @retval		None
  * @attention	None
*******************************************************************************/
static void rtc_handler(nrfx_rtc_int_type_t int_type)
{
	#if 0
	if(int_type == NRF_DRV_RTC_INT_TICK)	//中断类型为TICK事件中断
	{
		nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
	}
	else if(int_type == NRF_DRV_RTC_INT_COMPARE0)	//中断类型为COMPARE[0]比较事件
	{
		nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
	}
	#endif
	if(int_type == NRF_DRV_RTC_INT_OVERFLOW)	//中断类型为溢出事件
	{
		nrf_gpio_pin_toggle(LED1);
		nrf_rtc_task_trigger(rtc0.p_reg, NRF_RTC_TASK_TRIGGER_OVERFLOW);	//重新触发溢出中断
	}
}

/*******************************************************************************
  * @brief		启动内部LFCLK晶振功能（组件库）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void start_lfclk_component(void)
{
	ret_code_t error_code;	//错误代码
	
	error_code = nrf_drv_clock_init();	//初始化nrf_drv_clock模块的功能。初始化后，模块处于下电状态（不请求时钟）
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_clock_lfclk_request(NULL);	//请求LFCLK
}

/*******************************************************************************
  * @brief		RTC0配置（组件库）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rtc0_config_component(void)
{
	ret_code_t error_code;	//错误代码
	nrfx_rtc_config_t rtc0_configuration_parameter = NRF_DRV_RTC_DEFAULT_CONFIG;	//RTC0配置参数（默认配置参数）
	
	rtc0_configuration_parameter.prescaler = RTC0_PRESCALER;	//设置RTC0预分频值
	error_code = nrf_drv_rtc_init(&rtc0, &rtc0_configuration_parameter, rtc_handler);	//RTC0初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	nrf_drv_rtc_tick_enable(&rtc0, true);	//使能RTC0的TICK功能
	
	#if 0
	error_code = nrf_drv_rtc_cc_set(&rtc0, 0, RTC0_FREQUENCY, true);	//选择RTC0比较通道
	APP_ERROR_CHECK(error_code);	//错误代码检查
	#endif
	
	nrf_drv_rtc_overflow_enable(&rtc0, true);	//使能溢出事件中断
	nrf_rtc_task_trigger(rtc0.p_reg, NRF_RTC_TASK_TRIGGER_OVERFLOW);	//触发溢出中断
	
	nrf_drv_rtc_enable(&rtc0);	//使能RTC0驱动实例
}
#endif
