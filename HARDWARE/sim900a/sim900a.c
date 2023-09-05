#include"sim900a.h"
#include"string.h"
#include"usart2.h"
#include"usart.h"
#include"delay.h"
void sim_at_response()
{
	if(RECV_SR&0X8000)		//���յ�һ��������
	{ 
		USART2_RECV_BUF[RECV_SR&0X7FFF]=0;//��ӽ�����
		printf("%s",USART_RX_BUF);	//���͵�����
		RECV_SR=0;
	} 
}	
//str:�����Ľ��
//����0��δ�յ���ȷ��ACK
//�����������յ���ȷ��ACK
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(RECV_SR&0X8000)		//���յ�һ��������
	{ 
		USART_RX_BUF[RECV_SR&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;//����0����str��usart3_rx_buf��λ��
}
//��sim900a��������
//cmd:���͵��ַ����������16������ָ��
//ack:�յ���ȷ�ϻظ��ַ���
//waittime:�ȴ�ʱ��
//���صĽ����1������ʧ�� 0�����ͳɹ�
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waitime)
{
	u8 res = 0;//���صĽ����1������ʧ�� 0�����ͳɹ�
	usart2_init();
	if((u32)cmd < 0xff)//16������
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,cmd);
	}
	else//�����ַ���
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
				if(sim900a_check_cmd(ack))//�õ���ȷ��ACK
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
