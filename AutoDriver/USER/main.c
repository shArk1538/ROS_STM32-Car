#include "INCLUDE.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "delay.h"
#include "odometry.h"

uint8_t SendFlag;
uint8_t ControlFlag;
uint8_t StraightLineFlag;

//float ADC_ConvertedValueLocal[NOFCHANNEL];

float Duty1 = 0; //���pwm�Ĳ���
float Duty2 = 0;

float Milemeter_L_Motor,Milemeter_R_Motor;  //�ɵ��ʵ���ٶȸ�ֵ��������̼Ƽ���

char  odometry_data[21]={0};   //���͸����ڵ���̼���������

extern float position_x,position_y,oriention,velocity_linear,velocity_angular; //����õ�����̼���ֵ������odometry.c

u8 main_sta = 0;  //����������if,(1��ӡ��̼�)(2���ü�����̼����ݺ���)(3���ڽ��ճɹ�)(4ʧ��)

union odometry  //��̼����ݹ�����
{
    float odoemtry_float;
    unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;//Ҫ��������̼�����

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update && ControlFlag))
	{
		//������
		
		//�������ٶ�ֵ ��ʼΪ0x7FFF,������Ϊ��ת,С����Ϊ��ת
		//��������װʱ���ҷ����෴,����һ���ķ���ֵȡ��
		SpeedVal1 = -(((int16_t) Get_TIMx1_CNT()) - 0x7FFF);	//����ʵ���ٶȣ�ͨ��coder.c����
		SpeedVal2 =  ((int16_t) Get_TIMx2_CNT()) - 0x7FFF;	  //����ʵ���ٶ�
		
		Milemeter_L_Motor = (float)SpeedVal1*10;  //תΪ����������λmm/s
		Milemeter_R_Motor = (float)SpeedVal2*10;
		
		main_sta |= 0x02;  //ִ�м�����̼����ݲ���
		
		//����ռ�ձ�
		if(StraightLineFlag == 1)	   //��ֱ�ߣ����������־����ֵ������ĳһ�ߵ����Ӽ���
		{
			Distance += ((float)MidDistance)/10 + (float)(SpeedVal1 - SpeedVal2);			//����ʵ���ٶ�-����ʵ���ٶȣ�>0�����ֿ죬��ƫ����֮����ƫ
			float distance = -PID_Speed_Delta(&PID_DISTANCE, (int16_t)Distance, 0);
			Duty1 = PID_Speed_Delta(&PID_SPEED1, SpeedVal1, Speed1 - (int16_t)(distance)); //PWMռ�ձȼ���
			Duty2 = PID_Speed_Delta(&PID_SPEED2, SpeedVal2, Speed2 + (int16_t)(distance));
		}
		else	//����ֱ�ߣ�ֱ�����ø����ٶ�
		{
			Duty1 = PID_Speed_Delta(&PID_SPEED1, SpeedVal1, Speed1);   //SpeedValΪʵ���ٶȣ�SpeedΪ�����ٶ�
			Duty2 = PID_Speed_Delta(&PID_SPEED2, SpeedVal2, Speed2);
		}
		
		//����ռ�ձȣ����PWM
		PWM_SetDuty(Duty1, Duty2);
	}
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  //����жϱ�־λ
	
	if( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) 
	{	
		main_sta|=0x01;//ִ�з�����̼����ݲ���  ��main_sta = mian_sta|0x01(�������)	
		
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);//����жϱ�־λ  		 
	}	
	
}


