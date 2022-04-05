#include "LED.h"

//D13: LED1->PB0 ; D14: LED2->PB1

//LED初始化

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义GPIO初始化的结构体
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //LED0-->PB0, 1端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB的APB2总线时钟使能
	
	GPIO_Init(GPIOB, &GPIO_InitStructure); //根据设定参数初始化GPIOB0, 1
	GPIO_SetBits(GPIOB,GPIO_Pin_1); //PB.1输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_0); //PB.0输出高
}
