#include "bh1750.h"
#include "stdlib.h"


void BH1750_Write(uint8_t *data, uint8_t datalen)
{
    HAL_I2C_Master_Transmit(&hi2c1, BHAddWrite, data, datalen, BH1750_Timeout);  //传输数据
}


void BH1750_Read(uint8_t *data)
{
    HAL_I2C_Master_Receive(&hi2c1, BHAddRead, data, 2, BH1750_Timeout);  //传输数据
}

void BH1750_Init(void)
{
    //这里有坑，只能一个个命令写，不能一起输入
    uint8_t Init_CMD[4]={0x01, BHPowOn, BHReset, BHModeH1};
    BH1750_Write(Init_CMD, 1);
    BH1750_Write(Init_CMD+1, 1);
    BH1750_Write(Init_CMD+2, 1);
    BH1750_Write(Init_CMD+3, 1);
}
