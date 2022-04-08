#include "CODER.h"

//��������ʼ��

void TIMx_Encoder1_Config(void)  //������1 ����
{
	//���³�ʼ��
	TIM_DeInit(CODER_TIMx1);
	
	//RCCʱ��ʹ��
	CODER_TIMx1_APBxClock_FUN(CODER_TIMx1_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(CODER_GPIO1_CLK, ENABLE);
	
	//GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = CODER_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CODER_PORT1, &GPIO_InitStructure);
	
	//������ʱ����ʼ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = CODER_ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = CODER_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(CODER_TIMx1, &TIM_TimeBaseStructure);
	
	//���������벶���ʼ��
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(CODER_TIMx1, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(CODER_TIMx1, &TIM_ICInitStructure);
	
	//�������������ӿ�
	TIM_EncoderInterfaceConfig(CODER_TIMx1, CODER_EncoderMode, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	//�������ж�ʹ��
	TIM_ClearFlag(CODER_TIMx1, TIM_FLAG_Update);
	TIM_ITConfig(CODER_TIMx1, TIM_IT_Update, ENABLE);
	
	//����ֵ���㣬ʹ�ܼ�����
	CODER_TIMx1->CNT = 0x7FFF;
	TIM_Cmd(CODER_TIMx1, ENABLE);
}

void TIMx_Encoder2_Config(void)  //������2 ����
{
	//���³�ʼ��
	TIM_DeInit(CODER_TIMx2);
	
	//RCCʱ��ʹ��
	CODER_TIMx2_APBxClock_FUN(CODER_TIMx2_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(CODER_GPIO2_CLK, ENABLE);
	
	//GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = CODER_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CODER_PORT2, &GPIO_InitStructure);
	
	//������ʱ����ʼ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = CODER_ARR;
	TIM_TimeBaseStructure.TIM_Prescaler = CODER_PSC;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(CODER_TIMx2, &TIM_TimeBaseStructure);
	
	//���������벶���ʼ��
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(CODER_TIMx2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(CODER_TIMx2, &TIM_ICInitStructure);
	
	//�������������ӿ�
	TIM_EncoderInterfaceConfig(CODER_TIMx2, CODER_EncoderMode, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	//�������ж�ʹ��
	TIM_ClearFlag(CODER_TIMx2, TIM_FLAG_Update);
	TIM_ITConfig(CODER_TIMx2, TIM_IT_Update, ENABLE);
	
	//����ֵ���㣬ʹ�ܼ�����
	CODER_TIMx2->CNT = 0x7FFF;
	TIM_Cmd(CODER_TIMx2, ENABLE);
}

//��������ʼ��
void CODER_Init(void)
{
	TIMx_Encoder1_Config();
	TIMx_Encoder2_Config();
}

//��ȡ������ֵ
uint16_t Get_EncoderVal(TIM_TypeDef* TIMx)
{
	uint16_t temp = TIMx->CNT;
	TIMx->CNT = 0x7FFF;
	return temp;
}

uint16_t Get_TIMx1_CNT(void) //�����ٶ�ֵ
{
	return Get_EncoderVal(CODER_TIMx1);
}

uint16_t Get_TIMx2_CNT(void)  //�����ٶ�ֵ
{
	return Get_EncoderVal(CODER_TIMx2);
}
