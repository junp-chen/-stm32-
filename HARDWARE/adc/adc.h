#ifndef _ADC_H
#define ADC_H
#include "sys.h"
void adc1_init(void);
u16 get_adc_value(u8 ch);
u16 get_adc_average(u8 ch,u8 times);//ch:通道 times:次数
#endif
