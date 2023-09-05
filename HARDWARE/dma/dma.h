#ifndef _DMA_H
#define _DMA_h
#include"sys.h"
void my_dam_init(DMA_Channel_TypeDef * DMAy_Channelx, u32 cpdr, u32 cmdr, u16 cndtr);
void my_dma_enable(DMA_Channel_TypeDef * DMAy_Channelx);
#endif
