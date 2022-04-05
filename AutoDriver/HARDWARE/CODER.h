#ifndef __CODER_H
#define __CODER_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////


//					TIM3 CH1->PA6 ; CH2->PA7
//					TIM4 CH1->PB6 ; CH2->PB7
//					编码器输入初始化


////////////////////////////////////////////////////////////////////////

//CODER CLK
#define CODER_TIMx1					TIM3
#define CODER_TIMx1_APBxClock_FUN	RCC_APB1PeriphClockCmd
#define CODER_TIMx1_CLK				RCC_APB1Periph_TIM3
#define CODER_GPIO1_CLK				RCC_APB2Periph_GPIOA

#define CODER_TIMx2					TIM4
#define CODER_TIMx2_APBxClock_FUN	RCC_APB1PeriphClockCmd
#define CODER_TIMx2_CLK				RCC_APB1Periph_TIM4
#define CODER_GPIO2_CLK				RCC_APB2Periph_GPIOB

//GPIO PIN
#define CODER_PORT1					GPIOA
#define CODER_PIN1					GPIO_Pin_6 | GPIO_Pin_7

#define CODER_PORT2					GPIOB
#define CODER_PIN2					GPIO_Pin_6 | GPIO_Pin_7

//TIM_TimeBaseStructure
#define CODER_ARR					0xFFFF
#define CODER_PSC					0

//EncoderMode
#define CODER_EncoderMode			TIM_EncoderMode_TI12

void TIMx_Encoder1_Config(void);
void TIMx_Encoder2_Config(void);
void CODER_Init(void);								//编码器接口模式初始化
uint16_t Get_EncoderVal(TIM_TypeDef * TIMx);
uint16_t Get_TIMx1_CNT(void);
uint16_t Get_TIMx2_CNT(void);						//读取指定计数器的CNT值并将其清零

#endif
