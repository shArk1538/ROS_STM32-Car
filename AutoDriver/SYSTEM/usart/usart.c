#include "sys.h"
#include "usart.h"


//STM32F103���İ�����
//�⺯���汾����
/********** mcudev.taobao.com ��Ʒ  ********/


////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//STM32������
//����1��ʼ��		   

////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
	int handle;
};

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{ 
	x = x;
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0) //ѭ������,ֱ���������   
    USART1->DR = (u8) ch;
	return ch;
}
#endif

/*ʹ��microLib�ķ���*/
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
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;			//����״̬���

//���ݽ���״̬
//bit7,		���տ�ʼ��־
//bit6~0,	���յ����ݳ���
u8 USART_DATA_STA=0;		//���ݽ���״̬���

int16_t Speed1; //�����ٶ�
int16_t Speed2; //�����ٶ�
float Distance;
int16_t MidDistance;

int16_t SpeedVal1; //���ֱ���������
int16_t SpeedVal2; //���ֱ���������

void uart_init(u32 bound){
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
    //USART1_TX   PA.9  �������ݸ���λ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9 GPIO9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10  ������λ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
  
    //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;					//һ������Ϊ9600;(115200)
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); 		//��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�����ж�
    USART_Cmd(USART1, ENABLE);                    	//ʹ�ܴ���

}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);	//��ȡ���յ�������(USART1->DR)
		
		if((USART_RX_STA&0x8000) == 0)		//����δ���
		{
			if((USART_RX_STA&0x4000) == 1)		//���յ���0x0d
			{
				if(Res != 0x0a)					//û�н��յ�0x0a
				{
					USART_RX_STA = 0;			//���մ���,���¿�ʼ
				}
				else
				{
					USART_RX_STA |= 0x8000;		//���յ�0x0a�����������
				}
			}
			else 							//��û�յ�0X0d
			{
				if(Res == 0x0d)
				{
					USART_RX_STA |= 0x4000;	//���յ�0x0d
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = Res;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1))	//�������ݴ���,���¿�ʼ����
					{
						USART_RX_STA = 0;
					}
				}
			}
		}
		USART_DataHandler();
	}
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();
#endif
}
#endif

/*
	�Լ�ʵ�ֵĺ���
*/

//����

//ֱ���Զ���������ʽ����һ���ֽ�����
//
//@param1:data,0x00~0xFF
//��Ҫ���͵�8λ����
void USART_SendByte(uint8_t data)  //��������
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1, data);
}

//�ַ�����ʽ����һ���ֽ����ݣ�ռ�������ֽ�
//
//@param1:data,0x00~0xFF
//��Ҫ���͵�8λ����
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

//�ַ�����ʽ���������ֽ����ݣ�ռ���ĸ��ֽ�
//
//@param1:data,0x0000~0xFFFF
//��Ҫ���͵�16λ����
void USART_SendU16String(uint16_t data)
{
	uint8_t data_high,data_low;
	data_high = (data&0xFF00) >> 8;
	data_low = data&0x00FF;
	
	USART_SendByteString(data_high);
	USART_SendByteString(data_low);
}

//����������ʽ����8λ����
//
//@param1:data[]
//��Ҫ���͵�8λ����
//
//@param2:len
//��������ĳ���
void USART_SendArray(uint8_t *data, uint8_t len)
{
	while( len-- )
	{
		USART_SendByte(*data);
		data++;
	}
}

//����������ʽ�����ַ���
//
//@param1:data
//��Ҫ���͵��ַ���
void USART_SendString(uint8_t *data)
{
	while( data!='\0' )
	{
		USART_SendByte(*data);
		data++;
	}
}

//����������ʽ���͸�����
//
//@param1:data
//��Ҫ���͵ĸ�����
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

//����

//��ս��ջ�����
void USART_ClearRXBuff(void)
{
	for(uint8_t i=0; i<USART_REC_LEN; i++)
	{
		USART_RX_BUF[i] = 0x00;
	}
}

//���ջ���������������ǰ��һλ������һ����������
void USART_PushRXBuff(void)
{
	for(uint8_t i=0; i<USART_REC_LEN-2; i++)
	{
		USART_RX_BUF[i] = USART_RX_BUF[i+1];
	}
	USART_RX_BUF[USART_REC_LEN-1] = 0x00;
}

//�������ַ���תΪһ��u8
//
//@param1:hdata,'0'~'F'
//�ַ������ݸ�λ
//
//@param1:ldata,'0'~'F'
//�ַ������ݵ�λ
//
//@return:�õ���u8����
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

//��USART_RX_BUF[3]-[10]λ����λתΪfloat������
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

//������յ�������
void USART_DataHandler(void)
{
	if((USART_RX_BUF[0] == 'S') && ((USART_DATA_STA & 0x80) != 0x80))
	{
		USART_DATA_STA |= 0x80;		//���յ�'S'����ʼ��������
	}
	else if(USART_RX_BUF[(USART_DATA_STA&0X7F)+1] == 'X')
	{
		USART_DATA_STA &= 0x7F;		//���յ�'X'�����ս���
		if(USART_DATA_STA == USART_DATA_LEN)	//���ݳ�������ʱ�Ž��д���	10
		{
			switch(USART_RX_BUF[1])
			{
				case 'V':			//�ı��ٶ�
				{
					//�ֱ�����������ٶȣ�ֱ�߿���λ��0
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
				case 'P':		//�ı�P����
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
				case 'I':		//�ı�I����
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
				case 'D':		//�ı�D����
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
				case 'K':		//���Ʊ�־λ�����ڽ���ͣ��
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
				case 'T':		//���ݷ��ͱ�־λ�����ڿ����Ƿ�������
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
				case 'G':		//ֱ�߱�־λ�������Ƿ���ֱ��
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
		if((USART_DATA_STA & 0x80) == 0x80)		//�Ѿ����յ�'S'
		{
			USART_DATA_STA++;
		}
		else									//δ���յ�'S'����������
		{
			USART_PushRXBuff();
		}
	}
}


