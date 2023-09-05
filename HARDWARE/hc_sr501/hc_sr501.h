#ifndef HC_SR501 
#define HC_SR501
#include "sys.h"

#define HC_SR_IN PAin(5)
#define HC_SR_OUT PAout(5)
void hc_sr501_init(void);
#endif
