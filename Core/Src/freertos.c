/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "dht11.h"
#include "esp8266.h"
#include "led.h"
#include "bh1750.h"
#include "step_motor.h"
#include "uart_printf.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId esp8266TaskHandle;
osThreadId dht11TaskHandle;
osThreadId BH1750TaskHandle;
osThreadId LEDTaskHandle;
osThreadId OLEDTaskHandle;
osThreadId STEPTaskHandle;
osThreadId sysStartHandle;
osMessageQId bh_dataHandle;
osMessageQId dht_dataHandle;
osMessageQId step_dataHandle;
osMessageQId led_rgbHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void esp_task(void const * argument);
void dht_task(void const * argument);
void bh_task(void const * argument);
void led_task(void const * argument);
void oled_task(void const * argument);
void step_task(void const * argument);
void sys_start(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* definition and creation of sysStart */
	/* 创建一个启动任务 */
  osThreadDef(sysStart, sys_start, osPriorityNormal, 0, 128);
  sysStartHandle = osThreadCreate(osThread(sysStart), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_esp_task */
/**
  * @brief  Function implementing the esp8266Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_esp_task */
void esp_task(void const * argument)
{
  /* USER CODE BEGIN esp_task */
	uint8_t temp_value =0;
	uint8_t humi_value =0;
	uint16_t light = 0;
	uint8_t num_value =0;
	uint8_t direct_value =0;
	uint8_t led_status =0;
	uint8_t Red_value =0;
	uint8_t Blue_value =0;
	uint8_t Green_value =0;
  /* Infinite loop */
  for(;;)
  {
		esp8266_receive_msg(&num_value,&direct_value,&led_status,&Red_value,&Blue_value,&Green_value);
		xQueueSend(step_dataHandle,&num_value,0);
		xQueueSend(step_dataHandle,&direct_value,0);
		xQueueSend(led_rgbHandle,&led_status,0);
		xQueueSend(led_rgbHandle,&Red_value,0);
		xQueueSend(led_rgbHandle,&Blue_value,0);
		xQueueSend(led_rgbHandle,&Green_value,0);
		xQueuePeek(bh_dataHandle,&light,portMAX_DELAY);
		xQueuePeek(dht_dataHandle,&temp_value,portMAX_DELAY);
		xQueuePeek(dht_dataHandle,&humi_value,portMAX_DELAY);
		esp8266_send_msg(temp_value,humi_value,light);
    osDelay(1000);
  }
  /* USER CODE END esp_task */
}

/* USER CODE BEGIN Header_dht_task */
/**
* @brief Function implementing the dht11Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_dht_task */
void dht_task(void const * argument)
{
  /* USER CODE BEGIN dht_task */
	uint8_t temp_value =0;
	uint8_t humi_value =0;
  /* Infinite loop */
  for(;;)
  {
		dht11_read_data(&temp_value,&humi_value);
		xQueueOverwrite(dht_dataHandle,&temp_value);
		xQueueOverwrite(dht_dataHandle,&humi_value);
		osDelay(2000);
  }
  /* USER CODE END dht_task */
}

/* USER CODE BEGIN Header_bh_task */
/**
* @brief Function implementing the BH1750Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_bh_task */
void bh_task(void const * argument)
{
  /* USER CODE BEGIN bh_task */
	uint8_t light_temp[2];
	uint16_t light = 0;
  /* Infinite loop */
  for(;;)
  {
		BH1750_Read(light_temp);
		light=(light_temp[0]<<8)|light_temp[1];
		xQueueOverwrite(bh_dataHandle,&light);
    osDelay(1000);
  }
  /* USER CODE END bh_task */
}

/* USER CODE BEGIN Header_led_task */
/**
* @brief Function implementing the LEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_task */
void led_task(void const * argument)
{
  /* USER CODE BEGIN led_task */
	uint8_t led_status =0;
	uint8_t Red_value =0;
	uint8_t Blue_value =0;
	uint8_t Green_value =0;
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(led_rgbHandle,&led_status,portMAX_DELAY);
		xQueueReceive(led_rgbHandle,&Red_value,portMAX_DELAY);
		xQueueReceive(led_rgbHandle,&Blue_value,portMAX_DELAY);
		xQueueReceive(led_rgbHandle,&Green_value,portMAX_DELAY);
    set_led(led_status);
		set_rgb(255-Red_value, 255-Green_value, 255-Blue_value);
    osDelay(1000);
  }
  /* USER CODE END led_task */
}

/* USER CODE BEGIN Header_oled_task */
/**
* @brief Function implementing the OLEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_oled_task */
void oled_task(void const * argument)
{
  /* USER CODE BEGIN oled_task */
	uint8_t temp_value =0;
	uint8_t humi_value =0;
	uint16_t light = 0;
  /* Infinite loop */
  for(;;)
  {
		xQueuePeek(bh_dataHandle,&light,portMAX_DELAY);
		xQueuePeek(dht_dataHandle,&temp_value,portMAX_DELAY);
		xQueuePeek(dht_dataHandle,&humi_value,portMAX_DELAY);
		OLED_ShowNum(16,2,temp_value,16);
		OLED_ShowNum(80,2,humi_value,16);
		OLED_ShowNum(16,4,light,16);
    osDelay(1000);
  }
  /* USER CODE END oled_task */
}

/* USER CODE BEGIN Header_step_task */
/**
* @brief Function implementing the STEPTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_step_task */
void step_task(void const * argument)
{
  /* USER CODE BEGIN step_task */
	uint8_t num_value =0;
	uint8_t direct_value =0;
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(step_dataHandle,&num_value,portMAX_DELAY);
		xQueueReceive(step_dataHandle,&direct_value,portMAX_DELAY);
		rotate(direct_value,num_value);
    osDelay(10);
  }
  /* USER CODE END step_task */
}

