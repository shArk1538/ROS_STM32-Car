#include "TIMER.h"

void TIMER_TIMx_Config(void)
{
	//���³�ʼ��
	TIM_DeInit(TIMER_TIMx);
	
	//RCCʱ��ʹ��
	TIMER_TIMx_APBxClock_FUN(TIMER_TIMx_CLK, ENABLE);
	
	//������ʱ����ʼ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = TIMER_ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMER_TIMx, &TIM_TimeBaseStructure);
	
	//�������ж�ʹ��
	TIM_ClearFlag(TIMER_TIMx, TIM_FLAG_Update);
	TIM_ITConfig(TIMER_TIMx, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIMER_IT_IRQn;			//���þ����ĸ��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//��Ӧ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//����ֵ���㣬ʹ�ܼ�����
	TIMER_TIMx->CNT = 0;
	TIM_Cmd(TIMER_TIMx, ENABLE);
}
