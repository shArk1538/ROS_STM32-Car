#include "ADC.h"


__IO uint16_t ADC_ConvertedValue[NOFCHANNEL]={0};

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;							//定义GPIO初始化的结构体
	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE);					//打开ADC1端口时钟
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	
	//DMA初始化
	DMA_DeInit(ADC_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(ADC_x->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = NOFCHANNEL;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	//ADC初始化
	ADC_InitTypeDef ADC_InitStructure;
	ADC_APBxClock_FUN(ADC_CLK, ENABLE);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANNEL;
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	//ADC时钟和转换周期
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	
	//打开DMA
	ADC_DMACmd(ADC_x, ENABLE);
	
	//打开ADC
	ADC_Cmd(ADC_x, ENABLE);
	
	//校准ADC
	ADC_StartCalibration(ADC_x);
	while( ADC_GetCalibrationStatus(ADC_x) );
	
	//开始转换
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}


void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
