#include "LED.h"

//D13: LED1->PB0 ; D14: LED2->PB1

//LED��ʼ��

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����GPIO��ʼ���Ľṹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //LED0-->PB0, 1�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB��APB2����ʱ��ʹ��
	
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�����趨������ʼ��GPIOB0, 1
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1); //PB.1�����
	GPIO_SetBits(GPIOB,GPIO_Pin_0); //PB.0�����
}
