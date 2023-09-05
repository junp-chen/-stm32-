#include "sim900a1.h"
#include "sys.h"
#include "usart3.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

extern u8 Usart3_buff[128];     /*static��extern������*/
char dispbuf[64];


/**�ж�ACK�Ƿ���ȷ������ƥ��**/
u8 Find_char(char *a,char *b)  //bΪ�Ӵ�
{ 
  if(strstr(a,b)!=NULL)
	    return 0;
	else
			return 1;
}
/*
void sim900a_send_English_message(char *message,char *phonenumber)
{

	Usart_SendString2(USART3,"AT\r\n");                            //SIM900A�Ƿ��뵥Ƭ�������ӳɹ�
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                      
	printf("English_message_OK1\r\n");
	
	Usart_SendString2(USART3,"AT&F\r\n");                           //SIM900A��λ
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //�ַ���ƥ�Ժ���   
	printf("English_message_OK2\r\n");	
	
	Usart_SendString2(USART3,"AT+CSCS=\"GSM\"\r\n");               //Ӣ�Ķ���ָ��1
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     
	printf("English_message_OK3\r\n");
	
	
	Usart_SendString2(USART3,"AT+CMGF=1\r\n");                     //Ӣ�Ķ���ָ��2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK4\r\n");
	
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",phonenumber);
	Usart_SendString2(USART3,dispbuf);                             //Ӣ�Ķ���ָ��3
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK")); 
	printf("English_message_OK5\r\n");
	
	Usart_SendString2(USART3,message);                              //Ӣ�Ķ���ָ��4
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK6\r\n");
	
	Usart_SendHalfWord(USART3,0x1a);                                //����ָ��
	delay_ms(2000);  //��ʱ����
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK7\r\n");
} 
*/
void sim900a_send_Chinese_message(char *message,char *phonenumber)
{ 
	
	Usart_SendString2(USART3,"AT\r\n");                           //SIM900A�Ƿ��뵥Ƭ�������ӳɹ�
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //�ַ���ƥ�Ժ���   
	
	Usart_SendString2(USART3,"AT&F\r\n");                          //SIM900A��λ
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                    	
	
	Usart_SendString2(USART3,"AT+CSCS=\"UCS2\"\r\n");               //���Ķ���ָ��1
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                       
	
	
	Usart_SendString2(USART3,"AT+CMGF=1\r\n");                     //���Ķ���ָ��2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	Usart_SendString2(USART3,"AT+CSMP=17,167,2,25\r\n");            //���Ķ���ָ��2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",phonenumber);
	Usart_SendString2(USART3,dispbuf);                             //���Ķ���ָ��3
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK")); 
	
	Usart_SendString2(USART3,message);                              //���Ķ���ָ��4
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	Usart_SendHalfWord(USART3,0x1a);                                //���Ľ���ָ��
	delay_ms(2000);  //��ʱ����
	while(Find_char((char*)Usart3_buff,"OK"));  ;
} 


