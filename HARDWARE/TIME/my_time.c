#include"my_time.h"
#include"beep.h"
#include"led.h"
#include"sim900a1.h"
#include"usart.h"
#include "mq_2.h"
#include"my_oled.h"
#include "ds18b20.h"

extern  u8 flag_temp;//�¶Ⱦ�����־
extern u8 flag_mq2;//��������־
extern u8 flag_hr;//�����Ӧ��־
extern u32 flag_60s_temp ;
extern u32 flag_60s_mq2;
extern u32 flag_3s_hr;
extern u8 flag_email_temp;
extern u8 flag_email_mq2 ;

extern u16 adc_mq2x;	//����mq_2���ص�ADCת��ֵ,12λ
extern float mq2_vol; //����mq_2ת���ĵ�ѹֵ
extern float temperature; //���淵�ص��¶�ֵ������������
extern u8 flag_oled;

//arr:�Զ���װֵ
//psc:Ԥ��Ƶֵ
void TIM3_Int_Init(u16 arr,u16 psc)
{
	//ʹ�ܶ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//ʱ����Ԫ����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ�ܶ�ʱ�������ж�
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
		//ʱ����Ԫ����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);	

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ�ܶ�ʱ�������ж�
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
		/******************��������******************************/
		if(flag_60s_temp >= 60)//�¶ȱ���60S����ʱ����
		{
			flag_60s_temp = 0;
			flag_temp = 0;
			//�ر�LED,������
			LED1 = 0;
			BEEP = 0;
			flag_email_temp = 0;//�������������ٴη��Ͷ���
		}
		if(flag_60s_mq2 >= 60)//������60S����ʱ����
		{
			flag_60s_mq2 = 0;
			flag_mq2 = 0;
			//�ر�LED,������
			LED2 = 0;
			BEEP = 0;
			flag_email_mq2 = 0;//�������������ٴη��Ͷ���
		}
		if(flag_3s_hr > 6)
		{
			flag_3s_hr = 0;
			flag_hr = 0;
			//�رշ�������LED1,LED2
			LED1 = 0;
			LED2 = 0;
			BEEP = 0;
		}	
		if(flag_temp == 0 && flag_hr==0 && flag_mq2==0)
		{
			TIM_Cmd(TIM3,DISABLE);
			return;
		}
		/*****************������ʱ************************************************/
		if(flag_temp != 0)//�����¶ȱ���60s����ʱ
		{
			flag_60s_temp++;
			//LED1ȡ����˸������������
			LED1 = !LED1;
			BEEP = 1;
			if(flag_email_temp == 0)
			{
				//���͸���Ԥ������
				/*
				OLED_Fill(5,52,127,52,0);  
				OLED_ShowString(5,52,"send message...",12);
				sim900a_send_Chinese_message(Chinese_message_temp,Unicode_phonenumber);
				OLED_Fill(5,52,127,52,0);
				OLED_ShowString(5,52,"send over",12);
				*/
			}
		}
		if(flag_mq2 != 0)//����������60s����ʱ
		{
			flag_60s_mq2++;
			//LED2ȡ����˸������������
			LED2 = !LED2;
			BEEP = 1;
			if(flag_email_mq2 == 0)
			{
				//���ͱ�������
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
			//LED1����,LED2������������ȡ��
			LED1 = 1;
			LED2 = 1;
			BEEP = !BEEP;
		}

	}
}
