/**
****************************(C) COPYRIGHT 2023 ͼ����****************************
* @file       led.c
* @brief      ����led����غ���
* @note       
* @history
*  Version    Date            Author
*  V1.0.0     05-15-2023      ͼ����    
*
****************************(C) COPYRIGHT 2023 ͼ����****************************
*/
 
#include "led.h"
#include "tim.h"

/**
  * @brief          ����led1������
  * @param[in]      status: Ϊ1ʱled�Ƶ���,Ϊ0ʱled��Ϩ��
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
  * @brief          ����led_rgb�Ƶ���ɫ����
  * @param[in]      r, g, b: �ֱ�Ϊ������ɫ��ֵ
  * @retval         none
  */
void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, r);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, b);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, g);
}
