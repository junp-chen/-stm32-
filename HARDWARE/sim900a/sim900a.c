#include"sim900a.h"
#include"string.h"
#include"usart2.h"
#include"usart.h"
#include"delay.h"
void sim_at_response()
{
	if(RECV_SR&0X8000)		//接收到一次数据了
	{ 
		USART2_RECV_BUF[RECV_SR&0X7FFF]=0;//添加结束符
		printf("%s",USART_RX_BUF);	//发送到串口
		RECV_SR=0;
	} 
}	
//str:期望的结果
//返回0：未收到正确的ACK
//返回其他：收到正确的ACK
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(RECV_SR&0X8000)		//接收到一次数据了
	{ 
		USART_RX_BUF[RECV_SR&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;//返回0或者str在usart3_rx_buf的位置
}
//向sim900a发送命令
//cmd:发送的字符串命令或者16进制数指令
//ack:收到的确认回复字符串
//waittime:等待时间
//返回的结果，1：发送失败 0：发送成功
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waitime)
{
	u8 res = 0;//返回的结果，1：发送失败 0：发送成功
	usart2_init();
	if((u32)cmd < 0xff)//16进制数
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,cmd);
	}
	else//发送字符串
	{
		u3_printf(cmd);
	}
	if(ack&&waitime)
	{
		delay_init();
		while(waitime--)
		{
			delay_ms(10);
			if((RECV_SR &(1<<15)) == 1)
			{
				if(sim900a_check_cmd(ack))//得到正确的ACK
				{
					res = 0;
				}
				else
				{
					res = 1;
				}
				break;
			}
		}
		if(waitime == 0)
			res = 1;
	}
	return res;
}
