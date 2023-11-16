#include "watchdog.h"
#include "nrf_drv_clock.h"

nrf_drv_wdt_channel_id wdt_channel;	//看门狗通道

/*******************************************************************************
  * @brief		看门狗内部LFCLK晶振（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_lfclk_register(void)
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
  * @brief		看门狗配置（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_config_register(void)
{
	NRF_WDT->CRV = 65536;	//计数器重装载值
	NRF_WDT->CONFIG = WDT_CONFIG_SLEEP_Msk;	//CPU睡眠时，保持看门狗
	NRF_WDT->RREN = WDT_RREN_RR0_Msk;	//使用RR[0]通道
	
	NRF_WDT->EVENTS_TIMEOUT = WDT_EVENTS_TIMEOUT_EVENTS_TIMEOUT_Msk;	//使能看门狗定时器超时事件
	NRF_WDT->INTENSET = WDT_INTENSET_TIMEOUT_Msk;	//使能看门狗定时器超时事件事件中断
	
	NVIC_EnableIRQ(WDT_IRQn);	//使能看门狗中断
	NRF_WDT->TASKS_START = WDT_TASKS_START_TASKS_START_Msk;	//启动看门狗
}

/*******************************************************************************
  * @brief		看门狗事件处理函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
static void wdt_event_handler(void)
{
}

/*******************************************************************************
  * @brief		看门狗内部LFCLK晶振（组件库）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_lfclk_component(void)
{
	ret_code_t error_code;	//错误代码
	
	error_code = nrf_drv_clock_init();	//初始化nrf_drv_clock模块的功能。初始化后，模块处于下电状态（不请求时钟）
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_clock_lfclk_request(NULL);	//请求LFCLK
}

/*******************************************************************************
  * @brief		看门狗配置（组件库）函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void watchdog_config_component(void)
{
	ret_code_t error_code;	//错误代码
	nrf_drv_wdt_config_t watchdog_configuration_parameter = NRF_DRV_WDT_DEAFULT_CONFIG;	//看门狗配置参数（默认配置参数）
	
	error_code = nrf_drv_wdt_init(&watchdog_configuration_parameter, wdt_event_handler);	//看门狗初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	error_code = nrf_drv_wdt_channel_alloc(&wdt_channel);	//分配看门狗通道
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_wdt_enable();	//启动看门狗
}
