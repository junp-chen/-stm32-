# -stm32-
项目描述：基于stm32单片机实现了一个具有防火防盗防烟雾，具有烟雾检测，温度检测，以及人体感应检测功能。并能通过oled屏幕显示烟雾浓度与温度，还可通过按键主动进行报警关闭，报警采用LED灯与蜂鸣器进行报警，并且在报警的同时可通过sim900a模块进行短信通知。
总结：烟雾检测采用MQ-2传感器进行ADC采集转化，温度检测采用的是DS18B20温度传感器进行检测，人体感应模块采用人体红外传感器进行检测，项目主要用到的知识点有定时器中断（报警倒计时），ADC采集转换（烟雾检测），串口通信（sim900a模块与单片机通信），SPI通信（oled模块与单片机通信）。
