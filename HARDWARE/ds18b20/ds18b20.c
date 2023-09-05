#include"ds18b20.h"
#include"usart.h"
void DS18B20_Rst()
{
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT = 0;//����DQ
	delay_us(750);
	DS18B20_DQ_OUT = 1;//�ͷ�������
	delay_us(15);
}
//���DS18B20�Ƿ������Ӧ�ź�
u8 DS18B20_Check()
{
	u8 temp_time = 0;
	DS18B20_IO_IN();//����Ϊ��������
	while(DS18B20_DQ_IN)
	{
		if(temp_time >200)
			return 1;//һֱΪ�ߵ�ƽ��δ����Ӧ��
		temp_time++;
		delay_us(1);
	}
	temp_time = 0;
	while(DS18B20_DQ_IN == 0)
	{
		if(temp_time >= 240)
			return 1;//�͵�ƽʱ�����Ϊ240����������Ϊ��Ӧ��
		temp_time++;
		delay_us(1);
	}
	return 0;
}
u8 DS18B20_Read_Bit()
{
	u8 data = 0;
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT = 0;
	delay_us(2);
	DS18B20_DQ_OUT = 1;
	DS18B20_IO_IN();
	delay_us(12);
	data = (u8)DS18B20_DQ_IN;
	delay_us(50);
	return data;
}
u8 DS18B20_Read_Byte()
{
	u8 i,j,dat;
	dat = 0;
	for(i = 0;i < 8;i++)
	{
		j = DS18B20_Read_Bit();
		dat |= j<<i;
	}
	return dat;
}
void DS18B20_Write_Byte(u8 data)
{
	u8 temp_dat = 0;
	DS18B20_IO_OUT();
	u8 i;
	for(i = 0;i < 8;i++)
	{
		temp_dat = (data>>i)&0x01;
		if(temp_dat == 1)
		{
		    DS18B20_DQ_OUT=0;	// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);      
		}
		else
		{
			DS18B20_DQ_OUT=0;	// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2); 
		}
	}
}
void DS18B20_Start()
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
}
float DS18B20_Get_temp()
{
	u8 tem_sig = 0;
	short temp = 0;
	u8 TL=0,TH=0;
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();//
	DS18B20_Write_Byte(0xcc);	// skip rom
	DS18B20_Write_Byte(0xbe);	// ���洢������
	TL = DS18B20_Read_Byte();
	TH = DS18B20_Read_Byte();

	if(TH > 7)
	{
		TH=~TH;
    TL=~TL; 
		tem_sig = 0;//�¶�Ϊ��
	}
	else
	{
		tem_sig = 1;
	}
	temp = TH;
	temp <<= 8;
	temp += TL;
	if(tem_sig)
		return (float)temp*0.0625;
	else
		return -((float)(temp+1)*0.0625);
}
u8 DS18B20_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PORTG��ʱ�� 
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PORTG.11 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_6);    //���1,����״̬

	DS18B20_Rst();//��λ

	return DS18B20_Check();
	
}
