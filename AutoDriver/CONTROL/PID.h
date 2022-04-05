#ifndef __PID_H
#define __PID_H
#include "INCLUDE.h"

typedef struct
{
	float Kp;						//����Pϵ��
	float Ki;						//����Iϵ��						
	float Kd;						//����Dϵ��
	
	float Kp_OUT;					//P���
	float Ki_OUT;					//I���
	float Kd_OUT;					//D���
	
	float E_k;						//��һ����Ŀ��Ĳ�ֵ
	float E_k_1;					//��һ�εĲ�ֵ������΢�֣�
	float E_k_2;					//�����εĲ�ֵ
	float E_k_S;					//�ۼӵĲ�ֵ�����ڻ��֣�
	
	float PID_OUT;					//PID���
}PID_TypeDef;


extern PID_TypeDef PID_SPEED1;				//�����ٶ�PID�ṹ��
extern PID_TypeDef PID_SPEED2;				//�ҵ���ٶ�PID�ṹ��
extern PID_TypeDef PID_DISTANCE;			//���ٿ���PID�ṹ��

float PID_Speed_Delta(PID_TypeDef *PIDStruct, int16_t CurrentSpeed, int16_t SetSpeed);


#endif
