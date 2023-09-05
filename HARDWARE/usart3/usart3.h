#ifndef _USART3_H
#define _USART3_H
#include "sys.h"
extern u8 i3;
extern u8 Usart3_buff[];
#define USART_DEBUG		USART1		//调试打印所使用的串口组
void Usart3_Init(unsigned int baud);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void Usart_SendString2( USART_TypeDef * pUSARTx, char *str);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
#endif

