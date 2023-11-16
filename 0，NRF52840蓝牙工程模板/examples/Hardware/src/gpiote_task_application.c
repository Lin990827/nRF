#include "gpiote_task_application.h"
#include "nrf.h"

/*******************************************************************************
  * @brief		GPIOTE任务应用初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void gpiote_task_application_init(void)
{
	NVIC_EnableIRQ(GPIOTE_IRQn);	//使能外部中断
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (13 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (14 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
}
