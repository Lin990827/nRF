#ifndef __SAADC_H
#define __SAADC_H

void saadc_single_sample_component_init(void);	//SAADC���β�����ʼ��������⣩
void saadc_differential_sample_component_init(void);	//SAADC��ֲ�����ʼ��������⣩
void saadc_easydma_single_buffer_interrupt_single_sample_component_init(void);	//SAADC EasyDMA�������жϵ�ͨ��������ʼ��������⣩
void saadc_easydma_single_buffer_interrupt_more_sample_component_init(void);	//SAADC EasyDMA�������ж϶�ͨ��������ʼ��������⣩
void saadc_easydma_double_buffer_interrupt_more_sample_component_init(void);	//SAADC EasyDMA˫�����ж϶�ͨ��������ʼ��������⣩
void saadc_easydma_ppi_start_double_buffer_interrupt_sample_component_init(void);	//SAADC EasyDMA֮PPI����˫�����жϲ�����ʼ��������⣩
#endif
