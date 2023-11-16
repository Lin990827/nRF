#ifndef __PPI_PWM_OUTPUT_APPLICATION_H
#define __PPI_PWM_OUTPUT_APPLICATION_H

void ppi_pwm_output_application_register_init(void);	//PPI之PWM输出应用（寄存器）初始化
void ppi_pwm_output_application_component_init(void);	//PPI之PWM输出应用（组件库）初始化
void set_ppi_pwm_output_application_component_duty(void);	//设置PPI之PWM输出应用（组件库）占空比
#endif
