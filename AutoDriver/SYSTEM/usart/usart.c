#include "sys.h"
#include "usart.h"


//STM32F103核心板例程
//库函数版本例程
/********** mcudev.taobao.com 出品  ********/


////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//STM32开发板
//串口1初始化		   

////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
////加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
	int handle;
};

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{ 
	x = x;
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0) //循环发送,直到发送完毕   
    USART1->DR = (u8) ch;
	return ch;
}
#endif

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;			//接收状态标记

//数据接收状态
//bit7,		接收开始标志
//bit6~0,	接收到数据长度
u8 USART_DATA_STA=0;		//数据接收状态标记

int16_t Speed1; //左轮速度
int16_t Speed2; //右轮速度
float Distance;
int16_t MidDistance;

int16_t SpeedVal1; //左轮编码器读数
int16_t SpeedVal2; //右轮编码器读数

void uart_init(u32 bound){
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
    //USART1_TX   PA.9  发送数据给上位机
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9 GPIO9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10  接收上位机的数据
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化NVIC寄存器
  
    //USART 初始化设置
		USART_InitStructure.USART_BaudRate = bound;					//一般设置为9600;(115200)
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式

    USART_Init(USART1, &USART_InitStructure); 		//初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启中断
    USART_Cmd(USART1, ENABLE);                    	//使能串口

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1);	//读取接收到的数据(USART1->DR)
		
		if((USART_RX_STA&0x8000) == 0)		//接收未完成
		{
			if((USART_RX_STA&0x4000) == 1)		//接收到了0x0d
			{
				if(Res != 0x0a)					//没有接收到0x0a
				{
					USART_RX_STA = 0;			//接收错误,重新开始
				}
				else
				{
					USART_RX_STA |= 0x8000;		//接收到0x0a，接收完成了
				}
			}
			else 							//还没收到0X0d
			{
				if(Res == 0x0d)
				{
					USART_RX_STA |= 0x4000;	//接收到0x0d
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = Res;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1))	//接收数据错误,重新开始接收
					{
						USART_RX_STA = 0;
					}
				}
			}
		}
		USART_DataHandler();
	}
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();
#endif
}
#endif

/*
	自己实现的函数
*/

//发送

//直接以二进制码形式发送一个字节数据
//
//@param1:data,0x00~0xFF
//需要发送的8位数据
void USART_SendByte(uint8_t data)  //发送数据
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1, data);
}

//字符串形式发送一个字节数据，占用两个字节
//
//@param1:data,0x00~0xFF
//需要发送的8位数据
void USART_SendByteString(uint8_t data)
{
	uint8_t hexChar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	uint8_t data_high,data_low;
	data_high = (data&0xF0) >> 4;
	data_low = data&0x0F;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1, hexChar[data_high]);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1, hexChar[data_low]);
}

//字符串形式发送两个字节数据，占用四个字节
//
//@param1:data,0x0000~0xFFFF
//需要发送的16位数据
void USART_SendU16String(uint16_t data)
{
	uint8_t data_high,data_low;
	data_high = (data&0xFF00) >> 8;
	data_low = data&0x00FF;
	
	USART_SendByteString(data_high);
	USART_SendByteString(data_low);
}

//二进制码形式发送8位数组
//
//@param1:data[]
//需要发送的8位数组
//
//@param2:len
//发送数组的长度
void USART_SendArray(uint8_t *data, uint8_t len)
{
	while( len-- )
	{
		USART_SendByte(*data);
		data++;
	}
}

//二进制码形式发送字符串
//
//@param1:data
//需要发送的字符串
void USART_SendString(uint8_t *data)
{
	while( data!='\0' )
	{
		USART_SendByte(*data);
		data++;
	}
}

//二进制码形式发送浮点数
//
//@param1:data
//需要发送的浮点数
void USART_SendFloat(float data)
{
	uint8_t Usart_Tx_Buff[4];
	union FLOAT_UNI
	{
		uint8_t float_char[4];
		float float_val;
	}FLOAT;
	
	FLOAT.float_val = data;
	
	for(uint8_t i=0; i<4; i++)
	{
		Usart_Tx_Buff[i] = FLOAT.float_char[3-i];
	};
	
	for(uint8_t i=0; i<4; i++)
	{
		USART_SendByteString(Usart_Tx_Buff[i]);
	}
}

//接收

//清空接收缓冲区
void USART_ClearRXBuff(void)
{
	for(uint8_t i=0; i<USART_REC_LEN; i++)
	{
		USART_RX_BUF[i] = 0x00;
	}
}

//接收缓冲区所有数据向前移一位，将第一个数据舍弃
void USART_PushRXBuff(void)
{
	for(uint8_t i=0; i<USART_REC_LEN-2; i++)
	{
		USART_RX_BUF[i] = USART_RX_BUF[i+1];
	}
	USART_RX_BUF[USART_REC_LEN-1] = 0x00;
}

//将两个字符串转为一个u8
//
//@param1:hdata,'0'~'F'
//字符串数据高位
//
//@param1:ldata,'0'~'F'
//字符串数据低位
//
//@return:得到的u8数据
uint8_t USART_CharToHex(char hdata, char ldata)
{
	uint8_t hexChar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	uint8_t hres = 0x00;
	uint8_t lres = 0x00;
	while((hdata != hexChar[hres]) && (hres < 0x10))
	{
		hres++;
	}
	while((ldata != hexChar[lres]) && (lres < 0x10))
	{
		lres++;
	}
	return ((hres << 4) + lres);
}

