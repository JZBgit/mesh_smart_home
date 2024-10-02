#include "main.h"
#include "stdlib.h"
#include "oled.h"
#include "oledfont.h"

uint8_t Coordinate[3]={0xb0, 0x00, 0x10};


void OLED_Write(uint8_t *data, uint8_t datalen, uint8_t cmd)
{
    OLED_DC(cmd);    //选择命令/数据
    OLED_CS(0);      //片选使能
    HAL_SPI_Transmit(&hspi1, data, datalen, OLED_Timeout);  //传输数据
    OLED_CS(1);    //取消片选
    OLED_DC(1);
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)  
{  
	uint8_t i, n;
    uint8_t zeros[128];
    uint8_t Ori_coordinate[3]={0xb0, 0x10, 0x00}; //设置页地址、列低地址、列高地址
    for(n=0;n<128;n++)zeros[n]=0x00;  //在次被坑，不要相信不赋初值默认为0，清屏会花屏
	for(i=0;i<8;i++)  //这个可以理解为循环8次每次代表一页Page
	{  
        OLED_Write(Ori_coordinate, 3, OLED_CMD);
	    OLED_Write(zeros, 128, OLED_DATA);     //128列依次写入字节0000 0000
        Ori_coordinate[0] += 1;
	} //更新显示
}

//初始化SSD1306					    
void OLED_Init(void)
{
    uint8_t Init_CMD[28]={0xAE, 0x00, 0x10, 0x40, 0x81, 0xCF, 0xA1, 0xC8, 0xA6, 0xA8, \
    0x3f, 0xD3, 0x00, 0xd5, 0x80, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x40, 0x20, 0x02, 0x8D,\
    0x14, 0xA4, 0xA6, 0xAF};

	OLED_RES(1);
	HAL_Delay(100);
    OLED_RES(0);
	HAL_Delay(100);
    OLED_RES(1);

	OLED_Write(Init_CMD, 28, OLED_CMD);
    OLED_Clear();	//清屏 
}

//坐标设置
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
    Coordinate[0]=0xb0+y;
    Coordinate[1]=((x&0xf0)>>4)|0x10;
    Coordinate[2]=(x&0x0f);
    OLED_Write(Coordinate, 3, OLED_CMD);
}  

//在指定位置显示字符串
//x:0~127
//y:0~63				 
//sizey:选择字体  8/16  6x8  8x16
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t sizey)
{      	
	uint8_t c;
	uint8_t j=0;
	while(chr[j]!='\0')
	{
        c=chr[j++]-' ';
        if(sizey==8)
        {
            OLED_Set_Pos(x,y);
            OLED_Write(asc2_0806[c], 6, OLED_DATA);
            x += 6;
        }else if(sizey==16){
            OLED_Set_Pos(x,y);
            OLED_Write(asc2_1608[c], 8, OLED_DATA);
            OLED_Set_Pos(x,y+1);
            OLED_Write(asc2_1608[c]+8, 8, OLED_DATA);
            x += 8;
        }
	}
}

// num取值范围:0~65535
void OLED_ShowNum(uint8_t x, uint8_t y, uint16_t num, uint8_t sizey)
{
	uint8_t c, len=0;
    uint8_t temp[5];
	while(num>9)
	{
        temp[len]=num%10;
        num /= 10;
        len++;
	}
    temp[len++]=num;
    while(len > 0)
    {
        len -= 1;
        c = temp[len] + 16;
        if(sizey==8)
        {
            OLED_Set_Pos(x,y);
            OLED_Write(asc2_0806[c], 6, OLED_DATA);
            x += 6;
        }else if(sizey==16){
            OLED_Set_Pos(x,y);
            OLED_Write(asc2_1608[c], 8, OLED_DATA);
            OLED_Set_Pos(x,y+1);
            OLED_Write(asc2_1608[c]+8, 8, OLED_DATA);
            x += 8;
        }
    }
}

