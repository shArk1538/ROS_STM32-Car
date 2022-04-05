#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File��ps2Ӧ�ó���
Author��pinggai    Version:1.1     Data:2015/10/20
Description: ps2Ӧ�ó����������ֵ
             �����¡�L1��ʱ��С�𶯵��ת�������¡�R1��ʱ�����𶯵��ת����			   
History:  
V1.0: 	2015/05/16
1���������ֵ��ģ��ֵ
**********************************************************/	 	 	  
int main(void)
{								  
	u8 key;  
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,9600);  //����1��ʼ�� 
	LED_Init();
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ
	while(1)
	{
		LED =! LED;
		key=PS2_DataKey();
		if(key!=0)                   //�а�������
    	{
			printf("  \r\n   %d  is  light  \r\n",Data[1]);//ID
			printf("  \r\n   %d  is  pressed  \r\n",key);
			if(key == 11)
			{
				PS2_Vibration(0xFF,0x00);  //�����𶯺��������ʱ  delay_ms(1000);
				delay_ms(500);
			}
			else if(key == 12)
			{
				PS2_Vibration(0x00,0xFF);  //�����𶯺��������ʱ  delay_ms(1000);
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


