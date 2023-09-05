#include"delay.h"
#include"adc.h"
void adc1_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC频率为12M
	ADC_DeInit( ADC1);
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_InitStruct);

	ADC_Cmd(ADC1,ENABLE);//开启adc1
	//校准结果
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}
//ch:通道
u16 get_adc_value(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//设定ADC的规则组通道，设置他们的转化顺序和采样时间
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//使能软件转换功能
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
u16 get_adc_average(u8 ch,u8 times)//ch:通道 times:次数
{
	u32 temp_val = 0;
	for(u8 t = 0;t < times;t++)
	{
		temp_val += get_adc_value(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
