#include "ppi_pwm_output_application.h"
#include "boards.h"
#include "app_pwm.h"
#include "nrf_delay.h"

APP_PWM_INSTANCE(PWM1, 1);	//����PWM1ʵ��
static volatile bool duty_cycle_change;	//ռ�ձȸı�   true-ռ�ձȸı�   false-ռ�ձȲ��ı�

/*******************************************************************************
  * @brief		PPI֮PWM���Ӧ�ã��Ĵ�������ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_pwm_output_application_register_init(void)
{
	NRF_GPIOTE->CONFIG[0] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (13 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos)	//��ʼ������ߵ�ƽ
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��	
	
	NRF_GPIOTE->CONFIG[1] = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)	//����ģʽ�£�OUT[n]�������Ϊ��ת��ƽ���¼�ģʽ�£�����ܽű仯���ܲ���IN[n]�����¼�
							| (14 << GPIOTE_CONFIG_PSEL_Pos)	//������SET[N]��CLR[n]��OUT[n]�������IN[n]���¼���󶨵�GPIO�ܽź�
							| (GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos)	//��ʼ������ߵ�ƽ
							| (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);	//����ģʽ����PSEL�󶨵Ķ�Ӧ�ܽ�����Ϊ���������SET[N]��CLR[n]����OUT[n]�����ִ��POLARITY����ָ����
																					//������һ������Ϊ����ģʽ��GPIOTEģ�齫��ȡ�����ŵĿ���Ȩ�������Ų�������Ϊ����������Ŵ�GPIOģ��д��
	
	NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;	//��ʱ��0����ʱ��ģʽ
	NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;	//��ʱ��0��32λ��ʱ��
	NRF_TIMER0->PRESCALER = 4;	//��ʱ��0��Ԥ��Ƶֵ4-1
	NRF_TIMER0->SHORTS = (TIMER_SHORTS_COMPARE1_CLEAR_Enabled << TIMER_SHORTS_COMPARE1_CLEAR_Pos);	//��ʱ��1����ݷ�ʽע��COMPARE[i]�¼���CLEAR����֮��Ŀ�ݷ�ʽ
	NRF_TIMER0->CC[0] = 100;	//ռ�ձȣ�100us
	NRF_TIMER0->CC[1] = 5000;	//�������ڣ�5ms
	NRF_TIMER0->CC[2] = 4900;	//ռ�ձȣ�4.9ms
	NRF_TIMER0->CC[3] = 5000;	//�������ڣ�5ms
	NRF_TIMER0->TASKS_START = 1;	//������ʱ��0
	
	NRF_PPI->CH[0].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[0]);	//PPIͨ��0�¼��յ㣨��ʱ��0�Ƚ��¼�0��
	NRF_PPI->CH[0].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[0]);	//PPIͨ��0�����յ㣨GPIOTE�������OUT[0]���񣩣�
	NRF_PPI->CH[1].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[1]);	//PPIͨ��1�¼��յ㣨��ʱ��0�Ƚ��¼�1��
	NRF_PPI->CH[1].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[0]);	//PPIͨ��1�����յ㣨GPIOTE�������OUT[0]���񣩣�
	NRF_PPI->CH[2].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[2]);	//PPIͨ��2�¼��յ㣨��ʱ��0�Ƚ��¼�2��
	NRF_PPI->CH[2].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPIͨ��2�����յ㣨GPIOTE�������OUT[1]���񣩣�
	NRF_PPI->CH[3].EEP = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[3]);	//PPIͨ��3�¼��յ㣨��ʱ��0�Ƚ��¼�3��
	NRF_PPI->CH[3].TEP = (uint32_t)(&NRF_GPIOTE->TASKS_OUT[1]);	//PPIͨ��3�����յ㣨GPIOTE�������OUT[1]���񣩣�
	NRF_PPI->CHENSET = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos) | (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos) | (PPI_CHEN_CH2_Enabled << PPI_CHEN_CH2_Pos) | (PPI_CHEN_CH3_Enabled << PPI_CHEN_CH3_Pos);	//ͨ��ʹ�����üĴ���
}

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
  * @brief		PPI֮PWM���Ӧ�ã�����⣩��ʼ������
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void ppi_pwm_output_application_component_init(void)
{
	ret_code_t error_code;	//�������
	app_pwm_config_t pwm1_configuration_parameter = APP_PWM_DEFAULT_CONFIG_2CH(5000L, BSP_LED_0, BSP_LED_1);	//2��Ƶ����PWM������5ms��ͨ��LED1��LED2���
	
	pwm1_configuration_parameter.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;	//�����ϼ���״̬�ļ���
	error_code = app_pwm_init(&PWM1, &pwm1_configuration_parameter, app_pwm_callback);	//��ʼ��PWMʵ�����
	APP_ERROR_CHECK(error_code);	//���������
	app_pwm_enable(&PWM1);	//ʹ��PWMʵ��
}

/*******************************************************************************
  * @brief		����PPI֮PWM���Ӧ�ã�����⣩ռ�ձȺ���
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
		duty_cycle_change = false;	//ռ�ձ�û�ı�
		while(app_pwm_channel_duty_set(&PWM1, 0, value) == NRF_ERROR_BUSY);	//����ռ�ձȣ�ֱ��PWM׼����
		while(!duty_cycle_change);	//�ȴ�PWMռ�ձȸı����ʱִ�е�PWM�ص�����ִ�����
		APP_ERROR_CHECK(app_pwm_channel_duty_set(&PWM1, 1, value));	//����ռ�ձ�
	}
}
