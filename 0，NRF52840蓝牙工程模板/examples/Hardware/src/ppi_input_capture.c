#include "ppi_input_capture.h"
#include "boards.h"
#include "nrf_drv_timer.h"
#include "app_pwm.h"
#include "nrfx_ppi.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_gpiote.h"

#define PWM_OUTPUT_PIN					3	//PWM�������
#define PWM_INPUT_PIN					2	//PWM��������

const nrf_drv_timer_t input_capture_timer0 = NRF_DRV_TIMER_INSTANCE(0);	//������ʱ��0

static volatile bool duty_cycle_change;	//ռ�ձȸı�   true-ռ�ձȸı�   false-ռ�ձȲ��ı�

APP_PWM_INSTANCE(PWM, 1);	//����PWMʵ��

/*******************************************************************************
  * @brief		PWMռ�ձȸı����ʱִ�е�PWM�ص�����
  * @param		pwm_id - PWMʵ��ID
  * @retval		None
  * @attention	ֻ֧�־�̬���ã��Է�ֹ���������Զ��干����Դ
*******************************************************************************/
static void app_pwm_callback(uint32_t pwm_id)
{
	duty_cycle_change = true;	//ռ�ձȸı�
}

/*******************************************************************************
  * @brief		PPI֮���벶���ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_input_capture_init(void)
{
	ret_code_t error_code;	//�������
	nrf_drv_timer_config_t timer0_configuration_parameter = NRF_DRV_TIMER_DEFAULT_CONFIG;	//��ʱ��0���ò�����Ĭ�����ò�����
	nrf_drv_gpiote_in_config_t gpiote_in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	//����������ʹ��GPIO IN��PORT EVENT��������ϵ��½���
	app_pwm_config_t pwm_configuration_parameter = APP_PWM_DEFAULT_CONFIG_2CH(2000000L, PWM_OUTPUT_PIN, BSP_LED_1);	//2��Ƶ����PWM������5ms��ͨ��PWM������ź�LED2���
	nrf_ppi_channel_t ppi_channel;	//PPIͨ��
	
	timer0_configuration_parameter.mode = NRF_TIMER_MODE_COUNTER;	//������ģʽ
	error_code = nrf_drv_timer_init(&input_capture_timer0, &timer0_configuration_parameter, NULL);	//��ʱ��0��ʼ��
	APP_ERROR_CHECK(error_code);	//���������
	
	error_code = nrf_drv_gpiote_init();	//NRF��ʼ��GPIOTEģ��
	APP_ERROR_CHECK(error_code);	//������
	gpiote_in_config.pull = NRF_GPIO_PIN_PULLUP;	//����״̬
	error_code = nrf_drv_gpiote_in_init(PWM_INPUT_PIN, &gpiote_in_config, NULL);	//NRF��ʼ��GPIOTE��������
	APP_ERROR_CHECK(error_code);	//������
	nrf_drv_gpiote_in_event_enable(PWM_INPUT_PIN, true);	//ʹ��GPIOTE���������¼�
	
	error_code = nrf_drv_ppi_init();	//��ʼ��PPIģ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_alloc(&ppi_channel);	//����δʹ�õ�PPIͨ��
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_assign(ppi_channel, nrf_drv_gpiote_in_event_addr_get(PWM_INPUT_PIN), 
				 nrf_drv_timer_task_address_get(&input_capture_timer0, NRF_TIMER_TASK_COUNT));	//����EEP�¼��յ��TEP�����յ�
	APP_ERROR_CHECK(error_code);	//������
	error_code = nrf_drv_ppi_channel_enable(ppi_channel);	//ʹ��PPIͨ��������PPI
	APP_ERROR_CHECK(error_code);	//������
	
	pwm_configuration_parameter.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;	//�����ϼ���״̬�ļ���
	error_code = app_pwm_init(&PWM, &pwm_configuration_parameter, app_pwm_callback);	//��ʼ��PWMʵ�����
	APP_ERROR_CHECK(error_code);	//���������
	app_pwm_enable(&PWM);	//ʹ��PWMʵ��
	while(app_pwm_channel_duty_set(&PWM, 0, 10) == NRF_ERROR_BUSY);	//����ռ�ձ�,ֱ���ɹ�
	
	nrf_drv_timer_enable(&input_capture_timer0);	//ʹ�ܶ�ʱ��0
}
