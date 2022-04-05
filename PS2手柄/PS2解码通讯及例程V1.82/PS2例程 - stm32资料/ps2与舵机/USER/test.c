#include <stm32f10x_lib.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
#include "servo.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��PS2���ƶ��
Author��pinggai    Version:1.0     Data:2015/05/06
Description: PS2���ƶ��
**********************************************************/	 	 	  
int main(void)
{								  
//	u8 key;
	u8 angle;  
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,9600);  //����1��ʼ�� 
	LED_Init();
	PS2_Init();

	TIM4_PWM_Init(20000,71); //TIM4��ʼ��������50HZ PWM
	while(1)
	{
		PS2_ClearData();   //�������
		PS2_ReadData();	   //������
		angle = (PS2_AnologData(PSS_RX)/5)*3+15;  //ֻ����15��~165����˶���Χ
		SERVO1(angle);
		delay_ms(50);
    }
}

























