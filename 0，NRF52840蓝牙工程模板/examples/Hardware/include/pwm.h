#ifndef __PWM_H
#define __PWM_H
#include "nrf.h"

void pwm_count_up_init_register(void);	//PWM向上计数初始化（寄存器）
void pwm_count_down_init_register(void);	//PWM向下计数初始化（寄存器）
void pwm_common_mode_simple_playback_component(void);	//PWM共同模式简单回放（组件库）
void pwm_common_mode_complex_playback_component(void);	//PWM共同模式复杂回放（组件库）
void pwm_common_mode_simple_no_playback_component(void);	//PWM共同模式简单不回放（组件库）
void pwm_standalone_mode_simple_no_callback_component(void);	//PWM独立模式简单不回调（组件库）
void pwm_standalone_mode_simple_callback_component(void);	//PWM独立模式简单回调（组件库）
#endif
