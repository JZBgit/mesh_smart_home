#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"
void uart2_receiver_handle(void);
uint8_t esp8266_receive_msg(uint8_t *num_value,uint8_t *direct_value,uint8_t *led_status,uint8_t *Red_value,uint8_t *Blue_value,uint8_t *Green_value);
uint8_t esp8266_send_msg(uint8_t temp_value,uint8_t humi_value,uint16_t light);
void esp8266_init(void);
#endif

