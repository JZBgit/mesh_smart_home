#include "esp8266.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oled.h"
#include "core_json.h"

/*
*************************************
宏定义
*************************************
*/
#define WIFI_SSID        "k40jzb"
#define WIFI_PASSWD      "jzb123456"

#define MQTT_CLIENT_ID   "mqtt_stm32|securemode=2\\,signmethod=hmacsha1\\,timestamp=1727008745093|"   
#define MQTT_USER_NAME   "mqtt_stm32&k1s5gwx0tkq"
#define MQTT_PASSWD      "50E49463A250F0DED81C5BA74E9EE067889D4AEA"
#define BROKER_ASDDRESS  "k1s5gwx0tkq.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define SUB_TOPIC        "/sys/k1s5gwx0tkq/mqtt_stm32/thing/service/property/set"
#define PUB_TOPIC        "/sys/k1s5gwx0tkq/mqtt_stm32/thing/event/property/post"
#define JSON_FORMAT      "{\\\"params\\\":{\\\"Red\\\":%d\\,\\\"temp\\\":%d\\,\\\"Blue\\\":%d\\,\\\"light\\\":%d\\,\\\"num\\\":%d\\,\\\"humi\\\":%d\\,\\\"direct\\\":%d\\,\\\"led\\\":%d\\,\\\"Green\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"
//"{\\\"params\\\":{\\\"temp\\\":%d\\,\\\"step_motor\\\":{\\\"num\\\":%d\\,\\\"direct\\\":%d}\\,\\\"humi\\\":%d\\,\\\"RGBColor\\\":{\\\"Red\\\":%d\\,\\\"Blue\\\":%d\\,\\\"Green\\\":%d}\\,\\\"led\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"
//"{\\\"params\\\":{\\\"temp\\\":%d\\,\\\"humi\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"

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
/*
*************************************
变量定义
*************************************
*/
unsigned char receive_buf[512];	  //串口2接收缓存数组
unsigned char receive_start = 0;	//串口2接收开始标志位
uint16_t receive_count = 0;	      //串口2接收数据计数器
uint16_t receive_finish = 0;	    //串口2接收结束标志位 

extern uint8_t led_status;
extern uint8_t temp_value;
extern uint8_t humi_value;
extern uint8_t num_value;
extern uint8_t direct_value;
extern uint8_t Red_value;
extern uint8_t Blue_value;
extern uint8_t Green_value;
extern uint16_t light;
/**
  * @brief          解析json数据
  * @param[in]      json_msg:json数据,json_len:json数据长度
  * @retval         返回0找到指定json数据，否则返回1
  */
uint8_t parse_json_msg(uint8_t *json_msg,uint8_t json_len)
{
  uint8_t retval =0;
	uint8_t i=0;
  
  JSONStatus_t result;
//	char *query[] = {"params.led", "params.step_motor", "params.RGBColor"};
  char *query[]= {"params.led", "params.Red", "params.Blue", "params.Green", "params.direct", "params.num"};
//	step_motor
//	RGBColor
  size_t queryLength = 0;
//	size_t queryLength;
  char * value;
  size_t valueLength;
  result = JSON_Validate((const char *)json_msg, json_len);
  if( result == JSONSuccess)
  {
		for(i=0;i<6;++i){
			queryLength = strlen( query[i] );
			result = JSON_Search((char *)json_msg, json_len, query[i], queryLength,&value, &valueLength );
			if( result == JSONSuccess)
			{
				char save = value[valueLength];
				value[valueLength] = '\0';
				printf("Found: %s %d-> %s\n", query[i], valueLength,value);
				value[valueLength] = save;
				if(i==0){
					led_status = atoi(value);
				}else if(i==1){
					Red_value = atoi(value);
				}else if(i==2){
					Blue_value = atoi(value);
				}else if(i==3){
					Green_value = atoi(value);
				}else if(i==4){
					direct_value = atoi(value);
				}else if(i==5){
					num_value = atoi(value);
				}
				retval = 0;
				break;
			}
			else
			{
				retval = 1;
			}
		}
  }
  else
  {
    retval = 1;
  }  
  return retval;
}
/**
  * @brief          串口2数据接收处理函数
  * @param[in]      none
  * @retval         none
  */
void uart2_receiver_handle(void)
{
  unsigned char receive_data = 0;   
  if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE) != RESET)
  {
    HAL_UART_Receive(&huart2, &receive_data, 1, 1000);//串口2接收1位数据
    receive_buf[receive_count++] = receive_data;
    receive_start = 1;	                              //串口2接收数据开始标志位置1
    receive_finish = 0;	                              //串口2接收数据完成标志位清0
  }
}
/**
  * @brief          串口2数据接收清0函数
  * @param[in]      len:清空的数据长度
  * @retval         none
  */
