#include "gpiote_port_application.h"
#include "boards.h"
#include "nrf_drv_gpiote.h"
#include "led.h"

/*******************************************************************************
  * @brief		GPIO事件处理函数
  * @param		pin - 触发此事件的引脚
				action - 导致触发此事件的操作
  * @retval		None
  * @attention	只支持静态配置，以防止启动程序自定义共享资源
*******************************************************************************/
static void pin_event_handler(nrfx_gpiote_pin_t pin,nrf_gpiote_polarity_t action)
{
	if(pin == BUTTON_1)	//事件由SW1按下
		led_toggle(LED_1);	//翻转LED1
	else if(pin == BUTTON_2)	//事件由SW2按下
		led_toggle(LED_2);	//翻转LED2
	else if(pin == BUTTON_3)	//事件由SW3按下
		led_toggle(LED_3);	//翻转LED3
	else if(pin == BUTTON_4)	//事件由SW4按下
		led_toggle(LED_4);	//翻转LED4
}

/*******************************************************************************
  * @brief		GPIOTE端口应用初始化函数
  * @param		None
  * @retval		None
  * @attention	配置功能:输入PIN_IN引脚，输出PIN_OUT引脚，配置GPIOTE在引脚变化时中断
*******************************************************************************/
void gpiote_port_application_init(void)
{
	ret_code_t error_code;	//错误代码
	
	nrf_gpio_cfg_output(LED_1);	//配置LED1输出
	nrf_gpio_cfg_output(LED_2);	//配置LED2输出
	nrf_gpio_cfg_output(LED_3);	//配置LED3输出
	nrf_gpio_cfg_output(LED_4);	//配置LED4输出
	
	led_close(LED1);	//关闭LED1
	led_close(LED2);	//关闭LED2
	led_close(LED3);	//关闭LED3
	led_close(LED4);	//关闭LED4
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);	//配置引脚以使用GPIO IN或PORT EVENT检测高到低转换
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	
	/*SW1*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config, pin_event_handler);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//使能GPIOTE输入引脚事件
	
	/*SW2*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config, pin_event_handler);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//使能GPIOTE输入引脚事件
	
	/*SW3*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_2, &gpiote_in_config, pin_event_handler);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_2, true);	//使能GPIOTE输入引脚事件
	
	/*SW4*/
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_3, &gpiote_in_config, pin_event_handler);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_3, true);	//使能GPIOTE输入引脚事件
}
