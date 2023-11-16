#include "led.h"

/**@brief LED初始化函数
 *
 * @param[in]  None
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void led_init(void)
{
    nrf_gpio_cfg_output(LED1); /* 配置LED1 IO端口为输出状态 */
    nrf_gpio_cfg_output(LED2); /* 配置LED2 IO端口为输出状态 */
    nrf_gpio_cfg_output(LED3); /* 配置LED3 IO端口为输出状态 */
    nrf_gpio_cfg_output(LED4); /* 配置LED4 IO端口为输出状态 */

    led_close(LED1); /* 关闭LED1 */
    led_close(LED2); /* 关闭LED2 */
    led_close(LED3); /* 关闭LED3 */
    led_close(LED4); /* 关闭LED4 */
}

/**@brief LED 打开函数
 *
 * @param[in]  pin_number - LED端口和引脚号
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void led_open(uint32_t pin_number)
{
    nrf_gpio_pin_clear(pin_number); /* IO端口和引脚号输出低电平 */
}

/**@brief LED 关闭函数
 *
 * @param[in]  pin_number - LED端口和引脚号
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void led_close(uint32_t pin_number)
{
    nrf_gpio_pin_set(pin_number); /* IO端口和引脚号输出高电平 */
}

/**@brief LED 翻转函数
 *
 * @param[in]  pin_number - LED端口和引脚号
 *
 * @retval     None
 *
 * @note None
 * @attention None
 */
void led_toggle(uint32_t pin_number)
{
    nrf_gpio_pin_toggle(pin_number); /* IO端口和引脚号电平翻转（先输出高电平，再输出低电平） */
}
