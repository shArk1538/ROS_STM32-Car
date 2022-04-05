#include "INIT.h"

void HARDWARE_Init(void)
{
	//LED
	LED_Init();
	
	//PWM
	PWM_Init();
	
	//ADC
	//ADCx_Init();
	
	//CODER
	CODER_Init();
	
	//TIMER
	TIMER_TIMx_Config();
	
	//SYS
	delay_init();
	
	//NVIC
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	//USART1
	uart_init(115200);
	
	//PS2
	PS2_Init();
}
