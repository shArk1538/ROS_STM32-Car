#ifndef __PWM_H
#define __PWM_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////

//			USE:TIM2
//			CH1			CH2			CH3			CH4
//			PWM1->PA0 ; PWM2->PA1 ; PWM3->PA2 ; PWM4->PA3
//			��ʼ��PWM���������PWMռ�ձ�


////////////////////////////////////////////////////////////////////////

#define PWM_ARR 		799			//�Զ���װ�ؼĴ������ڵ�ֵ
#define PWM_PSC 		9			//������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
#define PWM_LIMIT 		0.6			//�޶�PWM���ķ�ֵ,36V*0.6=21.6V
#define PWM_MAX_DUTY	(u16)((PWM_ARR+1)*PWM_LIMIT)	//���ռ�ձȣ�800*0.6=480

void PWM_Init(void);
void PWM_SetDuty(float Compare1,float Compare2);

#endif
