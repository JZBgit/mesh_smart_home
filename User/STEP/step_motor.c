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
 
#include "step_motor.h"

/**
  * @brief          控制led1亮灭函数
  * @param[in]      status: 为1时led灯点亮,为0时led灯熄灭
  * @retval         none
  */
void rotate(uint8_t status)
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
