#include "rng.h"

/*******************************************************************************
  * @brief		�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void rng_init(void)
{
	ret_code_t error_code;	//�������
	#if RNG_CONFIGURATION_PARAMETER
	/*RNG���ò���*/
	nrf_drv_rng_config_t rng_configuration_parameter = {1,	//����������true������������   false��û������������
														1	//�ж����ȼ������ȼ�Խ�ͣ��жϼ���Խ�ߣ�
													   };
	
	error_code = nrf_drv_rng_init(&rng_configuration_parameter);	//��ʼ��RNGģ�飨�Զ���RNG���ò�����
	#else
	nrf_drv_rng_config_t *rng_configuration_parameter = NULL;	//RNGĬ�����ò���
	
	error_code = nrf_drv_rng_init(rng_configuration_parameter);	//��ʼ��RNGģ�飨RNGĬ�����ò�����
	#endif
	APP_ERROR_CHECK(error_code);	//������
}

/*******************************************************************************
  * @brief		��ȡ���������
  * @param		buffer - �洢������Ļ�����
				len - ���������������
  * @retval		ʵ�ʴ洢���������
  * @attention	None
*******************************************************************************/
uint8_t get_random_number(uint8_t *buffer,uint8_t len)
{
	uint8_t now_len;	//����ʵ�ʴ洢���������
	uint8_t pool_len;	//���浱ǰ������ڴ���п��ó���
	ret_code_t error_code;	//�������
	
	nrf_drv_rng_bytes_available(&pool_len);	//��ȡ��ǰ���õ����������
	now_len = MIN(len, pool_len);	//ʵ�ʴ洢���������ȡ������ڴ���п��ó��Ⱥ������������������Сֵ
	error_code = nrf_drv_rng_rand(buffer, now_len);	//����������浽������
	APP_ERROR_CHECK(error_code);	//������
	return now_len;
}
