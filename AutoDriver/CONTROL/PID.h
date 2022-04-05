#ifndef __PID_H
#define __PID_H
#include "INCLUDE.h"

typedef struct
{
	float Kp;						//正向P系数
	float Ki;						//正向I系数						
	float Kd;						//正向D系数
	
	float Kp_OUT;					//P输出
	float Ki_OUT;					//I输出
	float Kd_OUT;					//D输出
	
	float E_k;						//这一次与目标的差值
	float E_k_1;					//上一次的差值（用于微分）
	float E_k_2;					//上两次的差值
	float E_k_S;					//累加的差值（用于积分）
	
	float PID_OUT;					//PID输出
}PID_TypeDef;


extern PID_TypeDef PID_SPEED1;				//左电机速度PID结构体
extern PID_TypeDef PID_SPEED2;				//右电机速度PID结构体
extern PID_TypeDef PID_DISTANCE;			//差速控制PID结构体

float PID_Speed_Delta(PID_TypeDef *PIDStruct, int16_t CurrentSpeed, int16_t SetSpeed);


#endif
