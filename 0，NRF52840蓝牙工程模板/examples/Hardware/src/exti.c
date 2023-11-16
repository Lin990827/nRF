#include "exti.h"
#include "key.h"
#include "nrf_delay.h"
#include "led.h"

/*******************************************************************************
  * @brief		中断初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void exti_init(void)
{
	nrf_gpio_cfg_input(SW1, NRF_GPIO_PIN_PULLUP);	//配置SW1 IO端口为上拉输入状态
	nrf_gpio_cfg_input(SW2, NRF_GPIO_PIN_PULLUP);	//配置SW2 IO端口为上拉输入状态
	nrf_gpio_cfg_input(SW3, NRF_GPIO_PIN_PULLUP);	//配置SW3 IO端口为上拉输入状态
	nrf_gpio_cfg_input(SW4, NRF_GPIO_PIN_PULLUP);	//配置SW4 IO端口为上拉输入状态
	nrf_gpio_cfg_input(TOUCH, NRF_GPIO_PIN_PULLUP);	//配置TOUCH IO端口为上拉输入状态
	
	NVIC_EnableIRQ(GPIOTE_IRQn);	//使能外部中断
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为低电平；事件模式下：管脚下降沿到时候产生IN[n]输入事件
							| (11 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;	//使能IN[0]事件中断
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为低电平；事件模式下：管脚下降沿到时候产生IN[n]输入事件
							| (12 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN1_Set << GPIOTE_INTENSET_IN1_Pos;	//使能IN[1]事件中断
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为低电平；事件模式下：管脚下降沿到时候产生IN[n]输入事件
							| (24 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN2_Set << GPIOTE_INTENSET_IN2_Pos;	//使能IN[2]事件中断
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为低电平；事件模式下：管脚下降沿到时候产生IN[n]输入事件
							| (25 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN3_Set << GPIOTE_INTENSET_IN3_Pos;	//使能IN[3]事件中断
	
	NRF_GPIOTE->CONFIG[4] = (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (26 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN4_Set << GPIOTE_INTENSET_IN4_Pos;	//使能IN[4]事件中断
	
}

#if 0
/*******************************************************************************
  * @brief		GPIO事件中断服务函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void GPIOTE_IRQHandler(void)
{
	if((NRF_GPIOTE->EVENTS_IN[0] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))	//从CONFIG[0].PSEL中指定的引脚生成的事件并且IN[0]事件中断已使能
	{
		NRF_GPIOTE->EVENTS_IN[0] = 0;	//清除从CONFIG[0].PSEL中指定的引脚生成的事件
		nrf_delay_ms(20);	//消抖
		led_toggle(LED1);
	}
	if((NRF_GPIOTE->EVENTS_IN[1] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN1_Msk))	//从CONFIG[1].PSEL中指定的引脚生成的事件并且IN[1]事件中断已使能
	{
		NRF_GPIOTE->EVENTS_IN[1] = 0;	//清除从CONFIG[1].PSEL中指定的引脚生成的事件
		nrf_delay_ms(20);	//消抖
		led_toggle(LED2);
	}
	if((NRF_GPIOTE->EVENTS_IN[2] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN2_Msk))	//从CONFIG[2].PSEL中指定的引脚生成的事件并且IN[2]事件中断已使能
	{
		NRF_GPIOTE->EVENTS_IN[2] = 0;	//清除从CONFIG[2].PSEL中指定的引脚生成的事件
		nrf_delay_ms(20);	//消抖
		led_toggle(LED3);
	}
	if((NRF_GPIOTE->EVENTS_IN[3] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN3_Msk))	//从CONFIG[3].PSEL中指定的引脚生成的事件并且IN[3]事件中断已使能
	{
		NRF_GPIOTE->EVENTS_IN[3] = 0;	//清除从CONFIG[3].PSEL中指定的引脚生成的事件
		nrf_delay_ms(20);	//消抖
		led_toggle(LED4);
	}
	if((NRF_GPIOTE->EVENTS_IN[4] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN4_Msk))	//从CONFIG[4].PSEL中指定的引脚生成的事件并且IN[4]事件中断已使能
	{
		NRF_GPIOTE->EVENTS_IN[4] = 0;	//清除从CONFIG[4].PSEL中指定的引脚生成的事件
		nrf_delay_ms(20);	//消抖
		led_toggle(LED1);
		led_toggle(LED2);
		led_toggle(LED3);
		led_toggle(LED4);
	}
}
#endif
