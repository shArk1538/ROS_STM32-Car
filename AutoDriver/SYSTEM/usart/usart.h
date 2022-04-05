#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "INCLUDE.h"

//STM32F103核心板例程
//库函数版本例程
/********** mcudev.taobao.com 出品  ********/

//////////////////////////////////////////////////////////////////////////////////	 
//STM32开发板
//串口1初始化		   

#define USART_REC_LEN			200			//定义最大接收字节数 200
#define EN_USART1_RX			1			//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN];		//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;					//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义

#define USART_DATA_LEN			10			//定义数据帧字节数 10
extern u8 USART_DATA_STA;					//数据接收状态标记

void uart_init(u32 bound);

//自己实现的函数

//send
void USART_SendByte(uint8_t data);
void USART_SendByteString(uint8_t data);
void USART_SendU16String(uint16_t data);
void USART_SendArray(uint8_t *data, uint8_t len);
void USART_SendString(uint8_t *data);
void USART_SendFloat(float data);

//receive
void USART_ClearRXBuff(void);
void USART_PushRXBuff(void);
void USART_DataHandler(void);

#endif
