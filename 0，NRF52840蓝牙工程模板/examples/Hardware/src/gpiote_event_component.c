#include "gpiote_event_component.h"
#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_gpiote.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

#ifdef BSP_BUTTON_0	//BSP_BUTTON_0宏被定义
#define PIN_IN							BSP_BUTTON_0	//宏定义输入管脚
#endif

#ifndef PIN_IN	//PIN_IN宏未被定义
#error "Please indicate input pin"	//生成一个编译错误消息
#endif

#ifdef BSP_LED_0	//BSP_LED_0宏被定义
#define PIN_OUT							BSP_LED_0	//宏定义输出管脚
#endif

#ifndef PIN_OUT	//PIN_OUT宏未被定义
#error "Please indicate output pin"	//生成一个编译错误消息
#endif

/*******************************************************************************
  * @brief		GPIO事件处理函数
  * @param		pin - 触发此事件的引脚
				action - 导致触发此事件的操作
  * @retval		None
  * @attention	只支持静态配置，以防止启动程序自定义共享资源
*******************************************************************************/
static void pin_event_handler(nrfx_gpiote_pin_t pin,nrf_gpiote_polarity_t action)
{
	nrf_delay_ms(10);	//消抖
	if(nrf_gpio_pin_read(PIN_IN) == 0)	//输入引脚为低电平
	{
		nrf_drv_gpiote_out_toggle(PIN_OUT);	//输出引脚翻转
	}
}

/*******************************************************************************
  * @brief		GPIOTE事件组件初始化函数
  * @param		None
  * @retval		None
  * @attention	配置功能:输入PIN_IN引脚，输出PIN_OUT引脚，配置GPIOTE在引脚变化时中断
*******************************************************************************/
void gpiote_event_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_ppi_channel_t ppi_channel;	//PPI通道
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_gpiote_out_config_t gpiote_out_config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(PIN_OUT, &gpiote_out_config);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(PIN_OUT);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(PIN_IN, &gpiote_in_config, pin_event_handler);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(PIN_IN, true);	//使能GPIOTE输入引脚事件
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel);	//分配未使用的PPI通道
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel, nrf_drv_gpiote_in_event_addr_get(PIN_IN), 
				 nrf_drv_gpiote_out_task_addr_get(PIN_OUT));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel);	//使能PPI通道，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
}
