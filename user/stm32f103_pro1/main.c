#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "my_time.h"
#include "pwm.h"
#include "adc.h"
#include "dac.h"
#include "hc_sr501.h"
#include "ds18b20.h"
#include "my_oled.h"
#include "mq_2.h"

#include "usart3.h"
#include "sim900a1.h"
 /*SIM900A短信内容和号码*/
 char Chinese_message_temp[]={"9AD86E2962A58B668B66544AFF01"};							 //温度报警
 char Chinese_message2_mq2[]={"9AD86D535EA670DF96FE8B66544AFF01"};									//烟雾报警
 char Unicode_phonenumber[]={"00310033003100300037003000340036003200370031"};


 u16 adc_mq2x = 0;	 //保存mq_2返回的ADC转化值,12位
 float mq2_vol = 0; //保存mq_2转化的电压值
 float temperature = 0; //保存返回的温度值，浮点数类型

 u8 flag_temp = 0;//温度警报标志
 u8 flag_mq2 = 0;//烟雾警报标志
 u8 flag_hr = 0;//人体感应标志
 u32 flag_60s_temp = 0;
 u32 flag_60s_mq2 = 0;
 u32 flag_3s_hr = 0;
 u8 flag_email_temp = 0;
 u8 flag_email_mq2 = 0;
 
 /**************时间标志位********************************/
 u8 time_oled_refresh = 0;

 void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组
	delay_init();
	uart_init(115200);//串口1：打印信息
	//Usart3_Init(115200);//串口2：stm32与sim900a通信 
	/*******模块初始化*********/
	
	LED_Init();  //按键
	BEEP_Init(); //蜂鸣器
	mq_2_adc_init(); //mq-2
	DS18B20_Init();//温度传感器
	hc_sr501_init();//人体感应检测模块
	OLED_Init();
	/***********显示界面初始化******************/
	OLED_ShowString(0,0,"ADC1_CH2_VAL:",12);
	OLED_ShowString(0,12,"ADC1_CH2_VoL:0.000V",12);
	OLED_ShowString(0,24,"TEMP:+000.00c",12);
	OLED_ShowString(0,52,"note:",12);
	OLED_Refresh_Gram();
	KEY_Init();
	TIM2_Init(4999,7199);
}

 int main(void)
 {	
	Hardware_Init();
	 int temp_int = 0;
	 u8 key_x = 0;

	while(1)
	{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
			{
				TIM_ClearFlag(TIM2,TIM_FLAG_Update);
				time_oled_refresh++;
			}
			
			if(time_oled_refresh == 1)
			{
				time_oled_refresh = 0;
				
				adc_mq2x = get_mq2_adc_aver();
				OLED_ShowNum(13*6,0,adc_mq2x,4,12);//显示数字量
				mq2_vol = (float)adc_mq2x*(3.3/4096)*1.5;//转化为电压值,最大为5V
				temp_int = mq2_vol;//取浮点数的整数部分
				OLED_ShowNum(13*6,12,temp_int,1,12);//显示整数部分
				temp_int = mq2_vol*1000;//取三位小数
				OLED_ShowNum(15*6,12,temp_int/100%10,1,12);
				OLED_ShowNum(16*6,13,temp_int%100/10,1,12);
				OLED_ShowNum(17*6,13,temp_int%10,1,12);

				temperature = DS18B20_Get_temp();
				if(temperature < 0)
				{
					temperature = -temperature;
					OLED_ShowChar(5*6,24,'-',12,1);
				}
				else
					OLED_ShowChar(5*6,24,'+',12,1);
				temp_int = (int)temperature;
				OLED_ShowNum(6*6,24,temp_int,3,12);
				temp_int = (int)(temperature*100);
				OLED_ShowNum(10*6,24,temp_int%100/10,1,12);
				OLED_ShowNum(11*6,24,temp_int%10,1,12);
				OLED_Refresh_Gram();
		}
	
	/******************按键检测**********************/

	key_x = KEY_Scan(0);
		if(key_x == KEY1_PRES)
		{
			flag_temp = 0;
			flag_60s_temp = 0;
			flag_email_temp = 0;
		//关闭LED1,蜂鸣器
			BEEP = 0;
			LED1 = 0;
		}
		else if(key_x == KEY2_PRES)
		{
				flag_mq2 = 0;
				flag_60s_mq2 = 0;
				flag_email_mq2 = 0;
				//关闭LED2,蜂鸣器
				BEEP = 0;
				LED2 = 0;
		}
		else if(key_x == KEY3_PRES)
		{
				flag_hr = 0;
				flag_3s_hr = 0;
				//关闭LED1,led2,蜂鸣器
				LED1 = 0;
				LED2 = 0;
				BEEP = 0;
		}	
/***************报警检测*****************************/

		if(temperature > 42 && flag_temp == 0)
		{
			flag_temp = 1;
			TIM3_Int_Init(4999,7199);
			
		}
		if(mq2_vol > 1.3 && flag_mq2 == 0)
		{
			flag_mq2 = 1;
			TIM3_Int_Init(4999,7199);
		}
		if(HC_SR_IN == 1 && flag_hr == 0)
		{
			flag_hr = 1;
			TIM3_Int_Init(4999,7199);
		}
		printf("HC_SR_IN = %d\n",HC_SR_IN);
		printf("flag_hr = %d\n",flag_hr);
		printf("flag_3s_hr = %d\n",flag_3s_hr);
		printf("adc_mq2x = %d\n",adc_mq2x);
		printf("mq2_vol = %f\n",mq2_vol);
		
 	}
		/*
		key = KEY_Scan(0);
		if(key == KEY1_PRES)
		{
			LED1 = !LED1;
		}
		else if(key == KEY2_PRES)
		{
			LED2 = !LED2;
		}
		else if(key == KEY3_PRES)
		{
			BEEP = !BEEP;
		}
		*/
	/*SIM900A短信内容和号码*/
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组
	delay_init();
	uart_init(115200);//串口1：打印信息
	Usart3_Init(115200);//串口2：stm32与sim900a通信 
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");


	sim900a_send_Chinese_message(Chinese_message2_mq2,Unicode_phonenumber);
	delay_ms(2000);
	sim900a_send_Chinese_message(Chinese_message_temp,Unicode_phonenumber);
	while(1);
	*/
 	/*
	delay_init();
	uart_init(115200);
	hc_sr501_init();
	while(1)
	{
		if(HC_SR_IN == 1)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	*/
/*
	delay_init();
	uart_init(115200);
	float temperature = 0;
	u8 yon = DS18B20_Init();
	while(yon != 0)
	{
		printf("未发现DS18B20\r\n");
		delay_ms(100);
		DS18B20_Rst();
		yon =	DS18B20_Check();  
	}
	while(1)
	{
		temperature = DS18B20_Get_temp();
		printf("%f\n",temperature);
		delay_ms(100);
	}
	*/
/*
	delay_init();
	OLED_Init();
	OLED_ShowString(0,0,"never give up",12);
	OLED_ShowString(0,12,"2023/5/14",12);
	OLED_ShowString(0,24,"ASCII:",12);
	OLED_ShowString(64,24,"CODE:",12);

	OLED_Refresh_Gram();
	u8 t = ' ';
	while(1)
	{
		OLED_ShowChar(36,24,t,16,1);
		OLED_Refresh_Gram();
		t++;
		if(t>'~')
			t = ' ';
		OLED_ShowNum(64+30,24,t,3,16);
		delay_ms(500);
	}
*/
/*	
	delay_init();
	uart_init(115200);

	OLED_Init();
	hc_sr501_init();
	LED_Init();
	OLED_ShowString(0,0,"ADC1_CH2_VAL:",12);
	OLED_ShowString(0,12,"ADC1_CH2_VoL:0.000V",12);
	OLED_ShowString(0,24,"TEMP:+000.00c",12);
	OLED_ShowString(0,52,"note:",12);
	OLED_Refresh_Gram();


	mq_2_adc_init();

	u8 yon = DS18B20_Init();
	while(yon != 0)
	{
		OLED_ShowString(5*6,52,"no DS18B20",12);
		OLED_Refresh_Gram();
		delay_ms(100);
		DS18B20_Rst();
		yon = DS18B20_Check();  
	}
	OLED_Fill(5*6,52,127,52,0);
	
//数据处理与显示

	u16 adc_mq2x = 0;
	float temperature = 0;
	float temp_f = 0;//用于存储返回的浮点数
	int temp_int = 0;
	
	while(1)
	{
		adc_mq2x = get_mq2_adc_aver();
		//OLED_ShowNum(13*6,0,adc_mq2x,4,12);//显示数字量
		printf("adcval:%d\n",adc_mq2x);
		temp_f = (float)adc_mq2x*(3.3/4096);//转化为电压值
		printf("adc_mq2x:%f\n",temp_f);
	
		if(HC_SR_IN == 1)
		{
			LED1 = 0;
		}
		else
			LED1 = 1;
	 	
		adc_mq2x = get_mq2_adc_aver();
		//OLED_ShowNum(13*6,0,adc_mq2x,4,12);//显示数字量
		printf("adcval:%d\n",adc_mq2x);
		temp_f = (float)adc_mq2x*(3.3/4096);//转化为电压值
		printf("adc_mq2x:%f\n",temp_f);
	 	temp_int = temp_f;//取浮点数的整数部分
		OLED_ShowNum(13*6,12,temp_int,1,12);//显示整数部分
		temp_int = (int)(temp_f*1000)%1000;//后三位小数
		OLED_ShowNum(15*6,12,temp_int,3,12); 
		

		temperature = DS18B20_Get_temp();
		printf("temperature:%f\n",temperature);
		if(temperature < 0)
		{
			temperature = -temperature;
			OLED_ShowChar(5*6,24,'-',12,1);
		}
		else
			OLED_ShowChar(5*6,24,'+',12,1);
		temp_int = (int)temperature;
		OLED_ShowNum(6*6,24,temp_int,3,12);
		temp_int = (int)(temperature*100);
		temp_int = 100;
		OLED_ShowNum(10*6,24,temp_int%100/10,1,12);
		OLED_ShowNum(11*6,24,temp_int%10,1,12);
		
		OLED_Refresh_Gram();
		delay_ms(250);//每隔250ms取值一次
	} */
	
 }
