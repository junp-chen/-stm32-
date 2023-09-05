#ifndef _SIM900A_H
#define _SIM900A_H
#include "sys.h"
extern char Chinese_message2_mq2[];
extern char Chinese_message_temp[];
extern char Unicode_phonenumber[];
u8 Find_char(char *a,char *b) ;
void sim900a_send_English_message(char *message,char *phonenumber);

void sim900a_send_Chinese_message(char *message,char *phonenumber);
#endif
