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
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	//USART1
	uart_init(115200);
	
	//PS2
	PS2_Init();
}
