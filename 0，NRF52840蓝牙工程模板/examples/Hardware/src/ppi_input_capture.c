#include "ppi_input_capture.h"
#include "boards.h"
#include "nrf_drv_timer.h"
#include "app_pwm.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_gpiote.h"

#define PWM_OUTPUT_PIN					3	//PWM输出引脚
#define PWM_INPUT_PIN					2	//PWM输入引脚

const nrf_drv_timer_t input_capture_timer0 = NRF_DRV_TIMER_INSTANCE(0);	//创建定时器0

static volatile bool duty_cycle_change;	//占空比改变   true-占空比改变   false-占空比不改变

APP_PWM_INSTANCE(PWM, 1);	//创建PWM实例

/*******************************************************************************
  * @brief		PWM占空比改变完成时执行的PWM回调函数
  * @param		pwm_id - PWM实例ID
  * @retval		None
  * @attention	只支持静态配置，以防止启动程序自定义共享资源
*******************************************************************************/
static void app_pwm_callback(uint32_t pwm_id)
{
	duty_cycle_change = true;	//占空比改变
}

/*******************************************************************************
  * @brief		PPI之输入捕获初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_input_capture_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_drv_timer_config_t timer0_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器0配置参数（默认配置参数）
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//配置引脚以使用GPIO IN或PORT EVENT检测引脚上的下降沿
	app_pwm_config_t pwm_configuration_parameter = APP_PWM_DEFAULT_CONFIG_2CH(2000000L, PWM_OUTPUT_PIN, BSP_LED_1);	//2个频道的PWM，周期5ms，通过PWM输出引脚和LED2输出
	nrf_ppi_channel_t ppi_channel;	//PPI通道
	
	timer0_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//计数器模式
	error_code = nrf_drv_timer_init(&input_capture_timer0, &timer0_configuration_parameter, NULL);	//定时器0初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_gpiote_init();	//NRF初始化GPIOTE模块
	APP_ERROR_CHECK(error_code);	//错误检查
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//上拉状态
	error_code = nrf_drv_gpiote_in_init(PWM_INPUT_PIN, &gpiote_in_config, NULL);	//NRF初始化GPIOTE输入引脚
	APP_ERROR_CHECK(error_code);	//错误检查
	nrf_drv_gpiote_in_event_enable(PWM_INPUT_PIN, true);	//使能GPIOTE输入引脚事件
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel);	//分配未使用的PPI通道
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel, nrf_drv_gpiote_in_event_addr_get(PWM_INPUT_PIN), 
				 nrf_drv_timer_task_address_get(&input_capture_timer0, NRF_TIMER_TASK_COUNT));	//分配EEP事件终点和TEP任务终点
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel);	//使能PPI通道，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
	
	pwm_configuration_parameter.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;	//引脚上激活状态的极性
	error_code = app_pwm_init(&PWM, &pwm_configuration_parameter, app_pwm_callback);	//初始化PWM实例输出
	APP_ERROR_CHECK(error_code);	//错误代码检查
	app_pwm_enable(&PWM);	//使能PWM实例
	while(app_pwm_channel_duty_set(&PWM, 0, 10) == NRF_ERROR_BUSY);	//设置占空比,直至成功
	
	nrf_drv_timer_enable(&input_capture_timer0);	//使能定时器0
}
