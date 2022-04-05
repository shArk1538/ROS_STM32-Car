#include "TIMER.h"

void TIMER_TIMx_Config(void)
{
	//重新初始化
	TIM_DeInit(TIMER_TIMx);
	
	//RCC时钟使能
	TIMER_TIMx_APBxClock_FUN(TIMER_TIMx_CLK, ENABLE);
	
	//计数器时基初始化
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = TIMER_ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_TIMx, &TIM_TimeBaseStructure);
	
	//计数器中断使能
	TIM_ClearFlag(TIMER_TIMx, TIM_FLAG_Update);
	TIM_ITConfig(TIMER_TIMx, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIMER_IT_IRQn;			//配置具体哪个中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//相应优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//计数值清零，使能计数器
	TIMER_TIMx->CNT = 0;
	TIM_Cmd(TIMER_TIMx, ENABLE);
}
