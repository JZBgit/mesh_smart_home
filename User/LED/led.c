/**
****************************(C) COPYRIGHT 2023 图解编程****************************
* @file       led.c
* @brief      控制led灯相关函数
* @note       
* @history
*  Version    Date            Author
*  V1.0.0     05-15-2023      图解编程    
*
****************************(C) COPYRIGHT 2023 图解编程****************************
*/
 
#include "led.h"
#include "tim.h"

/**
  * @brief          控制led1亮灭函数
  * @param[in]      status: 为1时led灯点亮,为0时led灯熄灭
  * @retval         none
  */
void set_led(uint8_t status)
{
  if(status==1)
  {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }
}

/**
  * @brief          控制led_rgb灯的颜色函数
  * @param[in]      r, g, b: 分别为三个颜色的值
  * @retval         none
  */
void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, r);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, b);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, g);
}
