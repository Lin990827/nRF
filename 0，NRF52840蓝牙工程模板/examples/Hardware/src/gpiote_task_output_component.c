#include "gpiote_task_output_component.h"
#include "app_error.h"
#include "nrf_drv_gpiote.h"

/*******************************************************************************
  * @brief		GPIOTE任务输出组件初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void gpiote_task_output_component_init(void)
{
	ret_code_t error_code;	//错误代码
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	
	nrf_drv_gpiote_out_config_t gpiote_out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);	//配置引脚使用GPIO OUT TASK切换引脚状态
	error_code = nrf_drv_gpiote_out_init(13, &gpiote_out_config1);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(13);	//使能GPIOTE输出引脚任务
	
	nrf_drv_gpiote_out_config_t gpiote_out_config2 = GPIOTE_CONFIG_OUT_TASK_LOW;	//配置引脚使用GPIO OUT TASK将状态从高变为低
	error_code = nrf_drv_gpiote_out_init(14, &gpiote_out_config2);	//NRF初始化GPIOTE输出引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_out_task_enable(14);	//使能GPIOTE输出引脚任务
}
