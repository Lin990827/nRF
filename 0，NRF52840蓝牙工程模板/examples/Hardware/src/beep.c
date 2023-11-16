#include "beep.h"

/*******************************************************************************
  * @brief		BEEP初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_init(void)
{
	nrf_gpio_cfg_output(BEEP);	//配置BEEP IO端口为输出状态
}

/*******************************************************************************
  * @brief		BEEP打开函数
  * @param		pin_number - BEEP端口和引脚号
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_open(uint32_t pin_number)
{
	nrf_gpio_pin_set(pin_number);	//IO端口和引脚号输出高电平
}

/*******************************************************************************
  * @brief		BEEP关闭函数
  * @param		pin_number - BEEP端口和引脚号
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_close(uint32_t pin_number)
{
	nrf_gpio_pin_clear(pin_number);	//IO端口和引脚号输出低电平
}

/*******************************************************************************
  * @brief		BEEP电平翻转函数
  * @param		pin_number - BEEP端口和引脚号
  * @retval		None
  * @attention	None
*******************************************************************************/
void beep_toggle(uint32_t pin_number)
{
	nrf_gpio_pin_toggle(pin_number);	//IO端口和引脚号电平翻转（先输出高电平，再输出低电平）
}
