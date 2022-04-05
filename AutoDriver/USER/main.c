#include "INCLUDE.h"
#include "usart.h"
#include "odometry.h"
#include "string.h"
#include "math.h"
#include "delay.h"

uint8_t SendFlag;
uint8_t ControlFlag;
uint8_t StraightLineFlag;

float Duty1 = 0; //电机pwm的参数
float Duty2 = 0;
float ADC_ConvertedValueLocal[NOFCHANNEL];

/**********************************************************************************************/
// 输出
char  odometry_data[21]={0};   //发送给串口的里程计数据数组
float odometry_right=0,odometry_left=0; //串口得到的左右轮速度

// 输入
extern float position_x,position_y,oriention,velocity_linear,velocity_angular; //计算得到的里程计数值 
extern float Milemeter_L_Motor,Milemeter_R_Motor;     //dt时间内的左右轮速度，用于里程计计算

// 变量
u8 main_sta=0; //用作处理主函数if,去掉多余的flag(1打印里程计)(2调用计算里程计数据函数)(3串口接收成功)(4失败)

union recieveData  //接受到的数据
{
    float d;    //左右轮速度
    unsigned char data[4];
}leftdata,rightdata;  //接收的左右轮数据

union odometry  //里程计数据共用体
{
    float odoemtry_float;
    unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;//要发布的里程计数据
/**********************************************************************************************/

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update && ControlFlag))
	{
		//编码器
		
		//编码器速度值 初始为0x7FFF,大于其为正转,小于其为反转
		//编码器安装时左右方向相反,其中一个的返回值取反
		SpeedVal1 = -(((int16_t) Get_TIMx1_CNT()) - 0x7FFF);	//左轮编码器读数
		SpeedVal2 =  ((int16_t) Get_TIMx2_CNT()) - 0x7FFF;	//右轮编码器读数
		
		//计算占空比
		if(StraightLineFlag == 1)	//走直线，先算左右轮距离差值，再让某一边的轮子减速
		{
			Distance += ((float)MidDistance)/10 + (float)(SpeedVal1 - SpeedVal2);			//左轮编码器读数-右轮编码器读数，>0则左轮快，右偏；反之则左偏
			float distance = -PID_Speed_Delta(&PID_DISTANCE, (int16_t)Distance, 0);
			Duty1 = PID_Speed_Delta(&PID_SPEED1, SpeedVal1, Speed1 - (int16_t)(distance)); //电机pwm参数计算
			Duty2 = PID_Speed_Delta(&PID_SPEED2, SpeedVal2, Speed2 + (int16_t)(distance));
		}
		else						//不走直线，直接设置给定速度
		{
			Duty1 = PID_Speed_Delta(&PID_SPEED1, SpeedVal1, Speed1);
			Duty2 = PID_Speed_Delta(&PID_SPEED2, SpeedVal2, Speed2);
		}
		
		//输出PWM
		PWM_SetDuty(Duty1, Duty2);
		
	}
	//清除中断标志位
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}


int main(void)
{
  HARDWARE_Init(); //包含了各类初始化
	PARA_Init(); //初始化手柄、PID等
	LED1 = 0;
	LED2 = 0;
	u8 t=0;
  u8 i=0,j=0,m=0;
	
	while(1)
	{
		/******************************************PS2手柄*************************************************/
		/**************************************************************************************************/
		if( !PS2_RedLight() )  //PS2手柄设置  其实目的是为了将手柄模拟量转换成speed1、2
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
			
			Speed2 = Speed1 = PS2_LY * 0.25;	//前进后退速度值
			if(PS2_LY == 0)					//前进后退速度为0，原地转弯
			{
				Speed1 =  PS2_RX * 0.05;
				Speed2 = -PS2_RX * 0.05;
			}
			else
			{								//前进后退有速度，左右转
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
		
		/******************************************里程计***********************************************/
		/***********************************************************************************************/
		//第一步：计算里程计数据
		if(main_sta&0x02)//执行计算里程计数据步骤
    {
        odometry(Milemeter_R_Motor,Milemeter_L_Motor); //计算里程计

        main_sta&=0xFD; //执行发送里程计数据步骤
    } 
		//第二步：发送里程计数据
		if(main_sta&0x01)//执行发送里程计数据步骤
    {
        //里程计数据获取
        x_data.odoemtry_float=position_x;  //单位mm
        y_data.odoemtry_float=position_y;  //单位mm
        theta_data.odoemtry_float=oriention; //单位rad
        vel_linear.odoemtry_float=velocity_linear; //单位mm/s
        vel_angular.odoemtry_float=velocity_angular; //单位rad/s

        //将里程计数据存到要发送的数组
        for(j=0;j<4;j++)
        {
            odometry_data[j]=x_data.odometry_char[j];
            odometry_data[j+4]=y_data.odometry_char[j];
            odometry_data[j+8]=theta_data.odometry_char[j];
            odometry_data[j+12]=vel_linear.odometry_char[j];
            odometry_data[j+16]=vel_angular.odometry_char[j];
        }

        odometry_data[20]='\n';//添加结束符

        //发送数据到串口，对于串口函数需要对应修改
        for(i=0;i<21;i++)
        {
            USART_ClearFlag(USART1,USART_FLAG_TC);   //在发送第一个数据前加此句，解决第一个数据不能正常发送的问题            
            USART_SendData(USART1,odometry_data[i]); //发送第一个字节到串口 
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  //等待发送结束          
        }

        main_sta&=0xFE;//执行计算里程计数据步骤
    }
		//如果上位机没有接收成功
    if(main_sta&0x08)        //当发送指令没有正确接收时
    {
        USART_ClearFlag(USART1,USART_FLAG_TC);  //在发送第一个数据前加此句，解决第一个数据不能正常发送的问题
        for(m=0;m<3;m++)
        {
            USART_SendData(USART1,0x00);    
            while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
        }       
        USART_SendData(USART1,'\n');    
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); 
        main_sta&=0xF7;
    }
		/**********************************************************************************************/
		
		//发送参数
		if(SendFlag)
		{
			USART_SendU16String(SpeedVal1);
			USART_SendU16String(SpeedVal2);
			
			USART_SendFloat(Duty1);
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
