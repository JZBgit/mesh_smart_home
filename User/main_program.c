#include "main_program.h"
#include <stdio.h>
#include "time_handle.h"
#include "oled.h"
#include "dht11.h"
#include "esp8266.h"
#include "led.h"
#include "bh1750.h"
#include "step_motor.h"
#include "tim.h"

/*
*************************************
宏定义
*************************************
*/

/*
*************************************
变量声明
*************************************
*/
extern uint16_t index_oled;
extern uint16_t index_dht11;
extern uint8_t index_send_msg;
extern uint8_t index_led;
extern uint8_t index_bh1750;
extern uint8_t index_step;
/*
*************************************
函数声明
*************************************
*/

/*
*************************************
变量定义
*************************************
*/
uint8_t led_status =0;
uint8_t temp_value =0;
uint8_t humi_value =0;
uint8_t light_temp[2];
uint16_t light = 0;
uint8_t num_value =0;
uint8_t direct_value =0;
uint8_t Red_value =0;
uint8_t Blue_value =0;
uint8_t Green_value =0;
//uint16_t light_value = 0;
/*
*************************************
函数定义
*************************************
*/

/**
  * @brief          初始化函数,相当于Arduino的setup()函数,只在初始的时候执行一次
  * @param[in]      none
  * @retval         none
  */
void user_init_program(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	set_rgb(255, 255, 255);
  OLED_Init(); 
	BH1750_Init();
  esp8266_init();
  while(dht11_init())
  {
    HAL_Delay(500);
    printf("dht11 init faild\r\n");
//    OLED_printf (0,0,"dht11 init faild");
		OLED_ShowString(0,0,"dht11 init faild",16);
  }
//  OLED_printf (0,0,"  Alibaba Cloud IOT ");
	OLED_ShowString(0,0,"  Alibaba Cloud IOT ",16);
  time_slot_start();
}
/**
  * @brief          初始化函数,相当于Arduino的loop()函数,一直执行该函数
  * @param[in]      none
  * @retval         none
  */
void user_main_program(void)
{
  if(esp8266_receive_msg()!=1)
  {
  }
  if(index_dht11 ==1)
  {
    dht11_read_data(&temp_value,&humi_value);
    index_dht11=0;
  }
  if(index_led==1)
  {
    set_led(led_status);
		set_rgb(255-Red_value, 255-Green_value, 255-Blue_value);
    index_led=0;
  }
  if(index_bh1750==1)
  {
		BH1750_Read(light_temp);
		light=(light_temp[0]<<8)|light_temp[1];
    index_bh1750=0;
  }
  if(index_oled==1)
  {
//    OLED_printf (2,0,"temp:%d",temp_value);
//    OLED_printf (4,0,"humi:%d",humi_value);
		OLED_ShowString(0,2,"T:",16);
		OLED_ShowString(64,2,"H:",16);
		OLED_ShowString(0,4,"L:",16);
		OLED_ShowNum(16,2,temp_value,16);
		OLED_ShowNum(80,2,humi_value,16);
		OLED_ShowNum(16,4,light,16);
    index_oled=0;
  }
  if(index_send_msg==1)
	{
		index_send_msg =0;
		if(esp8266_send_msg()==0)
		{
		}
		else
		{
			printf("msg send error\r\n");
		}
	}
  if(index_step==1)
  {
    rotate(direct_value);
    index_step=0;
  }
}

