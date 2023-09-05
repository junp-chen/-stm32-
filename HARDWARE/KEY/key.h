#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
#define KEY1 PEin(4)
#define KEY2 PEin(3)
#define KEY3 PAin(0)

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3

void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
