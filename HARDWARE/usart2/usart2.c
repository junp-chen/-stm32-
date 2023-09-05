#include"usart2.h"
#include"my_time.h"

#include"string.h"
#include"stdio.h"
#include "stdarg.h"	 

u8 USART2_RECV_BUF[UASRT2_MAX_RECV_BUF] = {0};
u16 RECV_SR = 0; //[15]:完成一次数据的传输,[14,0]:一次接收到的数据
void usart2_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);

	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能中断
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		u8 data = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		if((RECV_SR & (1<<15)) == 0)
		{
			if(RECV_SR < UASRT2_MAX_RECV_BUF)
			{
				USART2_RECV_BUF[RECV_SR++] =data;	
				TIM3_Int_Init(999,7199);
				TIM_SetCounter(TIM3,0);//清空定时器
			}
			else
				RECV_SR |= 1<<15;//完成接收
		}
	}	
}

void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_RECV_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_RECV_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART3,USART2_RECV_BUF[j]); 
	} 
}

