#ifndef _DS18B20_H
#define _DS18B20_H
#include "sys.h"
#include"delay.h"
#define DS18B20_IO_IN() {GPIOA->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}//设置GPIO11为输入模式
#define DS18B20_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}//设置GPIO11为推完输出模式
#define DS18B20_DQ_OUT PAout(6)//GPIO_SetBits(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)
#define DS18B20_DQ_IN PAin(6)//GPIO_ReadInputDataBit(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)

void DS18B20_Rst(void);
u8 DS18B20_Check(void);
u8 DS18B20_Read_Bit(void);
u8 DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(u8 data);
void DS18B20_Start(void);
float DS18B20_Get_temp(void);
u8 DS18B20_Init(void);
#endif
