#ifndef _SIM900A_H
#define _SIM900A_H
#include"sys.h"
void sim_at_response(void);
u8* sim900a_check_cmd(u8 *str);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waitime);

#endif
