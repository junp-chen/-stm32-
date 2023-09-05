#include "exti.h"
#include "usart.h"

//KEY1:PB5  KEY2:PB6  KEY3:PB7  KEY4:PB8
extern  u8 flag_temp;//温度警报标志
extern u8 flag_mq2;//烟雾警报标志
extern u8 flag_hr;//人体感应标志
extern u32 flag_60s_temp ;
extern u32 flag_60s_mq2 ;
extern u32 flag_3s_hr;
extern u8 flag_email_temp;
extern u8 flag_email_mq2;

void EXTIX_Init()
{
	KEY_Init();//配置为上拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);//中断线映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line5|EXTI_Line6|EXTI_Line7|EXTI_Line8;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
/*
void EXTI3_IRQHandler()
{
	delay_ms(10);//按键消抖
	if(KEY1 == 0)
	{
		flag_temp = 0;
		flag_60s_temp = 0;
		flag_email_temp = 0;
		//关闭LED1,蜂鸣器
		BEEP = 0;
		LED1 = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}
void EXTI4_IRQHandler()
{
	delay_ms(10);//按键消抖
	if(KEY2 == 0)
	{
		flag_mq2 = 0;
		flag_60s_mq2 = 0;
		flag_email_mq2 = 0;
		//关闭LED2,蜂鸣器
		BEEP = 0;
		LED2 = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}
*/
void EXTI9_5_IRQHandler()
{
/***********************KEY1************************/
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		delay_ms(10);//按键消抖
		if(EXTI_GetITStatus(EXTI_Line5) == SET)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			if(KEY1 == 0)
			{
				flag_temp = 0;
				flag_60s_temp = 0;
				flag_email_temp = 0;
				//关闭LED1,蜂鸣器
				BEEP = 0;
				LED1 = 0;
			}
		}
		
	}
/***********************KEY2************************/
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		delay_ms(10);//按键消抖
		if(EXTI_GetITStatus(EXTI_Line6) == SET)
		{
			EXTI_ClearITPendingBit(EXTI_Line6);
			if(KEY2 == 0)
			{
				flag_mq2 = 0;
				flag_60s_mq2 = 0;
				flag_email_mq2 = 0;
				//关闭LED2,蜂鸣器
				BEEP = 0;
				LED2 = 0;
			}

		}
		
	}
/***********************KEY3************************/
	if(EXTI_GetITStatus(EXTI_Line7) == SET)
	{
		delay_ms(10);//按键消抖
		if(EXTI_GetITStatus(EXTI_Line7) == SET)
		{
			EXTI_ClearITPendingBit(EXTI_Line7);
			if(KEY3 == 0)
			{
				flag_hr = 0;
				flag_3s_hr = 0;
				//关闭LED1,led2,蜂鸣器
				LED1 = 0;
				LED2 = 0;
				BEEP = 0;
			}

		}
		
	}
/*
	delay_ms(10);//按键消抖
	if(KEY3 == 0)
	{
		flag_hr = 0;
		flag_3s_hr = 0;
		//关闭LED1,led2,蜂鸣器
		LED1 = 0;
		LED2 = 0;
		BEEP = 0;
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
	*/
}

