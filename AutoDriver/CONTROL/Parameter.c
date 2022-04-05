#include "Parameter.h"


void PARA_Init(void)
{
	//USART1 Send Flag
	SendFlag = 1;
	
	//驱动板控制标志
	ControlFlag = 1;
	
	//走直线标志
	StraightLineFlag = 0;
	
	//手柄断连标志
	DisconnectFlag = 0;
	DisconnectCount = 0;
	
	//SetSpeed
	Speed1 = 0;
	Speed2 = 0;
	Distance = 0;
	MidDistance = 0;
	
	//CurrentSpeed
	SpeedVal1 = 0;
	SpeedVal2 = 0;
	
	{	//PID_SPEED1
		PID_SPEED1.Kp = 3;
		PID_SPEED1.Ki = 0.1;
		PID_SPEED1.Kd = 0;
		
		PID_SPEED1.Kp_OUT = 0;
		PID_SPEED1.Ki_OUT = 0;
		PID_SPEED1.Kd_OUT = 0;
		
		PID_SPEED1.E_k = 0;
		PID_SPEED1.E_k_1 = 0;
		PID_SPEED1.E_k_2 = 0;
		PID_SPEED1.E_k_S = 0;
		
		PID_SPEED1.PID_OUT = 0;
	}
	{	//PID_SPEED2
		PID_SPEED2.Kp = 3;
		PID_SPEED2.Ki = 0.1;
		PID_SPEED2.Kd = 0;
		
		PID_SPEED2.Kp_OUT = 0;
		PID_SPEED2.Ki_OUT = 0;
		PID_SPEED2.Kd_OUT = 0;
		
		PID_SPEED2.E_k = 0;
		PID_SPEED2.E_k_1 = 0;
		PID_SPEED2.E_k_2 = 0;
		PID_SPEED2.E_k_S = 0;
		
		PID_SPEED2.PID_OUT = 0;
	}
	{	//PID_DISTANCE
		PID_DISTANCE.Kp = 0.3;
		PID_DISTANCE.Ki = 0;
		PID_DISTANCE.Kd = 0;
		
		PID_DISTANCE.Kp_OUT = 0;
		PID_DISTANCE.Ki_OUT = 0;
		PID_DISTANCE.Kd_OUT = 0;
		
		PID_DISTANCE.E_k = 0;
		PID_DISTANCE.E_k_1 = 0;
		PID_DISTANCE.E_k_2 = 0;
		PID_DISTANCE.E_k_S = 0;
		
		PID_DISTANCE.PID_OUT = 0;
	}
}
