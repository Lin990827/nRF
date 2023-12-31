#include "ppi_gpiote_application.h"
#include "boards.h"
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
  * @brief		PPI之GPIOTE应用（寄存器）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_gpiote_application_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为低电平；事件模式下：管脚下降沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPI通道0事件终点
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPI通道0任务终点
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos);	//通道使能
}

/*******************************************************************************
  * @brief		PPI之GPIOTE应用（组件库）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_gpiote_application_component_init(void)
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
	error_code = nrf_drv_gpiote_in_init(PIN_IN, &gpiote_in_config, NULL);	//NRF初始化GPIOTE输入引脚
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
