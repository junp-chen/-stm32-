#ifndef MQ_2_H
#define MQ_2_H
#include "sys.h"
#define MQ2_IN PAin(4)
#define MQ2_OUT PAout(4)

void mq_2_adc_init(void);
u16 get_mq2_adc(void);
u16 get_mq2_adc_aver(void);

#endif
