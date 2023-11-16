#include "ppi_timer_application.h"
#include "boards.h"
#include "nrf_drv_timer.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);	//创建定时器0
const nrf_drv_timer_t timer1 = NRF_DRV_TIMER_INSTANCE(1);	//创建定时器1
const nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(2);	//创建定时器2

/*******************************************************************************
  * @brief		PPI之定时器应用（寄存器）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_timer_application_register_init(void)
{
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Counter;	//定时器0：计数器模式
	NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//定时器0：16位宽定时器
	NRF_TIMER0->PRESCALER = 9;	//定时器0：预分频值9-1
	
	/* SysClk = 16 Mhz   PRESCALER = 9 */
	/* 触发时间 = 0xFFFF / (SysClk / 2 ^ PRESCALER) = 65535 / 31250 = 2.097 sec */
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;	//定时器1：定时器模式
	NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//定时器1：16位宽定时器
	NRF_TIMER1->PRESCALER = 9;	//定时器1：预分频值9-1
	NRF_TIMER1->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);	//定时器1：快捷方式注册COMPARE[i]事件和CLEAR任务之间的快捷方式
	NRF_TIMER1->CC[0] = 0xFFFFUL;	//定时器1：设置比较寄存器
	
	/* SysClk = 16 Mhz   PRESCALER = 9 */
	/* 触发时间 = 0xFFFF / (SysClk / 2 ^ PRESCALER) = 32767 / 31250 = 1.048 sec */
	NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;	//定时器2：定时器模式
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//定时器2：16位宽定时器
	NRF_TIMER2->PRESCALER = 9;	//定时器2：预分频值9-1
	NRF_TIMER2->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);	//定时器2：快捷方式注册COMPARE[i]事件和CLEAR任务之间的快捷方式
	NRF_TIMER2->CC[0] = 0x7FFFUL;	//定时器2：设置比较寄存器
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_TIMER1->EVENTS_COMPARE[0]);	//PPI通道0事件终点（定时器1比较事件）
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_TIMER0->TASKS_STOP);	//PPI通道0任务终点（定时器0停止任务）
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_TIMER2->EVENTS_COMPARE[0]);	//PPI通道1事件终点（定时器2比较事件）
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_TIMER0->TASKS_START);	//PPI通道1任务终点（定时器0开始任务）
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos) | (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos);	//通道使能
	
	NRF_TIMER1->TASKS_START = 1;	//开启定时器1
	NRF_TIMER2->TASKS_START = 1;	//开启定时器2
}

/*******************************************************************************
  * @brief		获取PPI之定时器应用（寄存器）值函数
  * @param		None
  * @retval		定时器计数的值
  * @attention	内部计数器Counter寄存器的值存储到CC[0]
*******************************************************************************/
uint32_t get_ppi_timer_application_register_value(void)
{
	NRF_TIMER0->TASKS_CAPTURE[0] = 1;	//将Counter值捕获到CC[0]寄存器
	NRF_TIMER0->TASKS_COUNT = 1;	//计数器递增（仅限计数器模式）
	return NRF_TIMER0->CC[0];
}

/*******************************************************************************
  * @brief		PPI之定时器应用（组件库）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_timer_application_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_drv_timer_config_t timer0_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器0配置参数（默认配置参数）
	nrf_drv_timer_config_t timer1_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器1配置参数（默认配置参数）
	nrf_drv_timer_config_t timer2_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器2配置参数（默认配置参数）
	nrf_ppi_channel_t ppi_channel1;	//PPI通道1
	nrf_ppi_channel_t ppi_channel2;	//PPI通道2
	
	timer0_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//计数器模式
	error_code = nrf_drv_timer_init(&timer0, &timer0_configuration_parameter, NULL);	//定时器0初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_timer_init(&timer1, &timer1_configuration_parameter, NULL);	//定时器1初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_timer_extended_compare(&timer1, NRF_TIMER_CC_CHANNEL0, 0xFFFFUL, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);	//使能定时器1比较通道，使能比较中断，设置触发比较寄存器CC[n]等参数
	
	error_code = nrf_drv_timer_init(&timer2, &timer2_configuration_parameter, NULL);	//定时器2初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_timer_extended_compare(&timer2, NRF_TIMER_CC_CHANNEL0, 0x7FFFUL, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);	//使能定时器2比较通道，使能比较中断，设置触发比较寄存器CC[n]等参数
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//分配未使用的PPI通道1
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, 
				 nrf_drv_timer_event_address_get(&timer1, NRF_TIMER_EVENT_COMPARE0), 
				 nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_STOP));	//分配EEP事件终点（定时器1比较事件）和TEP任务终点（定时器0停止任务）
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel1);	//使能PPI通道1，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//分配未使用的PPI通道2
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, 
				 nrf_drv_timer_event_address_get(&timer2, NRF_TIMER_EVENT_COMPARE0), 
				 nrf_drv_timer_task_address_get(&timer0, NRF_TIMER_TASK_START));	//分配EEP事件终点（定时器1比较事件）和TEP任务终点（定时器2开始任务）
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_enable(ppi_channel1);	//使能PPI通道1，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel2);	//使能PPI通道2，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_timer_enable(&timer0);	//使能定时器0
	nrf_drv_timer_enable(&timer1);	//使能定时器1
	nrf_drv_timer_enable(&timer2);	//使能定时器2
}

/*******************************************************************************
  * @brief		获取PPI之定时器应用（组件库）值函数
  * @param		None
  * @retval		定时器计数的值
  * @attention	内部计数器Counter寄存器的值存储到CC[0]
*******************************************************************************/
uint32_t get_ppi_timer_application_component_value(void)
{
	nrf_drv_timer_increment(&timer0);	//定时器0触发一次计数
	return nrf_drv_timer_capture(&timer0, NRF_TIMER_CC_CHANNEL0);	//捕获定时器0计时器值
}
