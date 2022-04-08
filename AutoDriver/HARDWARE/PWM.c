#include "PWM.h"

//PWM1->PA0 ; PWM2->PA1 ; PWM3->PA2 ; PWM4->PA3

//初始化TIM2和PWM输出

void PWM_Init(void)  //PWM初始化
{	
	GPIO_InitTypeDef GPIO_InitStructure;							//定义GPIO初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;					//定义TIM时基初始化的结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义TIM输出初始化的结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//GPIOA的APB2总线时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //PA0,1,2,3端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 				//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure); 							//根据设定参数初始化GPIO PA0,1,2,3
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2的APB1总线时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = PWM_ARR;						//设置自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_PSC;					//设置用来作为TIMx时钟频率除数的预分频值  //PWM频率：72000k/800/9=10k
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//设置时钟分割：TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM2向上计数模式
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);					//TIM2时基初始化
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//输出模式设置为PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//有效极性为高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //TIM2输出初始化
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	TIM_SetCompare1(TIM2, 0);		//TIM2比较寄存器赋值，PWM波占空比=compareval/800
	TIM_SetCompare2(TIM2, 0);
	TIM_SetCompare3(TIM2, 0);
	TIM_SetCompare4(TIM2, 0);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);				//ch1~4 OC ENABLE
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);			//TIM2 ENABLE
}

//分别设置左右电机的PWM占空比
//@param1:Compare1,0~480 ((PWM_ARR + 1) * PWM_LIMIT)
//			(PWM占空比=compareval/800)
//@param2:Compare2,0~480 ((PWM_ARR + 1) * PWM_LIMIT)
//			(PWM占空比=compareval/800)
void PWM_SetDuty(float Compare1,float Compare2)
{
	uint16_t temp1 = 0;
	uint16_t temp2 = 0;
	if(Compare1 >= 0)
	{
		temp1 = (uint16_t) Compare1;
		if (temp1 > PWM_MAX_DUTY)
		{
			temp1 = PWM_MAX_DUTY;
		}
		TIM_SetCompare1(TIM2, temp1);
		TIM_SetCompare2(TIM2, 0);
	}
	else
	{
		temp1 = (uint16_t) -Compare1;
		if (temp1 > PWM_MAX_DUTY)
		{
			temp1 = PWM_MAX_DUTY;
		}
		TIM_SetCompare1(TIM2, 0);
		TIM_SetCompare2(TIM2, temp1);
	}
	if(Compare2 >= 0)
	{
		temp2 = (uint16_t) Compare2;
		if (temp2 > PWM_MAX_DUTY)
		{
			temp2 = PWM_MAX_DUTY;
		}
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, temp2);
	}
	else
	{
		temp2 = (uint16_t) -Compare2;
		if (temp2 > PWM_MAX_DUTY)
		{
			temp2 = PWM_MAX_DUTY;
		}
		TIM_SetCompare3(TIM2, temp2);
		TIM_SetCompare4(TIM2, 0);
	}
}
