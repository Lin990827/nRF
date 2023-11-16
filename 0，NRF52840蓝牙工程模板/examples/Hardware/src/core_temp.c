#include "core_temp.h"

/*******************************************************************************
  * @brief		内部温度初始化函数
  * @param		None
  * @retval		None
  * @attention	None
*******************************************************************************/
void core_temp_init(void)
{
	nrf_temp_init();	//初始化临时模块（用于温度测量）并将其写入隐藏的配置寄存器
}

/*******************************************************************************
  * @brief		读取内部温度函数
  * @param		None
  * @retval		读取的温度值
  * @attention	None
*******************************************************************************/
float read_core_temp(void)
{
	float temp;
	
	NRF_TEMP->TASKS_START = 1;	//开始测量温度
	while(NRF_TEMP->EVENTS_DATARDY == 0)	//温度数据没采集完成，如果为DATARDY事件启用中断并在中断中读取结果，则可以跳过等待
	{
		//不做任何处理，等待采集完成
	}
	NRF_TEMP->EVENTS_DATARDY = 0;	//标记温度数据没采集完成，方便下次采集
	temp = nrf_temp_read() / 4.0f;	//温度测量读数,并转化为十进制数
	NRF_TEMP->TASKS_STOP = 1;	//停止测量温度
	return temp;
}
