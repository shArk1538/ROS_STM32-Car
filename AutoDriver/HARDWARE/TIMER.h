#ifndef __TIMER_H
#define __TIMER_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////


//					�ɶ�ʱ��1��ʱ�����ж�
//					���������Բɼ������п���


////////////////////////////////////////////////////////////////////////

#define TIMER_TIMx 					TIM1
#define TIMER_TIMx_APBxClock_FUN	RCC_APB2PeriphClockCmd
#define TIMER_TIMx_CLK				RCC_APB2Periph_TIM1

#define TIMER_ARR					49
#define TIMER_PSC					7200

#define TIMER_IT_IRQn				TIM1_UP_IRQn

void TIMER_TIMx_Config(void);

#endif
