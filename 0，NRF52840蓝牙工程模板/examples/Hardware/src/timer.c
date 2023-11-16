#include "timer.h"
#include "uart.h"

/*******************************************************************************
  * @brief		定时器定时初始化函数
  * @param		timer - 要初始化的定时器号
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_timing_init(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//定时器结构体指针
	
	//启动16MHz晶振
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)	//等待外部振荡器启动
	{
		//不做任何处理，等待采集完成
	}
	switch(timer)	//判断初始化的定时器号
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//定时器0模块
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//定时器1模块
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//定时器2模块
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//定时器3模块
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//定时器4模块
		default:		nrf_timer = 0;			break;	//没有定时器模块
	}
	if(nrf_timer == 0)
	{
		//不做任何处理
	}
	nrf_timer->MODE = TIMER_MODE_MODE_Timer;	//定时器模式
	nrf_timer->PRESCALER = 10;	//定时器预分频值：10-1   产生31250Hz定时器频率=> 1 tick = 32 us
	nrf_timer->BITMODE = TIMER_BITMODE_BITMODE_16Bit;	//16位宽定时器
}

/*******************************************************************************
  * @brief		定时器延时函数
  * @param		timer - 要初始化的定时器号
				delay_ms - 延时的ms时间
  * @retval		None
  * @attention	None
*******************************************************************************/
void nrf_timer_delay_ms(Timer_Enum timer,uint32_t delay_ms)
{
	NRF_TIMER_Type *nrf_timer;	//定时器结构体指针
	
	switch(timer)	//判断初始化的定时器号
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//定时器0模块
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//定时器1模块
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//定时器2模块
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//定时器3模块
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//定时器4模块
		default:		nrf_timer = 0;			break;	//没有定时器模块
	}
	nrf_timer->TASKS_CLEAR = 1;	//清定时器
	nrf_timer->CC[0] = delay_ms * 31;	//32个单位，乘以31.25得到毫秒
	nrf_timer->CC[0] += delay_ms / 4;	//设置比较寄存器值
	nrf_timer->TASKS_START = 1;	//开启定时器
	while(nrf_timer->EVENTS_COMPARE[0] == 0)	//等待比较CC[0]匹配事件，触发后比较CC[0]匹配事件置1
	{
		//不做任何处理
	}
	nrf_timer->EVENTS_COMPARE[0] = 0;	//比较CC[0]匹配事件清0
	nrf_timer->TASKS_STOP = 1;	//停止定时器
}

/*******************************************************************************
  * @brief		定时器计数初始化函数
  * @param		timer - 要初始化的定时器号
  * @retval		None
  * @attention	None
*******************************************************************************/
void timer_count_init(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//定时器结构体指针
	
	switch(timer)	//判断初始化的定时器号
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//定时器0模块
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//定时器1模块
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//定时器2模块
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//定时器3模块
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//定时器4模块
		default:		nrf_timer = 0;			break;	//没有定时器模块
	}
	if(nrf_timer == 0)
	{
		//不做任何处理
	}
	nrf_timer->MODE = TIMER_MODE_MODE_Counter;	//计数器模式
	nrf_timer->BITMODE = TIMER_BITMODE_BITMODE_24Bit;	//24位宽定时器
	nrf_timer->TASKS_START = 1;	//开启定时器
}

/*******************************************************************************
  * @brief		获取定时器计数的值函数
  * @param		timer - 要初始化的定时器号
  * @retval		定时器计数的值
  * @attention	内部计数器Counter寄存器的值存储到CC[0]
*******************************************************************************/
uint32_t get_timer_count_value(Timer_Enum timer)
{
	NRF_TIMER_Type *nrf_timer;	//定时器结构体指针
	
	switch(timer)	//判断初始化的定时器号
	{
		case timer0:	nrf_timer = NRF_TIMER0;	break;	//定时器0模块
		case timer1:	nrf_timer = NRF_TIMER1;	break;	//定时器1模块
		case timer2:	nrf_timer = NRF_TIMER2;	break;	//定时器2模块
		case timer3:	nrf_timer = NRF_TIMER3;	break;	//定时器3模块
		case timer4:	nrf_timer = NRF_TIMER4;	break;	//定时器4模块
		default:		nrf_timer = 0;			break;	//没有定时器模块
	}
	if(nrf_timer == 0)
	{
		//不做任何处理
	}
	nrf_timer->TASKS_CAPTURE[0] = 1;	//将Counter值捕获到CC[0]寄存器
	nrf_timer->TASKS_COUNT = 1;	//计数器递增（仅限计数器模式）
	return nrf_timer->CC[0];
}
