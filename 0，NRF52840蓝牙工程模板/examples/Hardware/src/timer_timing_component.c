#include "timer_timing_component.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_drv_timer.h"
#include "boards.h"

const nrf_drv_timer_t create_timer1 = NRF_DRV_TIMER_INSTANCE(1);	//创建定时器1

/*******************************************************************************
  * @brief		定时器事件处理函数
  * @param		event_type - 定时器事件
				p_context - 传递的参数
  * @retval		None
  * @attention	None
*******************************************************************************/
static void timer_event_handler(nrf_timer_event_t event_type,void *p_context)
{
	static uint32_t i;	//进入函数次数
	uint32_t led_num = ((i++) % LEDS_NUMBER);	//取当前LED号
	
	switch(event_type)	//判断定时器事件
	{
		case NRF_TIMER_EVENT_COMPARE0:	//比较CC[0]匹配事件
			bsp_board_led_invert(led_num);	//LED翻转
			break;
		default:break;
	}
}

/*******************************************************************************
  * @brief		定时器定时组件初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_timing_component_init(void)
{
	uint32_t timer_tick;	//定时器滴答次数
	ret_code_t error_code;	//错误代码
	nrf_drv_timer_config_t timer_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器配置参数（默认配置参数）
	
	bsp_board_init(BSP_INIT_LEDS);	//配置开发板所有的灯
	error_code = nrf_drv_timer_init(&create_timer1, &timer_configuration_parameter, timer_event_handler);	//定时器初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	timer_tick = nrf_drv_timer_ms_to_ticks(&create_timer1, 1000);	//将时间(以毫秒为单位)转换为计时器滴答
	nrf_drv_timer_extended_compare(&create_timer1, NRF_TIMER_CC_CHANNEL0, timer_tick, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);	//使能定时器比较通道，使能比较中断，设置触发比较寄存器CC[n]等参数
	nrf_drv_timer_enable(&create_timer1);	//使能定时器
}
