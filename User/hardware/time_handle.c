#include "time_handle.h"
#include "main.h"
#include "tim.h"
#include <stdio.h>

/*
*************************************
�궨��
*************************************
*/
/*
*************************************
�궨��
*************************************
*/
/*
*************************************
��������
*************************************
*/
/*
*************************************
��������
*************************************
*/
static uint16_t index_10ms = 0;
uint16_t index_oled = 0;
uint16_t index_dht11 = 0;
uint8_t index_send_msg = 0;
uint8_t index_led = 0;
uint8_t index_bh1750 = 0;
uint8_t index_step = 0;

/**
  * @brief          ��ʱ��2�жϷ�����,10msһ���ж�
  * @param[in]      htim:��ʱ��
  * @retval         none
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	
//{
//	if (htim->Instance == htim2.Instance)
//	{
//		index_10ms++;	 
//    if(index_10ms%100==0)
//    {
//      index_oled=1;
//      index_dht11=1;
//      index_led=1;
//			index_bh1750=1;
//    }
//    if(index_10ms >= 500)	
//		{
//			index_send_msg=1;
//			index_10ms = 0;
//		}
//		index_step=1;
//	}
//	if (htim->Instance == TIM4) {
//    HAL_IncTick();
//  }

//}
/**
  * @brief          ������ѯʱ��Ƭ,������ʱ��2
  * @param[in]      none
  * @retval         none
  */
void time_slot_start(void)
{
  HAL_TIM_Base_Start_IT(&htim2);	//�򿪶�ʱ��2�ж�
}

	


