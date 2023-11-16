#include "ppi_group_application.h"
#include "boards.h"
#include "nrf_drv_gpiote.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"

/*******************************************************************************
  * @brief		PPI之group分组应用（寄存器）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_group_application_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	nrf_gpio_cfg_input(BSP_BUTTON_1, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_1 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_1 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPI通道0事件终点
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[2]);	//PPI通道0任务终点
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[1]);	//PPI通道1事件终点
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[3]);	//PPI通道1任务终点
	NRF_PPI->CHG[0] = 0x03;	//绑定PPI通道0、1
}

nrf_ppi_channel_group_t ppi_channel_group;	//PPI通道分组

/*******************************************************************************
  * @brief		PPI之group分组应用（组件库）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_group_application_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_ppi_channel_t ppi_channel1;	//PPI通道1
	nrf_ppi_channel_t ppi_channel2;	//PPI通道2
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_0, &gpiote_out_config1);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_0);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_1, &gpiote_out_config2);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_1);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_in_config_t gpiote_in_config1 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config1.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config1, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//使能GPIOTE输入引脚事件
	
	nrf_drv_gpiote_in_config_t gpiote_in_config2 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config2.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config2, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//使能GPIOTE输入引脚事件
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//分配未使用的PPI通道1
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_0), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_0));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//分配未使用的PPI通道2
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_1), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_1));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_group_alloc(&ppi_channel_group);	//分配未被使用的PPI通道分组
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel1, ppi_channel_group);	//把指定的PPI通道包含到通道组中
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel2, ppi_channel_group);	//把指定的PPI通道包含到通道组中
	APP_ERROR_CHECK(error_code);	//错误检查
}

/*******************************************************************************
  * @brief		PPI事件管理group分组（寄存器）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_event_manager_group_register_init(void)
{
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	nrf_gpio_cfg_input(BSP_BUTTON_1, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	nrf_gpio_cfg_input(BSP_BUTTON_2, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	nrf_gpio_cfg_input(BSP_BUTTON_3, NRF_GPIO_PIN_PULLUP);	//设置引脚上拉输入
	
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_1 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]
	
	NRF_GPIOTE->CONFIG[2] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_0 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_GPIOTE->CONFIG[3] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_1 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_GPIOTE->CONFIG[4] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_2 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]事件
	
	NRF_GPIOTE->CONFIG[5] = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为高电平；事件模式下：管脚上升沿到时候产生IN[n]输入事件
							| (BSP_BUTTON_3 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);	//事件模式：把PSEL绑定的对应管脚设置为输入模式。由POLARITY域决定什么情况下触发IN[n]
	
	NRF_GPIOTE->CONFIG[6] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_2 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_GPIOTE->CONFIG[7] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (BSP_LED_3 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_PPI->CH[2].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[4]);	//PPI通道2事件终点
	NRF_PPI->CH[2].TEP = (uint32_t)(&NRF_PPI->TASKS_CHG[0].EN);	//PPI通道2任务终点
	NRF_PPI->FORK[2].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[6]);	//PPI通道2从任务终点
	NRF_PPI->CH[3].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[5]);	//PPI通道3事件终点
	NRF_PPI->CH[3].TEP = (uint32_t)(&NRF_PPI->TASKS_CHG[0].DIS);	//PPI通道3任务终点
	NRF_PPI->FORK[3].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[7]);	//PPI通道3从任务终点
	NRF_PPI->CHEN = (PPI_CHEN_CH2_Enabled << PPI_CHEN_CH2_Pos) | (PPI_CHEN_CH3_Enabled << PPI_CHEN_CH3_Pos);	//通道使能
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[0]);	//PPI通道0事件终点
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[2]);	//PPI通道0任务终点
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_GPIOTE->EVENTS_IN[1]);	//PPI通道1事件终点
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[3]);	//PPI通道1任务终点
	NRF_PPI->CHG[0] = 0x03;	//绑定PPI通道0、1
}

/*******************************************************************************
  * @brief		PPI事件管理group分组（组件库）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_event_manager_group_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_ppi_channel_t ppi_channel1;	//PPI通道1
	nrf_ppi_channel_t ppi_channel2;	//PPI通道2
	nrf_ppi_channel_t ppi_channel3;	//PPI通道3
	nrf_ppi_channel_t ppi_channel4;	//PPI通道4
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_0, &gpiote_out_config1);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_0);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_1, &gpiote_out_config2);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_1);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_in_config_t gpiote_in_config1 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config1.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &gpiote_in_config1, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);	//使能GPIOTE输入引脚事件
	
	nrf_drv_gpiote_in_config_t gpiote_in_config2 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config2.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_1, &gpiote_in_config2, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_1, true);	//使能GPIOTE输入引脚事件
	
	nrf_drv_gpiote_out_config_t gpiote_out_config3 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_2, &gpiote_out_config3);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_2);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_out_config_t gpiote_out_config4 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(BSP_LED_3, &gpiote_out_config4);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(BSP_LED_3);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_in_config_t gpiote_in_config3 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config3.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_2, &gpiote_in_config3, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_2, true);	//使能GPIOTE输入引脚事件
	
	nrf_drv_gpiote_in_config_t gpiote_in_config4 = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的任何更改
	gpiote_in_config4.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(BSP_BUTTON_3, &gpiote_in_config4, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(BSP_BUTTON_3, true);	//使能GPIOTE输入引脚事件
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel3);	//分配未使用的PPI通道
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel3, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_2), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_2));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel3, nrf_drv_gpiote_out_task_addr_get(BSP_LED_2));	//分配从任务Fork端点到PPI通道之上
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel3, (uint32_t)nrf_ppi_task_group_enable_address_get(ppi_channel_group));	//分配从任务Fork端点到PPI通道之上
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel3);	//使能PPI通道，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel4);	//分配未使用的PPI通道
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel4, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_3), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_3));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_fork_assign(ppi_channel4, (uint32_t)nrf_ppi_task_group_disable_address_get(ppi_channel_group));	//分配从任务Fork端点到PPI通道之上
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel4);	//使能PPI通道，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel1);	//分配未使用的PPI通道1
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel1, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_0), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_0));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel2);	//分配未使用的PPI通道2
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel2, nrf_drv_gpiote_in_event_addr_get(BSP_BUTTON_1), 
				 nrf_drv_gpiote_out_task_addr_get(BSP_LED_1));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_group_alloc(&ppi_channel_group);	//分配未被使用的PPI通道分组
	APP_ERROR_CHECK(error_code);	//错误检查
	
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel1, ppi_channel_group);	//把指定的PPI通道包含到通道组中
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_include_in_group(ppi_channel2, ppi_channel_group);	//把指定的PPI通道包含到通道组中
	APP_ERROR_CHECK(error_code);	//错误检查
}
