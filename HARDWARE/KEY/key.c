#include "key.h"
#include "sys.h"
#include "delay.h"
//KEY1:PB5  KEY2:PB6  KEY3:PB7  KEY4:PB8
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
u8 KEY_Scan(u8 mode) //mode == 1支持连按,mode == 0,不支持连按
{
	static u8 key_up = 1;
	if(mode)
		key_up = 1;//支持连按
	if(key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0))
	{
		delay_ms(10);
		key_up = 0;
		if(KEY1 == 0)
			return KEY1_PRES;
		else if(KEY2 == 0)
			return KEY2_PRES;
		else if(KEY3 == 0)
			return KEY3_PRES;
	}
	else if(KEY1 == 1 && KEY2 == 1 && KEY3 == 1)
	{
		key_up = 1;
	}
	return 0;
}
