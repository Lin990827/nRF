#ifndef __LED_H
#define __LED_H
#include "nrf.h"
#include "nrf_gpio.h"

#define LED1 NRF_GPIO_PIN_MAP(0, 13) /* 宏定义LED1端口和引脚号 */
#define LED2 NRF_GPIO_PIN_MAP(0, 14) /* 宏定义LED2端口和引脚号 */
#define LED3 NRF_GPIO_PIN_MAP(0, 15) /* 宏定义LED3端口和引脚号 */
#define LED4 NRF_GPIO_PIN_MAP(0, 16) /* 宏定义LED4端口和引脚号 */

void led_init(void); /* LED初始化 */
void led_open(uint32_t pin_number); /* LED亮 */
void led_close(uint32_t pin_number); /* LED灭 */
void led_toggle(uint32_t pin_number); /* LED电平翻转 */
#endif