void uart2_receiver_clear(uint16_t len)	
{
	memset(receive_buf,0x00,len);
	receive_count = 0;
	receive_start = 0;
	receive_finish = 0;
}
/**
  * @brief          esp8266发送命令函数
  * @param[in]      cmd:发送的命令,len:命令的长度,rec_data:期望接收数据
  * @retval         none
  */
uint8_t esp8266_send_cmd(unsigned char *cmd,unsigned char len,char *rec_data)	
{
  unsigned char retval =0;
  unsigned int count = 0;

  HAL_UART_Transmit(&huart2, cmd, len, 1000);	                                   
  while((receive_start == 0)&&(count<1000))
  {
    count++;
    HAL_Delay(1);
  }

  if(count >= 1000)	
  {
    retval = 1;	
  }
  else	
  {
    do
    {
      receive_finish++;
      HAL_Delay(1);
    }
    while(receive_finish < 500);
    retval = 2;
    if(strstr((const char*)receive_buf, rec_data))	
    {
      retval = 0;	
    }
  }
  uart2_receiver_clear(receive_count);
  return retval;
}
/**
  * @brief          esp8266配置wifi网络
  * @param[in]      none
  * @retval         网络配置成功返回0,否则返回1
  */
uint8_t esp8266_config_network(void)
{
	uint8_t retval =0;
	uint16_t count = 0;
	
	HAL_UART_Transmit(&huart2, (unsigned char *)"AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWD"\"\r\n",strlen("AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWD"\"\r\n"), 1000);
	
	while((receive_start == 0)&&(count<1000))
	{
		count++;
		HAL_Delay(1);
	}
	
	if(count >= 1000)	
	{
		retval = 1;	
	}
	else
	{
		HAL_Delay(8000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;
    }
	}
  uart2_receiver_clear(receive_count);
	return retval;
}
/**
  * @brief          esp8266连接服务
  * @param[in]      none
  * @retval         连接成功返回0,否则返回1
  */
uint8_t esp8266_connect_server(void)
{
	uint8_t retval=0;
	uint16_t count = 0;

	HAL_UART_Transmit(&huart2, (unsigned char *)"AT+MQTTCONN=0,\""BROKER_ASDDRESS"\",1883,0\r\n",strlen("AT+MQTTCONN=0,\""BROKER_ASDDRESS"\",1883,0\r\n"), 1000);	
	while((receive_start == 0)&&(count<1000))	
	{
		count++;
		HAL_Delay(1);
	}
	
	if(count >= 1000)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(5000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;	
    }
	}
  uart2_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief          esp8266复位
  * @param[in]      none
  * @retval         返回0复位成功,返回1复位失败
  */
uint8_t esp8266_reset(void)
{
	uint8_t retval =0;
	uint16_t count = 0;
	
	HAL_UART_Transmit(&huart2, (unsigned char *)"AT+RST\r\n",8, 1000);
	while((receive_start == 0)&&(count<2000))	
	{
		count++;
		HAL_Delay(1);
	}
	if(count >= 2000)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(5000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;	
    }
	}
  uart2_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief          esp8266发送数据
  * @param[in]      none
  * @retval         返回0发送数据成功,返回1发送数据失败
  */
uint8_t esp8266_send_msg(uint8_t temp_value,uint8_t humi_value,uint16_t light)	
{
	uint8_t retval =0;	
  uint16_t count = 0;			
	static uint8_t error_count=0;
	unsigned char msg_buf[256];

  sprintf((char *)msg_buf,"AT+MQTTPUB=0,\""PUB_TOPIC"\",\""JSON_FORMAT"\",0,0\r\n",Red_value,temp_value,Blue_value,light,num_value,humi_value,direct_value,led_status,Green_value);
	HAL_UART_Transmit(&huart2, (unsigned char *)msg_buf,strlen((const char *)msg_buf), 1000);	
  HAL_UART_Transmit(&huart1, (unsigned char *)msg_buf,strlen((const char *)msg_buf), 1000);	
	while((receive_start == 0)&&(count<500))	
	{
		count++;
		HAL_Delay(1);
	}
	if(count >= 500)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(50);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
			error_count=0;
		}
		else 
		{
			error_count++;
			if(error_count==5)
			{
				error_count=0;
        printf("RECONNECT MQTT BROKER!!!\r\n");
				esp8266_init();
			}
		}
	}
  uart2_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief          esp8266接收数据
  * @param[in]      none
  * @retval         返回0接收数据正常,返回1接收数据异常或无数据
  */
