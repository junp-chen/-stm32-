#ifndef _USART3_H
#define _USART3_H
#include "sys.h"
#define UASRT2_MAX_RECV_BUF 500
extern u8 USART2_RECV_BUF[UASRT2_MAX_RECV_BUF];
extern u16 RECV_SR;
void usart2_init(void);
void u3_printf(char* fmt,...);

#endif

