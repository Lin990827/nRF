#include "saadc.h"
#include "nrf_drv_saadc.h"
#include "uart.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"

#define SAMPLE_BUFFER_SIZE				2	//采样缓冲区大小
#define SAMPLE_MEMORY_POOL_SIZE			6	//采样内存池大小

static nrf_saadc_value_t sample_buffer[SAMPLE_BUFFER_SIZE];	//采样缓冲区
static nrf_saadc_value_t sample_memory_pool[2][SAMPLE_MEMORY_POOL_SIZE];	//采样内存池
static uint32_t saadc_event_count = 0;	//saadc事件触发次数
static const nrf_drv_timer_t timer0 = NRF_DRV_TIMER_INSTANCE(0);	//创建定时器0

 /*******************************************************************************
  * @brief		SAADC驱动程序事件处理函数
  * @param		p_event - 指向SAADC驱动程序事件的指针
  * @retval		None
  * @attention	None
*******************************************************************************/
static void saadc_event_handler(nrfx_saadc_evt_t const *p_event)
{
	float value;
	ret_code_t error_code;	//错误代码
	
	if(p_event->type == NRFX_SAADC_EVT_DONE)	//采样缓冲区填充完成事件
	{
		error_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLE_MEMORY_POOL_SIZE);	//将采样数据转换到采样缓冲区
		APP_ERROR_CHECK(error_code);	//错误代码检查
		printf("SAADC event count:%ld\r\n",saadc_event_count);
		for(uint8_t i = 0; i < SAMPLE_MEMORY_POOL_SIZE; i++)
		{
			value = p_event->data.done.p_buffer[i] * 3.6 / 1024;
			printf("SAADC[%d]:%fmV\r\n",i,value*1000);
		}
		saadc_event_count++;
	}
}

/*******************************************************************************
  * @brief		SAADC单次采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_single_sample_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_configuration_parameter = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);	//在单端模式下将ADC通道配置设置为正端输入AIN2
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_configuration_parameter);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
}

/*******************************************************************************
  * @brief		SAADC差分采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_differential_sample_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_configuration_parameter = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_DIFFERENTIAL(NRF_SAADC_INPUT_AIN2, NRF_SAADC_INPUT_AIN0);	//在差分模式下将ADC通道配置设置为正端输入AIN2，负端输入AIN0
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_configuration_parameter);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
}

/*******************************************************************************
  * @brief		SAADC EasyDMA单缓冲中断单通道采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_easydma_single_buffer_interrupt_single_sample_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_configuration_parameter = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);	//在单端模式下将ADC通道配置设置为正端输入AIN2
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_configuration_parameter);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_buffer_convert(sample_buffer, SAMPLE_BUFFER_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
}

/*******************************************************************************
  * @brief		SAADC EasyDMA单缓冲中断多通道采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_easydma_single_buffer_interrupt_more_sample_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_channel0_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);	//在单端模式下将ADC通道0配置设置为正端输入AIN2
	nrf_saadc_channel_config_t saadc_channel1_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);	//在单端模式下将ADC通道1配置设置为正端输入AIN0
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_channel0_config);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
	error_code = nrf_drv_saadc_channel_init(1, &saadc_channel1_config);	//初始化SAADC通道1
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_buffer_convert(sample_buffer, SAMPLE_BUFFER_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
}

/*******************************************************************************
  * @brief		SAADC EasyDMA双缓冲中断多通道采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_easydma_double_buffer_interrupt_more_sample_component_init(void)
{
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_channel0_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);	//在单端模式下将ADC通道0配置设置为正端输入AIN2
	nrf_saadc_channel_config_t saadc_channel1_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);	//在单端模式下将ADC通道1配置设置为正端输入AIN0
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_channel0_config);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
	error_code = nrf_drv_saadc_channel_init(1, &saadc_channel1_config);	//初始化SAADC通道1
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_buffer_convert(sample_memory_pool[0], SAMPLE_MEMORY_POOL_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
	error_code = nrf_drv_saadc_buffer_convert(sample_memory_pool[1], SAMPLE_MEMORY_POOL_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
}

/*******************************************************************************
  * @brief		SAADC EasyDMA之PPI启动双缓冲中断采样初始化（组件库）函数
  * @param		None
  * @retval		None
  * @attention	输出结果：RESULT = [V(P) - V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
*******************************************************************************/
void saadc_easydma_ppi_start_double_buffer_interrupt_sample_component_init(void)
{
	uint32_t ticks;	//定时器刻度
	uint32_t timer_compare_event_addr;	//定时器0比较事件地址
	uint32_t saadc_sample_task_addr;	//SAADC采样任务地址
	ret_code_t error_code;	//错误代码
	nrf_saadc_channel_config_t saadc_configuration_parameter = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);	//在单端模式下将ADC通道配置设置为正端输入AIN2
	nrf_ppi_channel_t ppi_channel;	//PPI通道
	nrf_drv_timer_config_t timer0_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//定时器0配置参数（默认配置参数）
	
	error_code = nrf_drv_saadc_init(NULL, saadc_event_handler);	//初始化SAADC
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_channel_init(0, &saadc_configuration_parameter);	//初始化SAADC通道0
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	error_code = nrf_drv_saadc_buffer_convert(sample_memory_pool[0], SAMPLE_MEMORY_POOL_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
	error_code = nrf_drv_saadc_buffer_convert(sample_memory_pool[1], SAMPLE_MEMORY_POOL_SIZE);	//将采样数据转换到采样缓冲区
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	timer0_configuration_parameter.bit_width = NRF_TIMER_BIT_WIDTH_32;	//定时器0：32位宽定时器
	error_code = nrf_drv_timer_init(&timer0, &timer0_configuration_parameter, NULL);	//定时器0初始化
	APP_ERROR_CHECK(error_code);	//错误代码检查
	
	ticks = nrf_drv_timer_ms_to_ticks(&timer0, 400);	//将时间(以毫秒为单位)转换为定时器刻度，总采样时间 400*SAMPLE_MEMORY_POOL_SIZE ms
	nrf_drv_timer_extended_compare(&timer0, NRF_TIMER_CC_CHANNEL0, ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);	//设置扩展比较模式下的定时器通道
	nrf_drv_timer_enable(&timer0);	//使能定时器0
	
	timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&timer0, NRF_TIMER_CC_CHANNEL0);	//返回定时器0比较事件的地址
	saadc_sample_task_addr = nrf_drv_saadc_sample_task_get();	//获取SAADC采集任务地址
	
	error_code = nrf_drv_ppi_init();	//初始化PPI模块
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel);	//分配未使用的PPI通道
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_assign(ppi_channel, timer_compare_event_addr, saadc_sample_task_addr);	//分配EEP事件终点（定时器0比较事件）和TEP任务终点（SAADC采集任务）
	APP_ERROR_CHECK(error_code);	//错误检查
	error_code = nrf_drv_ppi_channel_enable(ppi_channel);	//使能PPI通道，开启PPI
	APP_ERROR_CHECK(error_code);	//错误检查
}
