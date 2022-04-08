#include "PWM.h"

//PWM1->PA0 ; PWM2->PA1 ; PWM3->PA2 ; PWM4->PA3

//��ʼ��TIM2��PWM���

void PWM_Init(void)  //PWM��ʼ��
{	
	GPIO_InitTypeDef GPIO_InitStructure;							//����GPIO��ʼ���Ľṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;					//����TIMʱ����ʼ���Ľṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����TIM�����ʼ���Ľṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//GPIOA��APB2����ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //PA0,1,2,3�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 				//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure); 							//�����趨������ʼ��GPIO PA0,1,2,3
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2��APB1����ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = PWM_ARR;						//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_PSC;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  //PWMƵ�ʣ�72000k/800/9=10k
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//����ʱ�ӷָTDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM2���ϼ���ģʽ
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);					//TIM2ʱ����ʼ��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//���ģʽ����ΪPWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//��Ч����Ϊ��
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //TIM2�����ʼ��
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	TIM_SetCompare1(TIM2, 0);		//TIM2�ȽϼĴ�����ֵ��PWM��ռ�ձ�=compareval/800
	TIM_SetCompare2(TIM2, 0);
	TIM_SetCompare3(TIM2, 0);
	TIM_SetCompare4(TIM2, 0);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);				//ch1~4 OC ENABLE
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);			//TIM2 ENABLE
}

//�ֱ��������ҵ����PWMռ�ձ�
//@param1:Compare1,0~480 ((PWM_ARR + 1) * PWM_LIMIT)
//			(PWMռ�ձ�=compareval/800)
//@param2:Compare2,0~480 ((PWM_ARR + 1) * PWM_LIMIT)
//			(PWMռ�ձ�=compareval/800)
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
