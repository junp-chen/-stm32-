#include"my_time.h"
#include"beep.h"
#include"led.h"
#include"sim900a1.h"
#include"usart.h"
#include "mq_2.h"
#include"my_oled.h"
#include "ds18b20.h"

extern  u8 flag_temp;//温度警报标志
extern u8 flag_mq2;//烟雾警报标志
extern u8 flag_hr;//人体感应标志
extern u32 flag_60s_temp ;
extern u32 flag_60s_mq2;
extern u32 flag_3s_hr;
extern u8 flag_email_temp;
extern u8 flag_email_mq2 ;

extern u16 adc_mq2x;	//保存mq_2返回的ADC转化值,12位
extern float mq2_vol; //保存mq_2转化的电压值
extern float temperature; //保存返回的温度值，浮点数类型
extern u8 flag_oled;

//arr:自动重装值
//psc:预分频值
void TIM3_Int_Init(u16 arr,u16 psc)
{
	//使能定时器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//时基单元配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能定时器更新中断
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM3,ENABLE);	
}

void TIM2_Init(u16 arr,u16 psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		//时基单元配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);	

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能定时器更新中断
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM2,ENABLE);
}
void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		/******************报警结束******************************/
		if(flag_60s_temp >= 60)//温度报警60S倒计时结束
		{
			flag_60s_temp = 0;
			flag_temp = 0;
			//关闭LED,蜂鸣器
			LED1 = 0;
			BEEP = 0;
			flag_email_temp = 0;//报警结束，可再次发送短信
		}
		if(flag_60s_mq2 >= 60)//烟雾报警60S倒计时结束
		{
			flag_60s_mq2 = 0;
			flag_mq2 = 0;
			//关闭LED,蜂鸣器
			LED2 = 0;
			BEEP = 0;
			flag_email_mq2 = 0;//报警结束，可再次发送短信
		}
		if(flag_3s_hr > 6)
		{
			flag_3s_hr = 0;
			flag_hr = 0;
			//关闭蜂鸣器，LED1,LED2
			LED1 = 0;
			LED2 = 0;
			BEEP = 0;
		}	
		if(flag_temp == 0 && flag_hr==0 && flag_mq2==0)
		{
			TIM_Cmd(TIM3,DISABLE);
			return;
		}
		/*****************报警计时************************************************/
		if(flag_temp != 0)//开启温度报警60s倒计时
		{
			flag_60s_temp++;
			//LED1取反闪烁，蜂鸣器鸣响
			LED1 = !LED1;
			BEEP = 1;
			if(flag_email_temp == 0)
			{
				//发送高温预警短信
				/*
				OLED_Fill(5,52,127,52,0);  
				OLED_ShowString(5,52,"send message...",12);
				sim900a_send_Chinese_message(Chinese_message_temp,Unicode_phonenumber);
				OLED_Fill(5,52,127,52,0);
				OLED_ShowString(5,52,"send over",12);
				*/
			}
		}
		if(flag_mq2 != 0)//开启烟雾报警60s倒计时
		{
			flag_60s_mq2++;
			//LED2取反闪烁，蜂鸣器鸣响
			LED2 = !LED2;
			BEEP = 1;
			if(flag_email_mq2 == 0)
			{
				//发送报警短信
				/*
				OLED_Fill(5,52,127,52,0);  
				OLED_ShowString(5,52,"send message...",12);
				sim900a_send_Chinese_message(Chinese_message2_mq2,Unicode_phonenumber);
				OLED_Fill(5,52,127,52,0);
				OLED_ShowString(5,52,"send over",12);
				*/
			}
		}
		if(flag_hr != 0)
		{
			flag_3s_hr++;
			//LED1常亮,LED2常亮，蜂鸣器取反
			LED1 = 1;
			LED2 = 1;
			BEEP = !BEEP;
		}

	}
}
