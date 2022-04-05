#ifndef __PWM_H
#define __PWM_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////

//			USE:TIM2
//			CH1			CH2			CH3			CH4
//			PWM1->PA0 ; PWM2->PA1 ; PWM3->PA2 ; PWM4->PA3
//			初始化PWM输出，控制PWM占空比


////////////////////////////////////////////////////////////////////////

#define PWM_ARR 		799			//自动重装载寄存器周期的值
#define PWM_PSC 		9			//用来作为TIMx时钟频率除数的预分频值
#define PWM_LIMIT 		0.6			//限定PWM波的幅值,36V*0.6=21.6V
#define PWM_MAX_DUTY	(u16)((PWM_ARR+1)*PWM_LIMIT)	//最大占空比：800*0.6=480

void PWM_Init(void);
void PWM_SetDuty(float Compare1,float Compare2);

#endif
