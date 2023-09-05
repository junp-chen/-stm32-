#include "sim900a1.h"
#include "sys.h"
#include "usart3.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

extern u8 Usart3_buff[128];     /*static和extern的区别*/
char dispbuf[64];


/**判断ACK是否正确，即相匹配**/
u8 Find_char(char *a,char *b)  //b为子串
{ 
  if(strstr(a,b)!=NULL)
	    return 0;
	else
			return 1;
}
/*
void sim900a_send_English_message(char *message,char *phonenumber)
{

	Usart_SendString2(USART3,"AT\r\n");                            //SIM900A是否与单片机来连接成功
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                      
	printf("English_message_OK1\r\n");
	
	Usart_SendString2(USART3,"AT&F\r\n");                           //SIM900A复位
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
	printf("English_message_OK2\r\n");	
	
	Usart_SendString2(USART3,"AT+CSCS=\"GSM\"\r\n");               //英文短信指令1
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     
	printf("English_message_OK3\r\n");
	
	
	Usart_SendString2(USART3,"AT+CMGF=1\r\n");                     //英文短信指令2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK4\r\n");
	
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",phonenumber);
	Usart_SendString2(USART3,dispbuf);                             //英文短信指令3
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK")); 
	printf("English_message_OK5\r\n");
	
	Usart_SendString2(USART3,message);                              //英文短信指令4
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK6\r\n");
	
	Usart_SendHalfWord(USART3,0x1a);                                //结束指令
	delay_ms(2000);  //延时两秒
	while(Find_char((char*)Usart3_buff,"OK"));  
	printf("English_message_OK7\r\n");
} 
*/
void sim900a_send_Chinese_message(char *message,char *phonenumber)
{ 
	
	Usart_SendString2(USART3,"AT\r\n");                           //SIM900A是否与单片机来连接成功
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
	
	Usart_SendString2(USART3,"AT&F\r\n");                          //SIM900A复位
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                    	
	
	Usart_SendString2(USART3,"AT+CSCS=\"UCS2\"\r\n");               //中文短信指令1
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                       
	
	
	Usart_SendString2(USART3,"AT+CMGF=1\r\n");                     //中文短信指令2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	Usart_SendString2(USART3,"AT+CSMP=17,167,2,25\r\n");            //中文短信指令2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",phonenumber);
	Usart_SendString2(USART3,dispbuf);                             //中文短信指令3
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK")); 
	
	Usart_SendString2(USART3,message);                              //中文短信指令4
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK"));  
	
	Usart_SendHalfWord(USART3,0x1a);                                //中文结束指令
	delay_ms(2000);  //延时两秒
	while(Find_char((char*)Usart3_buff,"OK"));  ;
} 


