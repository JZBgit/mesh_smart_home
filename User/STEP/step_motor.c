/**
********************************************************
* @file       step_motor.c
* @brief      控制步进电机相关函数
* @note       
* @history
*  Version    Date            Author
*  V1.0.0     05-15-2021      蒋志邦    
*
********************************************************
*/
 
#include "step_motor.h"

/**
  * @brief          控制步进电机转动方向和圈数
  * @param[in]      status: 为0时顺时针转动，为1时逆时针转动
	                  num: 转动圈数
  * @retval         none
  */
void rotate(uint8_t status, uint8_t num)
{
	static int step_sel=0;
	if(status){
		step_sel++;
	}else{
		step_sel--;
	}
	if(step_sel==8){
		step_sel=0;
	}
	if(step_sel==-1){
		step_sel=7;
	}
	switch(step_sel){
		case 0:
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, B_Pin|C_Pin|D_Pin, GPIO_PIN_RESET);break;
		case 1:
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|B_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, C_Pin|D_Pin, GPIO_PIN_RESET);break;
		case 2:
			HAL_GPIO_WritePin(A_GPIO_Port, B_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|C_Pin|D_Pin, GPIO_PIN_RESET);break;
		case 3:
			HAL_GPIO_WritePin(A_GPIO_Port, B_Pin|C_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|D_Pin, GPIO_PIN_RESET);break;
		case 4:
			HAL_GPIO_WritePin(A_GPIO_Port, C_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|B_Pin|D_Pin, GPIO_PIN_RESET);break;
		case 5:
			HAL_GPIO_WritePin(A_GPIO_Port, C_Pin|D_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|B_Pin, GPIO_PIN_RESET);break;
		case 6:
			HAL_GPIO_WritePin(A_GPIO_Port, D_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|B_Pin|C_Pin, GPIO_PIN_RESET);break;
		case 7:
			HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|D_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(A_GPIO_Port, B_Pin|C_Pin, GPIO_PIN_RESET);break;
	}
}
