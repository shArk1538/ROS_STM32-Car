#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "INCLUDE.h"

//STM32F103���İ�����
//�⺯���汾����
/********** mcudev.taobao.com ��Ʒ  ********/

//////////////////////////////////////////////////////////////////////////////////	 
//STM32������
//����1��ʼ��		   

#define USART_REC_LEN			200			//�����������ֽ��� 200
#define EN_USART1_RX			1			//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN];		//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;					//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��

#define USART_DATA_LEN			10			//��������֡�ֽ��� 10
extern u8 USART_DATA_STA;					//���ݽ���״̬���

void uart_init(u32 bound);

//�Լ�ʵ�ֵĺ���

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
