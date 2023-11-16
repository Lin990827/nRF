#ifndef __SAADC_H
#define __SAADC_H

void saadc_single_sample_component_init(void);	//SAADC单次采样初始化（组件库）
void saadc_differential_sample_component_init(void);	//SAADC差分采样初始化（组件库）
void saadc_easydma_single_buffer_interrupt_single_sample_component_init(void);	//SAADC EasyDMA单缓冲中断单通道采样初始化（组件库）
void saadc_easydma_single_buffer_interrupt_more_sample_component_init(void);	//SAADC EasyDMA单缓冲中断多通道采样初始化（组件库）
void saadc_easydma_double_buffer_interrupt_more_sample_component_init(void);	//SAADC EasyDMA双缓冲中断多通道采样初始化（组件库）
void saadc_easydma_ppi_start_double_buffer_interrupt_sample_component_init(void);	//SAADC EasyDMA之PPI启动双缓冲中断采样初始化（组件库）
#endif
