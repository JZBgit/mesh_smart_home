#ifndef __OLED_H
#define __OLED_H

#include "spi.h" 


#define OLED_RES(x)      do{ x ? \
                                  HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* 设置RES引脚 */
#define OLED_DC(x)      do{ x ? \
                                  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* 设置DC引脚 */
#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* 设置CS引脚 */

#define Get_Array_Length(arr) (sizeof(arr) / sizeof(arr[0]))

/* 命令/数据 定义 */
#define OLED_CMD        0       /* 写命令 */
#define OLED_DATA       1       /* 写数据 */
#define OLED_Timeout    500

void OLED_Init(void);           /* OLED初始化 */
void OLED_Clear(void);          /* OLED清屏 */
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t sizey);
void OLED_ShowNum(uint8_t x, uint8_t y, uint16_t num, uint8_t size);

#endif




