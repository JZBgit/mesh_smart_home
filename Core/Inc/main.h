/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define D0_Pin GPIO_PIN_5
#define D0_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_7
#define D1_GPIO_Port GPIOA
#define led_g_Pin GPIO_PIN_0
#define led_g_GPIO_Port GPIOB
#define RES_Pin GPIO_PIN_1
#define RES_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_10
#define DC_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_11
#define CS_GPIO_Port GPIOB
#define A_Pin GPIO_PIN_12
#define A_GPIO_Port GPIOB
#define B_Pin GPIO_PIN_13
#define B_GPIO_Port GPIOB
#define C_Pin GPIO_PIN_14
#define C_GPIO_Port GPIOB
#define D_Pin GPIO_PIN_15
#define D_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_8
#define DHT11_GPIO_Port GPIOA
#define led_r_Pin GPIO_PIN_4
#define led_r_GPIO_Port GPIOB
#define led_b_Pin GPIO_PIN_5
#define led_b_GPIO_Port GPIOB
#define myII_SCL_Pin GPIO_PIN_8
#define myII_SCL_GPIO_Port GPIOB
#define myIIC_SDA_Pin GPIO_PIN_9
#define myIIC_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
