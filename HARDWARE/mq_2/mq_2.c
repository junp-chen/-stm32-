#include"mq_2.h"
#include"delay.h"
void mq_2_adc_init()//PA4,ADC1IN4
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72/6 = 9M
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��������ת������ⲿ�����¼�������ѡ�������������ʹ���ⲿ����
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStruct);

	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);//У׼
	ADC_StartCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	while(ADC_GetCalibrationStatus(ADC1));
}

//����ADC������ͨ���Լ���������,��ȡadcֵ
u16 get_mq2_adc()
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);//ADC1��ͨ��1������˳��Ϊ1������ʱ��Ϊ239.5������
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ʹ���������
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	return  ADC_GetConversionValue(ADC1);
}
u16 get_mq2_adc_aver()
{
	delay_init();
	u16 temp_val = 0;
	u8 t = 0;
	for(t = 0;t < 10;t++)
	{
		temp_val += get_mq2_adc();
		delay_ms(5);
	}
	return temp_val/10;
}
