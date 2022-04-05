#ifndef __ADC_H
#define __ADC_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////

//			USE:ADC1
//			PIN:PA4			PA5
//				ADC1 IN4	ADC1 IN5
//			初始化ADC1,独立模式,DMA读取


////////////////////////////////////////////////////////////////////////


//ADC GPIO
#define		ADC_GPIO_APBxClock_FUN		RCC_APB2PeriphClockCmd
#define		ADC_GPIO_CLK				RCC_APB2Periph_GPIOA
#define		ADC_PORT					GPIOA


//ADC1/2/3
#define		ADC_APBxClock_FUN			RCC_APB2PeriphClockCmd
#define		ADC_x						ADC1
#define		ADC_CLK						RCC_APB2Periph_ADC1


//ADC Channel
#define		NOFCHANNEL					2

#define		ADC_PIN1					GPIO_Pin_4
#define		ADC_CHANNEL1				ADC_Channel_4

#define		ADC_PIN2					GPIO_Pin_5
#define		ADC_CHANNEL2				ADC_Channel_5


//DMA
#define		ADC_DMA_CLK					RCC_AHBPeriph_DMA1
#define		ADC_DMA_CHANNEL				DMA1_Channel1

extern __IO uint16_t ADC_ConvertedValue[NOFCHANNEL];

void ADCx_GPIO_Config(void);
void ADCx_Mode_Config(void);
void ADCx_Init(void);


#endif
