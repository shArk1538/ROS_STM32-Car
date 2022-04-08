#include "odometry.h"

/***********************************************  ���  *****************************************************************/

float position_x=0,position_y=0,oriention=0,velocity_linear=0,velocity_angular=0;

/***********************************************  ����  *****************************************************************/

extern float odometry_right,odometry_left; //���ڵõ����������ٶ�, main.c�б����帳ֵ

/***********************************************  ����  *****************************************************************/
//�޸�����
float wheel_diameter=190.0f;     //����ֱ������λmm
float dt=0.005f;                 //�������ʱ��5ms
float multiplier=4.0f;           //��Ƶ����4��Ƶ
float deceleration_ratio=5.5f;  //���ٱ� �Ӷ���ֱ��110mm��������ֱ��20mm
float line_number=200.0f;       //��������������
float wheel_interval= 500.0f;   //���ּ�� 50cm
//�̶�����
float pi_1_2=1.570796f;          //pi/2
float pi=3.141593f;              //pi
float pi_3_2=4.712389f;          //pi*3/2
float pi_2_1=6.283186f;          //pi*2
float oriention_interval=0;      //dtʱ���ڷ���仯ֵ

float sin_=0;  //�Ƕȼ���ֵ
float cos_=0;

float delta_distance=0,delta_oriention=0;   //����ʱ�����˶��ľ���

float const_frame=0,const_angle=0,distance_sum=0,distance_diff=0;

float oriention_1=0;

u8 once=1;

/****************************************************************************************************************/

//��̼Ƽ��㺯��
void odometry(float right,float left)
{   
    if(once)  //����������һ��
    {
        const_frame=wheel_diameter*pi/(line_number*multiplier*deceleration_ratio); // �ܳ��£���������Ƶ�������ٱȣ�
        const_angle=const_frame/wheel_interval;
        once=0;
    }

    distance_sum = 0.5f*(right+left); //�ں̵ܶ�ʱ���ڣ�С����ʻ��·��Ϊ�����ٶȺ�
    distance_diff = right-left; //�ں̵ܶ�ʱ���ڣ�С����ʽ�ĽǶ�Ϊ�����ٶȲ�

    //���������ֵķ��򣬾�����ʱ���ڣ�С����ʻ��·�̺ͽǶ���������
    if((odometry_right>0)&&(odometry_left>0))            //���Ҿ���
    {
        delta_distance = distance_sum;
        delta_oriention = distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left<0))       //���Ҿ���
    {
        delta_distance = -distance_sum;
        delta_oriention = -distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left>0))       //�����Ҹ�
    {
        delta_distance = -distance_diff;
        delta_oriention = -2.0f*distance_sum;       
    }
    else if((odometry_right>0)&&(odometry_left<0))       //������
    {
        delta_distance = distance_diff;
        delta_oriention = 2.0f*distance_sum;
    }
    else
    {
        delta_distance=0;
        delta_oriention=0;
    }

    oriention_interval = delta_oriention * const_angle; //����ʱ�����ߵĽǶ� 
    oriention = oriention + oriention_interval; //�������̼Ʒ����
    oriention_1 = oriention + 0.5f * oriention_interval; //��̼Ʒ��������λ���仯�����ڽǺ�������

    sin_ = sin(oriention_1);//���������ʱ����y����
    cos_ = cos(oriention_1);//���������ʱ����x����

    position_x = position_x + delta_distance * cos_ * const_frame;//�������̼�x����
    position_y = position_y + delta_distance * sin_ * const_frame;//�������̼�y����

    velocity_linear = delta_distance*const_frame/dt;  //�������̼����ٶ�
    velocity_angular = oriention_interval/dt;         //�������̼ƽ��ٶ�

    //����ǽǶȾ���
    if(oriention > pi)
    {
        oriention -= pi_2_1;
    }
    else
    {
        if(oriention < -pi)
        {
            oriention += pi_2_1;
        }
    }
}