/* USER CODE BEGIN Header_sys_start */
/**
* @brief Function implementing the sysStart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sys_start */
void sys_start(void const * argument)
{
  /* USER CODE BEGIN sys_start */
	
	/* 进入临界区:保护临界区里的代码不会被打断 */
	taskENTER_CRITICAL();
	
  /* Create the queue(s) */
  /* definition and creation of bh_data */
  osMessageQDef(bh_data, 1, uint16_t);
  bh_dataHandle = osMessageCreate(osMessageQ(bh_data), NULL);

  /* definition and creation of dht_data */
  osMessageQDef(dht_data, 2, uint8_t);
  dht_dataHandle = osMessageCreate(osMessageQ(dht_data), NULL);

  /* definition and creation of step_data */
  osMessageQDef(step_data, 2, uint8_t);
  step_dataHandle = osMessageCreate(osMessageQ(step_data), NULL);

  /* definition and creation of led_rgb */
  osMessageQDef(led_rgb, 4, uint8_t);
  led_rgbHandle = osMessageCreate(osMessageQ(led_rgb), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of esp8266Task */
  osThreadDef(esp8266Task, esp_task, osPriorityNormal, 0, 128);
  esp8266TaskHandle = osThreadCreate(osThread(esp8266Task), NULL);

  /* definition and creation of dht11Task */
  osThreadDef(dht11Task, dht_task, osPriorityNormal, 0, 128);
  dht11TaskHandle = osThreadCreate(osThread(dht11Task), NULL);

  /* definition and creation of BH1750Task */
  osThreadDef(BH1750Task, bh_task, osPriorityNormal, 0, 128);
  BH1750TaskHandle = osThreadCreate(osThread(BH1750Task), NULL);

  /* definition and creation of LEDTask */
  osThreadDef(LEDTask, led_task, osPriorityNormal, 0, 128);
  LEDTaskHandle = osThreadCreate(osThread(LEDTask), NULL);

  /* definition and creation of OLEDTask */
  osThreadDef(OLEDTask, oled_task, osPriorityNormal, 0, 128);
  OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

  /* definition and creation of STEPTask */
  osThreadDef(STEPTask, step_task, osPriorityNormal, 0, 128);
  STEPTaskHandle = osThreadCreate(osThread(STEPTask), NULL);
	
	/* 启动任务只需要执行一次即可，用完就删除自己 */
	vTaskDelete(NULL);

	/* 退出临界区 */
	taskEXIT_CRITICAL();
	
  /* USER CODE END sys_start */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

