#include "ppi_pwm_output_application.h"
#include "boards.h"
#include "app_pwm.h"
#include "nrf_delay.h"

APP_PWM_INSTANCE(PWM1, 1);	//创建PWM1实例
static volatile bool duty_cycle_change;	//占空比改变   true-占空比改变   false-占空比不改变

/*******************************************************************************
  * @brief		PPI之PWM输出应用（寄存器）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_pwm_output_application_register_init(void)
{
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (13 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos)	//初始化输出高电平
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入	
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//任务模式下：OUT[n]任务输出为翻转电平；事件模式下：任意管脚变化都能产生IN[n]输入事件
							| (14 << GPIOTE_CONFIG_PSEL_Pos)	//设置与SET[N]，CLR[n]和OUT[n]的任务和IN[n]的事件相绑定的GPIO管脚号
							| (GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos)	//初始化输出高电平
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//任务模式：把PSEL绑定的对应管脚设置为输出，触发SET[N]，CLR[n]或者OUT[n]任务会执行POLARITY域中指定的
																					//动作。一旦配置为任务模式后，GPIOTE模块将获取该引脚的控制权，该引脚不能再作为常规输出引脚从GPIO模块写入
	
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;	//定时器0：定时器模式
	NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;	//定时器0：32位宽定时器
	NRF_TIMER0->PRESCALER = 4;	//定时器0：预分频值4-1
	NRF_TIMER0->SHORTS = (TIMER_SHORTS_COMPARE1_CLEAR_Enabled << TIMER_SHORTS_COMPARE1_CLEAR_Pos);	//定时器1：快捷方式注册COMPARE[i]事件和CLEAR任务之间的快捷方式
	NRF_TIMER0->CC[0] = 100;	//占空比，100us
	NRF_TIMER0->CC[1] = 5000;	//方波周期，5ms
	NRF_TIMER0->CC[2] = 4900;	//占空比，4.9ms
	NRF_TIMER0->CC[3] = 5000;	//方波周期，5ms
	NRF_TIMER0->TASKS_START = 1;	//开启定时器0
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[0]);	//PPI通道0事件终点（定时器0比较事件0）
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[0]);	//PPI通道0任务终点（GPIOTE输出任务（OUT[0]任务））
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[1]);	//PPI通道1事件终点（定时器0比较事件1）
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[0]);	//PPI通道1任务终点（GPIOTE输出任务（OUT[0]任务））
	NRF_PPI->CH[2].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[2]);	//PPI通道2事件终点（定时器0比较事件2）
	NRF_PPI->CH[2].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPI通道2任务终点（GPIOTE输出任务（OUT[1]任务））
	NRF_PPI->CH[3].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[3]);	//PPI通道3事件终点（定时器0比较事件3）
	NRF_PPI->CH[3].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPI通道3任务终点（GPIOTE输出任务（OUT[1]任务））
	NRF_PPI->CHENSET = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos) | (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos) | (PPI_CHEN_CH2_Enabled << PPI_CHEN_CH2_Pos) | (PPI_CHEN_CH3_Enabled << PPI_CHEN_CH3_Pos);	//通道使能设置寄存器
}

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
  * @brief		PPI之PWM输出应用（组件库）初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_pwm_output_application_component_init(void)
{
	ret_code_t error_code;	//错误代码
	app_pwm_config_t pwm1_configuration_parameter = APP_PWM_DEFAULT_CONFIG_2CH(5000L, BSP_LED_0, BSP_LED_1);	//2个频道的PWM，周期5ms，通过LED1和LED2输出
	
	pwm1_configuration_parameter.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;	//引脚上激活状态的极性
	error_code = app_pwm_init(&PWM1, &pwm1_configuration_parameter, app_pwm_callback);	//初始化PWM实例输出
	APP_ERROR_CHECK(error_code);	//错误代码检查
	app_pwm_enable(&PWM1);	//使能PWM实例
}

/*******************************************************************************
  * @brief		设置PPI之PWM输出应用（组件库）占空比函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void set_ppi_pwm_output_application_component_duty(void)
{
	uint32_t value;
	
	for(uint8_t i = 0; i < 40; i++)
	{
		value = (i < 20) ? (i * 5) : (100 - (i - 20) * 5);
		duty_cycle_change = false;	//占空比没改变
		while(app_pwm_channel_duty_set(&PWM1, 0, value) == NRF_ERROR_BUSY);	//设置占空比，直到PWM准备好
		while(!duty_cycle_change);	//等待PWM占空比改变完成时执行的PWM回调函数执行完成
		APP_ERROR_CHECK(app_pwm_channel_duty_set(&PWM1, 1, value));	//设置占空比
	}
}