int main(void)
{
  HARDWARE_Init(); //�����˸����ʼ��
	PARA_Init(); //��ʼ���ֱ���PID��
	LED1 = 0;
	LED2 = 0;
	u8 i=0,j=0,m=0;
	
	while(1)
	{
		if( !PS2_RedLight() )  //PS2�ֱ�����  ��ʵĿ����Ϊ�˽��ֱ�ģ����ת����speed1��2�����������ٶ�
		{
			delay_ms(10);
			PS2_DataKey();
			
			int16_t PS2_LX = ((int16_t)PS2_AnologData(PSS_LX) - 0x80);	//128=0 0x80
			int16_t PS2_LY = ((int16_t)0x7F - PS2_AnologData(PSS_LY));	//127=0 0x7F
			int16_t PS2_RX = ((int16_t)PS2_AnologData(PSS_RX) - 0x80);	//128=0 0x80
			int16_t PS2_RY = ((int16_t)0x7F - PS2_AnologData(PSS_RY));	//127=0 0x7F
			
			StraightLineFlag = 0;
			
			if(PS2_RX == 0 && PS2_LY != 0)
			{
				StraightLineFlag = 1;
			}
			
			Speed2 = Speed1 = PS2_LY * 0.25;	//ǰ�������ٶ�ֵ
			if(PS2_LY == 0)					//ǰ�������ٶ�Ϊ0��ԭ��ת��
			{
				Speed1 =  PS2_RX * 0.05;
				Speed2 = -PS2_RX * 0.05;
			}
			else
			{								//ǰ���������ٶȣ�����ת
				float turn = ((float)PS2_RX / 0x0080) * 0.4;
				if(turn < 0)
				{
					Speed1 = Speed1 * (turn + 1);
				}
				else
				{
					Speed2 = Speed2 * (1 - turn);
				}
			}
			
			if(Speed1 > 50) Speed1 = 50;
			if(Speed1 <-50) Speed1 =-50;
			if(Speed2 > 50) Speed2 = 50;
			if(Speed2 <-50) Speed2 =-50;
		}
		else
		{
			delay_ms(10);
		}
		
//		ADC_ConvertedValueLocal[0] = (float) ( (float) ADC_ConvertedValue[0]/4096*3.3 - 1.65) * 20;
//		ADC_ConvertedValueLocal[1] = (float) ( (float) ADC_ConvertedValue[1]/4096*3.3 - 1.65) * 20;

		
//������̼���Ϣ����λ��
//
//@�ַ���21λ������5��4=20�����ݣ�����һ��������־λ"\n"
		if(main_sta&0x01)//ִ�� ���� ��̼����ݲ���
    {
        //��̼����ݻ�ȡ
        x_data.odoemtry_float=position_x;  //��λmm
        y_data.odoemtry_float=position_y;  //��λmm
        theta_data.odoemtry_float=oriention; //��λrad
        vel_linear.odoemtry_float=velocity_linear; //��λmm/s
        vel_angular.odoemtry_float=velocity_angular; //��λrad/s

        //����̼����ݴ浽Ҫ���͵�����
        for(j=0;j<4;j++)
        {
            odometry_data[j]=x_data.odometry_char[j];
            odometry_data[j+4]=y_data.odometry_char[j];
            odometry_data[j+8]=theta_data.odometry_char[j];
            odometry_data[j+12]=vel_linear.odometry_char[j];
            odometry_data[j+16]=vel_angular.odometry_char[j];
        }

        odometry_data[20]='\n';//��ӽ�����

        //�������ݵ�����
        for(i=0;i<21;i++)
        {
            USART_ClearFlag(USART1,USART_FLAG_TC);   //�ڷ��͵�һ������ǰ�Ӵ˾䣬�����һ�����ݲ����������͵�����            
            USART_SendData(USART1,odometry_data[i]); //���͵�һ���ֽڵ����� 
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  //�ȴ����ͽ���          
        }
				main_sta &= 0xFE; //ִ�� ���� ��̼����ݲ���
    }
		
		//������̼�����
		if(main_sta&0x02)//ִ�� ���� ��̼����ݲ���
    {
        odometry(Milemeter_R_Motor,Milemeter_L_Motor); //������̼�

        main_sta &= 0xFD; //ִ�� ���� ��̼����ݲ���
    } 
		
		//�����λ��û�н��ճɹ�
    if(main_sta&0x08)  //������ָ��û����ȷ����ʱ
    {
        USART_ClearFlag(USART1,USART_FLAG_TC);  //�ڷ��͵�һ������ǰ�Ӵ˾䣬�����һ�����ݲ����������͵�����
        for(m=0;m<3;m++)
        {
            USART_SendData(USART1,0x00);    
            while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
        }       
        USART_SendData(USART1,'\n');    
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); 
				
        main_sta&=0xF7;  //ִ��_____��־λ
    }
		
		
		//���Ͳ���
		if(SendFlag)
		{
			USART_SendU16String(SpeedVal1);  //�������ֱ�ӻ��
			USART_SendU16String(SpeedVal2);
			
			USART_SendFloat(Duty1);  //��Ҫ����SpeedVal��Speed����
			USART_SendFloat(Duty2);
			
			USART_SendFloat(PID_SPEED1.Kp);
			USART_SendFloat(PID_SPEED2.Kp);
			
			USART_SendFloat(PID_SPEED1.Ki);
			USART_SendFloat(PID_SPEED2.Ki);
			
			USART_SendFloat(PID_SPEED1.Kd);
			USART_SendFloat(PID_SPEED2.Kd);
			
		}
		delay_ms(200);
		
		/**********************************************************************************************/
		
	}//end while
} //end main
