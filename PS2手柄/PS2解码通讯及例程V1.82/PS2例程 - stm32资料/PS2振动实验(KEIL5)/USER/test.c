#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：ps2应用程序
Author：pinggai    Version:1.1     Data:2015/10/20
Description: ps2应用程序输出按键值
             当按下“L1”时，小震动电机转动，按下“R1”时，大震动电机转动。			   
History:  
V1.0: 	2015/05/16
1、输出按键值，模拟值
**********************************************************/	 	 	  
int main(void)
{								  
	u8 key;  
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	uart_init(72,9600);  //串口1初始化 
	LED_Init();
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                     //开启震动模式
	while(1)
	{
		LED =! LED;
		key=PS2_DataKey();
		if(key!=0)                   //有按键按下
    	{
			printf("  \r\n   %d  is  light  \r\n",Data[1]);//ID
			printf("  \r\n   %d  is  pressed  \r\n",key);
			if(key == 11)
			{
				PS2_Vibration(0xFF,0x00);  //发出震动后必须有延时  delay_ms(1000);
				delay_ms(500);
			}
			else if(key == 12)
			{
				PS2_Vibration(0x00,0xFF);  //发出震动后必须有延时  delay_ms(1000);
				delay_ms(500);
			}
			else
				 PS2_Vibration(0x00,0x00); 
    	}
		printf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
		delay_ms(50);
	}	 
}


