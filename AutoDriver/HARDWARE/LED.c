#include "LED.h"

//D13: LED1->PB0 ; D14: LED2->PB1

//LED³õÊ¼»¯

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//¶¨ÒåGPIO³õÊ¼»¯µÄ½á¹¹Ìå¡
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //LED0-->PB0, 1¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO¿ÚËÙ¶ÈÎª50MHz
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOBµÄAPB2×ÜÏßÊ±ÖÓÊ¹ÄÜ
	
	GPIO_Init(GPIOB, &GPIO_InitStructure); //¸ù¾İÉè¶¨²ÎÊı³õÊ¼»¯GPIOB0, 1
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1); //PB.1Êä³ö¸ß
	GPIO_SetBits(GPIOB,GPIO_Pin_0); //PB.0Êä³ö¸ß
}