uint8_t esp8266_receive_msg(uint8_t *num_value,uint8_t *direct_value,uint8_t *led_status,uint8_t *Red_value,uint8_t *Blue_value,uint8_t *Green_value)	
{
  uint8_t retval =0;
	int msg_len=0;
	uint8_t msg_body[128] = {0};
  
	if(receive_start == 1)	
	{
		do
    {
			receive_finish++;
			HAL_Delay(1);
		}
    while(receive_finish < 5);	
		
		if(strstr((const char*)receive_buf,"+MQTTSUBRECV:"))
		{
			sscanf((const char *)receive_buf,"+MQTTSUBRECV:0,\""SUB_TOPIC"\",%d,%s",&msg_len,msg_body);
      printf("len:%d,msg:%s\r\n",msg_len,msg_body);
			if(strlen((const char*)msg_body)== msg_len)
			{
        retval = parse_json_msg(msg_body,msg_len);
			}
      else
      {
        retval = 1;
      }
		}
    else 
    {
      retval = 1;
    }
	}
  else
  {
    retval = 1;
  }
  uart2_receiver_clear(receive_count);	
  return retval;
}
/**
  * @brief          esp8266初始化
  * @param[in]      none
  * @retval         none
  */
void esp8266_init(void)
{
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);           											//打开串口2接收中断
	
//	printf("1.SETTING STATION MODE\r\n");
////  OLED_printf(0,0,"1.SETTING STATION MODE                   ");
//	OLED_ShowString(0,0,"1.SET STATION",16);
//	while(esp8266_send_cmd((uint8_t *)"AT+CWMODE=1\r\n",strlen("AT+CWMODE=1\r\n"),"OK")!=0)
//	{
//		HAL_Delay(1000);
//	}
//	printf("2.CLOSE ESP8266 ECHO\r\n");
////  OLED_printf(0,0,"2.CLOSE ESP8266 ECHO                     ");
//	OLED_ShowString(0,0,"2.CLOSE ESP",16);
//	while(esp8266_send_cmd((uint8_t *)"ATE0\r\n",strlen("ATE0\r\n"),"OK")!=0)
//	{
//		HAL_Delay(1000);
//	}
//	printf("3.NO AUTO CONNECT WIFI\r\n"); 
////  OLED_printf(0,0,"3.NO AUTO CONNECT WIFI                   ");
//	OLED_ShowString(0,0,"3.NO AUTO",16);
//	while(esp8266_send_cmd((uint8_t *)"AT+CWAUTOCONN=0\r\n",strlen("AT+CWAUTOCONN=0\r\n"),"OK")!=0)
//	{
//		HAL_Delay(1000);
//	}
	printf("4.RESET ESP8266\r\n");
//  OLED_printf(0,0,"4.RESET ESP8266                          ");
	OLED_ShowString(0,0,"4.RESET ESP",16);
	while(esp8266_reset() != 0)
	{
		HAL_Delay(5000);
	}
	printf("5.CONFIG WIFI NETWORK\r\n");
//  OLED_printf(0,0,"5.CONFIG WIFI NETWORK                    ");
	OLED_ShowString(0,0,"5.CONFIG WIFI",16);
	while(esp8266_config_network() != 0)
	{
		HAL_Delay(8000);
	}
	printf("6.MQTT USER CONFIG\r\n");
//  OLED_printf(0,0,"6.MQTT USER CONFIG                       ");
	OLED_ShowString(0,0,"6.USER CONFIG",16);
	while(esp8266_send_cmd((uint8_t *)"AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"\r\n",
                          strlen("AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"\r\n"),"OK")!=0)
	{
		HAL_Delay(2000);
	}
	printf("7.CONNECT MQTT BROKER\r\n");
//  OLED_printf(0,0,"7.CONNECT MQTT BROKER                    ");
	OLED_ShowString(0,0,"7.CON MQTT",16);
	while(esp8266_connect_server() != 0)
	{
		HAL_Delay(8000);
	}
	printf("8.SUBSCRIBE TOPIC\r\n");
//  OLED_printf(0,0,"8.SUBSCRIBE TOPIC                ");
	OLED_ShowString(0,0,"8.SUB TOPIC",16);
	while(esp8266_send_cmd((uint8_t *)"AT+MQTTSUB=0,\""SUB_TOPIC"\",0\r\n",strlen("AT+MQTTSUB=0,\""SUB_TOPIC"\",0\r\n"),"OK")!=0)
	{
		HAL_Delay(2000);
	}
	printf("9.ESP8266 INIT OK!!!\r\n");
//  OLED_printf(0,0,"9.ESP8266 INIT OK!!!                ");
	OLED_ShowString(0,0,"9.ESP INIT OK!!!",16);
}





