//将USART_RX_BUF[3]-[10]位数据位转为float并返回
float USART_GetFloat(void)
{
	union FLOAT_UNI
	{
		uint8_t float_char[4];
		float float_val;
	}FLOAT;
	for(uint8_t i=3; i<11; i+=2)
	{
		FLOAT.float_char[3-((i-3)/2)] = USART_CharToHex(USART_RX_BUF[i], USART_RX_BUF[i+1]);
	}
	return FLOAT.float_val;
}

//处理接收到的数据
void USART_DataHandler(void)
{
	if((USART_RX_BUF[0] == 'S') && ((USART_DATA_STA & 0x80) != 0x80))
	{
		USART_DATA_STA |= 0x80;		//接收到'S'，开始接收数据
	}
	else if(USART_RX_BUF[(USART_DATA_STA&0X7F)+1] == 'X')
	{
		USART_DATA_STA &= 0x7F;		//接收到'X'，接收结束
		if(USART_DATA_STA == USART_DATA_LEN)	//数据长度正常时才进行处理	10
		{
			switch(USART_RX_BUF[1])
			{
				case 'V':			//改变速度
				{
					//分别控制做右轮速度，直线控制位置0
					StraightLineFlag = 0;
					if(USART_RX_BUF[2] == 'D')
					{
						uint8_t hexChar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
						uint8_t data;
						uint16_t uspeed1, uspeed2;
						for(uint8_t i=3; i<7; i++)
						{
							data = 0x00;
							while(USART_RX_BUF[i] != hexChar[data] && (data<0x10))
							{
								data++;
							}
							uspeed1 = uspeed1 << 4;
							uspeed1 += data;
						}
						for(uint8_t i=7; i<11; i++)
						{
							data = 0x00;
							while(USART_RX_BUF[i] != hexChar[data] && (data<0x10))
							{
								data++;
							}
							uspeed2 = uspeed2 << 4;
							uspeed2 += data;
						}
						int16_t rxspeed1 = ((int16_t) uspeed1) - 0x7FFF;
						int16_t rxspeed2 = ((int16_t) uspeed2) - 0x7FFF;
						if(rxspeed1 > 0x00A0)rxspeed1 = 0x00A0;
						if(rxspeed1 <-0x00A0)rxspeed1 =-0x00A0;
						if(rxspeed2 > 0x00A0)rxspeed2 = 0x00A0;
						if(rxspeed2 <-0x00A0)rxspeed2 =-0x00A0;
						Speed1 = rxspeed1;
						Speed2 = rxspeed2;
					}
					break;
				}
				case 'P':		//改变P参数
				{
					if(USART_RX_BUF[2] == 'R')
					{
						PID_SPEED1.Kp = USART_GetFloat();
					}
					else if(USART_RX_BUF[2] == 'L')
					{
						PID_SPEED2.Kp = USART_GetFloat();
					}
					break;
				}
				case 'I':		//改变I参数
				{
					if(USART_RX_BUF[2] == 'R')
					{
						PID_SPEED1.Ki = USART_GetFloat();
					}
					else if(USART_RX_BUF[2] == 'L')
					{
						PID_SPEED2.Ki = USART_GetFloat();
					}
					break;
				}
				case 'D':		//改变D参数
				{
					if(USART_RX_BUF[2] == 'R')
					{
						PID_SPEED1.Kd = USART_GetFloat();
					}
					else if(USART_RX_BUF[2] == 'L')
					{
						PID_SPEED2.Kd = USART_GetFloat();
					}
					break;
				}
				case 'K':		//控制标志位，用于紧急停车
				{
					if(USART_RX_BUF[2] == '0')
					{
						PARA_Init();
						
						TIM_SetCompare1(TIM2, 0);
						TIM_SetCompare2(TIM2, 0);
						TIM_SetCompare3(TIM2, 0);
						TIM_SetCompare4(TIM2, 0);
					}
					if(USART_RX_BUF[2] == '1')
					{
						ControlFlag = 1;
					}
					break;
				}
				case 'T':		//数据发送标志位，用于控制是否发送数据
				{
					if(USART_RX_BUF[2] == '1')
					{
						SendFlag = 1;
					}
					else if(USART_RX_BUF[2] == '0')
					{
						SendFlag = 0;
					}
					break;
				}
				case 'G':		//直线标志位，控制是否走直线
				{
					if(USART_RX_BUF[2] == '1')
					{
						StraightLineFlag = 1;
						Distance = 0;
						
						uint8_t hexChar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
						uint8_t data;
						uint16_t uspeed;
						for(uint8_t i=3; i<7; i++)
						{
							data = 0x00;
							while(USART_RX_BUF[i] != hexChar[data] && (data<0x10))
							{
								data++;
							}
							uspeed = uspeed << 4;
							uspeed += data;
						}
						int16_t rxspeed = ((int16_t) uspeed) - 0x7FFF;
						Speed1 = Speed2 = rxspeed;
						
						uint16_t udistance;
						for(uint8_t i=7; i<11; i++)
						{
							data = 0x00;
							while(USART_RX_BUF[i] != hexChar[data] && (data<0x10))
							{
								data++;
							}
							udistance = udistance << 4;
							udistance += data;
						}
						int16_t rxdistance = ((int16_t) udistance) - 0x7FFF;
						MidDistance = rxdistance;
					}
					else if(USART_RX_BUF[2] == '0')
					{
						StraightLineFlag = 0;
					}
					break;
				}
			}
		}
		USART_DATA_STA = 0x00;
		USART_ClearRXBuff();
	}
	else
	{
		if((USART_DATA_STA & 0x80) == 0x80)		//已经接收到'S'
		{
			USART_DATA_STA++;
		}
		else									//未接收到'S'，无用数据
		{
			USART_PushRXBuff();
		}
	}
}


