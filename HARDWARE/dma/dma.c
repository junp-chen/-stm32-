#include"dma.h"
u16 dma1_mem_len = 0;
void my_dam_init(DMA_Channel_TypeDef * DMAy_Channelx,u32 cpdr,u32 cmdr,u16 cndtr)
{
	dma1_mem_len = cndtr;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//使能DMA时钟
	DMA_DeInit(DMAy_Channelx);
	
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = cndtr;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = cmdr;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr = cpdr;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMAy_Channelx,&DMA_InitStruct);
}
void my_dma_enable(DMA_Channel_TypeDef *DMAy_Channelx)//设置dma缓存大小,并使能dma通道
{
	DMA_Cmd(DMAy_Channelx,DISABLE);
	DMA_SetCurrDataCounter(DMAy_Channelx,dma1_mem_len);
	DMA_Cmd(DMAy_Channelx,ENABLE);
}
