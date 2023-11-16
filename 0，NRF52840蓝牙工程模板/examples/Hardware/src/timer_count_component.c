#include "timer_count_component.h"
#include "nrf_drv_timer.h"

const nrf_drv_timer_t create_timer0 = NRF_DRV_TIMER_INSTANCE(0);	//创建定时器0

/*******************************************************************************
  * @brief		定时器事件处理函数
  * @param		event_type - 定时器事件
				p_context - 传递的参数
  * @retval		None
  * @attention	None
*******************************************************************************/
static void timer_event_handler(nrf_timer_event_t event_type,void *p_context)
{
}

/*******************************************************************************
  * @brief		定时器计数组件初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_count_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_drv_timer_config_t timer_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器配置参数（默认配置参数）
	
	timer_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//计数器模式
	error_code = nrf_drv_timer_init(&create_timer0, &timer_configuration_parameter, timer_event_handler);	//定时器初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	nrf_drv_timer_enable(&create_timer0);	//使能定时器
}

/*******************************************************************************
  * @brief		获取组件定时器计数的值函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
uint32_t get_timer_count_component_value(void)
{
	nrf_drv_timer_increment(&create_timer0);	//触发一次计数
	return nrf_drv_timer_capture(&create_timer0, NRF_TIMER_CC_CHANNEL0);	//捕获计时器值
}
