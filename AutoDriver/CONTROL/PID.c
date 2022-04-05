#include "PID.h"

PID_TypeDef PID_SPEED1;
PID_TypeDef PID_SPEED2;
PID_TypeDef PID_DISTANCE;

float PID_Speed_Delta(PID_TypeDef *PIDStruct, int16_t CurrentSpeed, int16_t SetSpeed)
{
	//计算误差
	PIDStruct->E_k_2 = PIDStruct->E_k_1;
	PIDStruct->E_k_1 = PIDStruct->E_k;
	PIDStruct->E_k   = SetSpeed - CurrentSpeed;
	PIDStruct->E_k_S = PIDStruct->E_k + PIDStruct->E_k_1 + PIDStruct->E_k_2;
	
	//计算PID输出值
	PIDStruct->Kp_OUT = PIDStruct->Kp * (PIDStruct->E_k - PIDStruct->E_k_1);
	PIDStruct->Ki_OUT = PIDStruct->Ki * PIDStruct->E_k;
	PIDStruct->Kd_OUT = PIDStruct->Kd * (PIDStruct->E_k - 2*PIDStruct->E_k_1 + PIDStruct->E_k_2);
	
	//增量式PID输出
	PIDStruct->PID_OUT += (PIDStruct->Kp_OUT + PIDStruct->Ki_OUT + PIDStruct->Kd_OUT);
	if(PIDStruct->PID_OUT > 500) PIDStruct->PID_OUT = 500;
	if(PIDStruct->PID_OUT <-500) PIDStruct->PID_OUT =-500;
	return PIDStruct->PID_OUT;
}
