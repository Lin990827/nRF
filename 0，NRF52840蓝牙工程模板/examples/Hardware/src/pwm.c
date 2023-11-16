#include "pwm.h"
#include "led.h"
#include "nrf_drv_pwm.h"
#include "app_error.h"

static uint16_t pwm_count_up_sequencexian[4] = {0x1088, 0x1588, 0x1A88, 0x1F88};	//PWM向上计数序列
static nrf_drv_pwm_t pwm0 = NRF_DRV_PWM_INSTANCE(0);	//创建PWM0驱动
static uint8_t pwm_period;	//周期
static nrf_pwm_values_individual_t sequencexian;	//PWM计数序列
static nrf_pwm_sequence_t pwm_duty_cycle_sequencexian = 
{
	.values.p_individual = &sequencexian,	//设置（PWM序列起始地址）PWM的占空比
	.length = NRF_PWM_VALUES_LENGTH(sequencexian),	//PWM序列占空比个数
	.repeats = 0,	//加载到比较寄存器的采样之间的额外PWM周期数
	.end_delay = 0	//在PWM周期序列后增加的时间
};	//PWM占空比序列

/*******************************************************************************
  * @brief		PWM向上计数初始化（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_count_up_init_register(void)
{
	NRF_PWM0->PSEL.OUT[0] = (LED1 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED1与PWM输出通道0连接
	NRF_PWM0->PSEL.OUT[1] = (LED2 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED2与PWM输出通道1连接
	NRF_PWM0->PSEL.OUT[2] = (LED3 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED3与PWM输出通道2连接
	NRF_PWM0->PSEL.OUT[3] = (LED4 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED4与PWM输出通道3连接
	
	NRF_PWM0->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);	//向上计数模式
	NRF_PWM0->PRESCALER = (PWM_PRESCALER_PRESCALER_DIV_1 << PWM_PRESCALER_PRESCALER_Pos);	//设置预分频系数：1分频
	NRF_PWM0->COUNTERTOP = (25000 << PWM_COUNTERTOP_COUNTERTOP_Pos);	//脉冲计数器计数到的值
	NRF_PWM0->LOOP = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);	//失能循环回放次数
	NRF_PWM0->DECODER = (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos) | (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);	//解码器配置：独立模式、 刷新计数
	NRF_PWM0->SEQ[0].PTR = ((uint32_t)(pwm_count_up_sequencexian) << PWM_SEQ_PTR_PTR_Pos);	//设置（PWM序列起始地址）PWM的占空比：比较寄存器的值，级性
	NRF_PWM0->SEQ[0].CNT = ((sizeof(pwm_count_up_sequencexian) / sizeof(uint16_t)) << PWM_SEQ_CNT_CNT_Pos);	//PWM序列占空比个数
	NRF_PWM0->SEQ[0].REFRESH = 0;	//加载到比较寄存器的采样之间的额外PWM周期数
	NRF_PWM0->SEQ[0].ENDDELAY = 0;	//在PWM周期序列后增加的时间
	
	NRF_PWM0->ENABLE = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);	//使能PWM模块
	NRF_PWM0->TASKS_SEQSTART[0] = 1;	//从序列0开始在所有启用的通道上加载第一个PWM值，并以SEQ[0].REFRESH或解码器MODE中定义的速率开始播放该序列
}

/*******************************************************************************
  * @brief		PWM向下计数初始化（寄存器）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_count_down_init_register(void)
{
	NRF_PWM0->PSEL.OUT[0] = (LED1 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED1与PWM输出通道0连接
	NRF_PWM0->PSEL.OUT[1] = (LED2 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED2与PWM输出通道1连接
	NRF_PWM0->PSEL.OUT[2] = (LED3 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED3与PWM输出通道2连接
	NRF_PWM0->PSEL.OUT[3] = (LED4 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);	//LED4与PWM输出通道3连接
	
	NRF_PWM0->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);	//向上计数模式
	NRF_PWM0->PRESCALER = (PWM_PRESCALER_PRESCALER_DIV_1 << PWM_PRESCALER_PRESCALER_Pos);	//设置预分频系数：1分频
	NRF_PWM0->COUNTERTOP = (25000 << PWM_COUNTERTOP_COUNTERTOP_Pos);	//脉冲计数器计数到的值
	NRF_PWM0->LOOP = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);	//失能循环回放次数
	NRF_PWM0->DECODER = (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos) | (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);	//解码器配置：独立模式、 刷新计数
	NRF_PWM0->SEQ[0].PTR = ((uint32_t)(pwm_count_up_sequencexian) << PWM_SEQ_PTR_PTR_Pos);	//设置（PWM序列起始地址）PWM的占空比：比较寄存器的值，级性
	NRF_PWM0->SEQ[0].CNT = ((sizeof(pwm_count_up_sequencexian) / sizeof(uint16_t)) << PWM_SEQ_CNT_CNT_Pos);	//PWM序列占空比个数
	NRF_PWM0->SEQ[0].REFRESH = 0;	//加载到比较寄存器的采样之间的额外PWM周期数
	NRF_PWM0->SEQ[0].ENDDELAY = 0;	//在PWM周期序列后增加的时间
	
	NRF_PWM0->ENABLE = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);	//使能PWM模块
	NRF_PWM0->TASKS_SEQSTART[0] = 1;	//从序列0开始在所有启用的通道上加载第一个PWM值，并以SEQ[0].REFRESH或解码器MODE中定义的速率开始播放该序列
}

/*******************************************************************************
  * @brief		PWM共同模式简单回放（组件库）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_common_mode_simple_playback_component(void)
{
	nrf_drv_pwm_config_t pwm0_config = 
	{
		.output_pins = 
		{
			LED1 | NRF_DRV_PWM_PIN_INVERTED,	//LED1与PWM输出通道0连接
            LED2 | NRF_DRV_PWM_PIN_INVERTED,	//LED2与PWM输出通道1连接
            LED3 | NRF_DRV_PWM_PIN_INVERTED,	//LED3与PWM输出通道2连接
            LED4 | NRF_DRV_PWM_PIN_INVERTED,	//LED4与PWM输出通道3连接
		},
		.irq_priority = APP_IRQ_PRIORITY_LOWEST,	//中断优先级
		.base_clock = NRF_PWM_CLK_1MHz,	//PWM时钟频率
		.count_mode = NRF_PWM_MODE_UP,	//计数模式：向上计数
		.top_value = 25000,	//脉冲计数器计数到的值
		.load_mode = NRF_PWM_LOAD_COMMON,	//RAM加载序列数据的方式：前半字(16位)用于所有PWM通道0..3
		.step_mode = NRF_PWM_STEP_AUTO	//活动序列的推进方式：自动下一步
	};	//PWM配置
	uint16_t sequencexian_value[] = {0x1388, 0x1388, 0x1388, 0x1388};	//PWM计数序列
	nrf_pwm_sequence_t pwm_sequencexian = 
	{
		.values.p_common = sequencexian_value,	//设置（PWM序列起始地址）PWM的占空比
		.length = NRF_PWM_VALUES_LENGTH(sequencexian_value),	//PWM序列占空比个数
		.repeats = 0,	//加载到比较寄存器的采样之间的额外PWM周期数
		.end_delay = 0	//在PWM周期序列后增加的时间
	};	//PWM占空比序列
	
	APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL));	//初始化PWM驱动
	(void)nrf_drv_pwm_simple_playback(&pwm0, &pwm_sequencexian, 1, NRF_DRV_PWM_FLAG_LOOP);	//启动单序列回放功能
}

/*******************************************************************************
  * @brief		PWM共同模式复杂回放（组件库）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_common_mode_complex_playback_component(void)
{
	nrf_drv_pwm_config_t pwm0_config = 
	{
		.output_pins = 
		{
			LED1 | NRF_DRV_PWM_PIN_INVERTED,	//LED1与PWM输出通道0连接
            LED2 | NRF_DRV_PWM_PIN_INVERTED,	//LED2与PWM输出通道1连接
            LED3 | NRF_DRV_PWM_PIN_INVERTED,	//LED3与PWM输出通道2连接
            LED4 | NRF_DRV_PWM_PIN_INVERTED,	//LED4与PWM输出通道3连接
		},
		.irq_priority = APP_IRQ_PRIORITY_LOWEST,	//中断优先级
		.base_clock = NRF_PWM_CLK_1MHz,	//PWM时钟频率
		.count_mode = NRF_PWM_MODE_UP,	//计数模式：向上计数
		.top_value = 25000,	//脉冲计数器计数到的值
		.load_mode = NRF_PWM_LOAD_COMMON,	//RAM加载序列数据的方式：前半字(16位)用于所有PWM通道0..3
		.step_mode = NRF_PWM_STEP_AUTO	//活动序列的推进方式：自动下一步
	};	//PWM0配置
	uint16_t value = 0;	//PWM计数序列值
	uint16_t sequencexian0_value[25];	//PWM计数序列0
	uint16_t sequencexian1_value[] = {0, 0x8000, 0, 0x8000, 0, 0};	//PWM计数序列1
	nrf_pwm_sequence_t pwm_sequencexian0;	//PWM占空比序列0
	nrf_pwm_sequence_t pwm_sequencexian1;	//PWM占空比序列1
	
	APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL));	//初始化PWM驱动
	for(uint8_t i = 0; i < 25; i++)
	{
		value += 1000;
		sequencexian0_value[i] = value;	//赋值PWM计数序列
	}
	pwm_sequencexian0.values.p_common = sequencexian0_value;	//设置（PWM序列0起始地址）PWM的占空比
	pwm_sequencexian0.length = NRF_PWM_VALUES_LENGTH(sequencexian0_value);	//PWM序列0占空比个数
	pwm_sequencexian0.repeats = 1;	//加载到比较寄存器的采样之间的额外PWM周期数（PWM周期*1执行一次序列）
	pwm_sequencexian0.end_delay = 0;	//在PWM周期序列后增加的时间
	
	pwm_sequencexian1.values.p_common = sequencexian1_value;	//设置（PWM序列1起始地址）PWM的占空比
	pwm_sequencexian1.length = NRF_PWM_VALUES_LENGTH(sequencexian0_value);	//PWM序列1占空比个数
	pwm_sequencexian1.repeats = 40;	//加载到比较寄存器的采样之间的额外PWM周期数（PWM周期*40执行一次序列）
	pwm_sequencexian1.end_delay = 0;	//在PWM周期序列后增加的时间
	
	(void)nrf_drv_pwm_complex_playback(&pwm0, &pwm_sequencexian0, &pwm_sequencexian1, 1, NRF_DRV_PWM_FLAG_LOOP);	//启动复杂回放功能，串联两个序列
}

/*******************************************************************************
  * @brief		PWM共同模式简单不回放（组件库）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_common_mode_simple_no_playback_component(void)
{
	nrf_drv_pwm_config_t pwm0_config = 
	{
		.output_pins = 
		{
			LED1 | NRF_DRV_PWM_PIN_INVERTED,	//LED1与PWM输出通道0连接
            LED2 | NRF_DRV_PWM_PIN_INVERTED,	//LED2与PWM输出通道1连接
            LED3 | NRF_DRV_PWM_PIN_INVERTED,	//LED3与PWM输出通道2连接
            LED4 | NRF_DRV_PWM_PIN_INVERTED,	//LED4与PWM输出通道3连接
		},
		.irq_priority = APP_IRQ_PRIORITY_LOWEST,	//中断优先级
		.base_clock = NRF_PWM_CLK_1MHz,	//PWM时钟频率
		.count_mode = NRF_PWM_MODE_UP,	//计数模式：向上计数
		.top_value = 25000,	//脉冲计数器计数到的值
		.load_mode = NRF_PWM_LOAD_COMMON,	//RAM加载序列数据的方式：前半字(16位)用于所有PWM通道0..3
		.step_mode = NRF_PWM_STEP_AUTO	//活动序列的推进方式：自动下一步
	};	//PWM配置
	uint16_t sequencexian_value[] = {0, 0x8000, 0, 0x8000, 0, 0};	//PWM计数序列
	nrf_pwm_sequence_t pwm_sequencexian = 
	{
		.values.p_common = sequencexian_value,	//设置（PWM序列起始地址）PWM的占空比
		.length = NRF_PWM_VALUES_LENGTH(sequencexian_value),	//PWM序列占空比个数
		.repeats = 40,	//加载到比较寄存器的采样之间的额外PWM周期数
		.end_delay = 40	//在PWM周期序列后增加的时间
	};	//PWM占空比序列
	
	APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL));	//初始化PWM驱动
	(void)nrf_drv_pwm_simple_playback(&pwm0, &pwm_sequencexian, 0, NRF_DRV_PWM_FLAG_STOP);	//启动单序列停止回放功能
}

/*******************************************************************************
  * @brief		PWM独立模式简单不回调（组件库）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_standalone_mode_simple_no_callback_component(void)
{
	nrf_drv_pwm_config_t pwm0_config = 
	{
		.output_pins = 
		{
			LED1 | NRF_DRV_PWM_PIN_INVERTED,	//LED1与PWM输出通道0连接
            LED2 | NRF_DRV_PWM_PIN_INVERTED,	//LED2与PWM输出通道1连接
            LED3 | NRF_DRV_PWM_PIN_INVERTED,	//LED3与PWM输出通道2连接
            LED4 | NRF_DRV_PWM_PIN_INVERTED,	//LED4与PWM输出通道3连接
		},
		.irq_priority = APP_IRQ_PRIORITY_LOWEST,	//中断优先级
		.base_clock = NRF_PWM_CLK_1MHz,	//PWM时钟频率
		.count_mode = NRF_PWM_MODE_UP,	//计数模式：向上计数
		.top_value = 25000,	//脉冲计数器计数到的值
		.load_mode = NRF_PWM_LOAD_INDIVIDUAL,	//RAM加载序列数据的方式：独立模式
		.step_mode = NRF_PWM_STEP_AUTO	//活动序列的推进方式：自动下一步
	};	//PWM配置
	nrf_pwm_values_individual_t sequencexian_value[] = {
		{ 0x8000,      0,      0,      0 },
        {      0, 0x8000,      0,      0 },
        {      0,      0, 0x8000,      0 },
        {      0,      0,      0, 0x8000 }};	//PWM计数序列
	nrf_pwm_sequence_t pwm_sequencexian = 
	{
		.values.p_individual = sequencexian_value,	//设置（PWM序列起始地址）PWM的占空比
		.length = NRF_PWM_VALUES_LENGTH(sequencexian_value),	//PWM序列占空比个数
		.repeats = 10,	//加载到比较寄存器的采样之间的额外PWM周期数
		.end_delay = 0	//在PWM周期序列后增加的时间
	};	//PWM占空比序列
	
	APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm0, &pwm0_config, NULL));	//初始化PWM驱动
	(void)nrf_drv_pwm_simple_playback(&pwm0, &pwm_sequencexian, 1, NRF_DRV_PWM_FLAG_LOOP);	//启动单序列回放功能
}

/*******************************************************************************
  * @brief		PWM事件回调函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
static void pwm_handler(nrfx_pwm_evt_type_t event_type)
{
	uint8_t channel = pwm_period >> 1;
	bool down = pwm_period & 1;
	bool next_period = false;
	uint16_t *p_channel = (uint16_t *)&sequencexian;
	uint16_t value = p_channel[channel];
	
	if(event_type == NRF_DRV_PWM_EVT_FINISHED)	//序列回放完成事件
	{
		if(down)
		{
			value -= 500;
			if(value == 0)
			{
				next_period = true;
			}
		}
		else
		{
			value += 500;
			if(value >= 25000)
			{
				next_period = true;
			}
		}
		p_channel[channel] = value;
		if(next_period)
		{
			if(++pwm_period >= 2 * NRF_PWM_CHANNEL_COUNT)
			{
				pwm_period = 0;
			}
		}
	}
}

/*******************************************************************************
  * @brief		PWM独立模式简单回调（组件库）函数
  * @param		None
  * @retval		None
  * @attention	PWM周期 = PWM时钟的周期 * 脉冲计数器计数到的值
*******************************************************************************/
void pwm_standalone_mode_simple_callback_component(void)
{
	nrf_drv_pwm_config_t pwm0_config = 
	{
		.output_pins = 
		{
			LED1 | NRF_DRV_PWM_PIN_INVERTED,	//LED1与PWM输出通道0连接
            LED2 | NRF_DRV_PWM_PIN_INVERTED,	//LED2与PWM输出通道1连接
            LED3 | NRF_DRV_PWM_PIN_INVERTED,	//LED3与PWM输出通道2连接
            LED4 | NRF_DRV_PWM_PIN_INVERTED,	//LED4与PWM输出通道3连接
		},
		.irq_priority = APP_IRQ_PRIORITY_LOWEST,	//中断优先级
		.base_clock = NRF_PWM_CLK_1MHz,	//PWM时钟频率
		.count_mode = NRF_PWM_MODE_UP,	//计数模式：向上计数
		.top_value = 25000,	//脉冲计数器计数到的值
		.load_mode = NRF_PWM_LOAD_INDIVIDUAL,	//RAM加载序列数据的方式：独立模式
		.step_mode = NRF_PWM_STEP_AUTO	//活动序列的推进方式：自动下一步
	};	//PWM配置
	
	sequencexian.channel_0 = 0;	//通道0占空比值
	sequencexian.channel_1 = 0;	//通道1占空比值
	sequencexian.channel_2 = 0;	//通道2占空比值
	sequencexian.channel_3 = 0;	//通道3占空比值
	pwm_period = 0;
	APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm0, &pwm0_config, pwm_handler));	//初始化PWM驱动
	(void)nrf_drv_pwm_simple_playback(&pwm0, &pwm_duty_cycle_sequencexian, 1, NRF_DRV_PWM_FLAG_LOOP);	//启动单序列回放功能
}
