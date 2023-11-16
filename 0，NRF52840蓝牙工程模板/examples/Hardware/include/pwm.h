#ifndef __PWM_H
#define __PWM_H
#include "nrf.h"

void pwm_count_up_init_register(void);	//PWM���ϼ�����ʼ�����Ĵ�����
void pwm_count_down_init_register(void);	//PWM���¼�����ʼ�����Ĵ�����
void pwm_common_mode_simple_playback_component(void);	//PWM��ͬģʽ�򵥻طţ�����⣩
void pwm_common_mode_complex_playback_component(void);	//PWM��ͬģʽ���ӻطţ�����⣩
void pwm_common_mode_simple_no_playback_component(void);	//PWM��ͬģʽ�򵥲��طţ�����⣩
void pwm_standalone_mode_simple_no_callback_component(void);	//PWM����ģʽ�򵥲��ص�������⣩
void pwm_standalone_mode_simple_callback_component(void);	//PWM����ģʽ�򵥻ص�������⣩
#endif
