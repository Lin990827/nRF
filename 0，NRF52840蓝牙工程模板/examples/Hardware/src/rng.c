#include "rng.h"

/*******************************************************************************
  * @brief		随机数初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rng_init(void)
{
	ret_code_t error_code;	//错误代码
	#if RNG_CONFIGURATION_PARAMETER
	/*RNG配置参数*/
	nrf_drv_rng_config_t rng_configuration_parameter = {1,	//数据修正（true：有数据修正   false：没有数据修正）
														1	//中断优先级（优先级越低，中断级别越高）
													   };
	
	error_code = nrf_drv_rng_init(&rng_configuration_parameter);	//初始化RNG模块（自定义RNG配置参数）
	#else
	nrf_drv_rng_config_t *rng_configuration_parameter = NULL;	//RNG默认配置参数
	
	error_code = nrf_drv_rng_init(rng_configuration_parameter);	//初始化RNG模块（RNG默认配置参数）
	#endif
	APP_ERROR_CHECK(error_code);	//错误检查
}

/*******************************************************************************
  * @brief		获取随机数函数
  * @param		buffer - 存储随机数的缓冲区
				len - 随机数缓冲区长度
  * @retval		实际存储随机数长度
  * @attention	None
*******************************************************************************/
uint8_t get_random_number(uint8_t *buffer,uint8_t len)
{
	uint8_t now_len;	//保存实际存储随机数长度
	uint8_t pool_len;	//保存当前随机数内存池中可用长度
	ret_code_t error_code;	//错误代码
	
	nrf_drv_rng_bytes_available(&pool_len);	//获取当前可用的随机数长度
	now_len = MIN(len, pool_len);	//实际存储随机数长度取随机数内存池中可用长度和随机数缓冲区长度最小值
	error_code = nrf_drv_rng_rand(buffer, now_len);	//将随机数保存到缓冲区
	APP_ERROR_CHECK(error_code);	//错误检查
	return now_len;
}
