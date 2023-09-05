#include"dac.h"
void dac1_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 GPIO_InitTypeDef GPIO_InitStruct;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	 DAC_InitTypeDef DAC_InitStruct;
	 DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude  = DAC_LFSRUnmask_Bit0;
	 DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	 DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
	 DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1,&DAC_InitStruct);
	DAC_Cmd(DAC_Channel_1,ENABLE);

	DAC_SetChannel1Data(DAC_Align_12b_R,0);
}
void dac1_set_vol(u16 vol)
{
  vol = vol*(4095/3.3);
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);
}
