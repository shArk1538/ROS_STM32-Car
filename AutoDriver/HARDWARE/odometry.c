#include "odometry.h"

/***********************************************  输出  *****************************************************************/

float position_x=0,position_y=0,oriention=0,velocity_linear=0,velocity_angular=0;

/***********************************************  输入  *****************************************************************/

extern float odometry_right,odometry_left; //串口得到的左右轮速度, main.c中被定义赋值

/***********************************************  变量  *****************************************************************/
//修改数据
float wheel_diameter=190.0f;     //轮子直径，单位mm
float dt=0.005f;                 //采样间隔时间5ms
float multiplier=4.0f;           //倍频数，4倍频
float deceleration_ratio=5.5f;  //减速比 从动轮直径110mm÷主动轮直径20mm
float line_number=200.0f;       //编码器码盘线数
float wheel_interval= 500.0f;   //两轮间距 50cm
//固定数据
float pi_1_2=1.570796f;          //pi/2
float pi=3.141593f;              //pi
float pi_3_2=4.712389f;          //pi*3/2
float pi_2_1=6.283186f;          //pi*2
float oriention_interval=0;      //dt时间内方向变化值

float sin_=0;  //角度计算值
float cos_=0;

float delta_distance=0,delta_oriention=0;   //采样时间间隔运动的距离

float const_frame=0,const_angle=0,distance_sum=0,distance_diff=0;

float oriention_1=0;

u8 once=1;

/****************************************************************************************************************/

//里程计计算函数
void odometry(float right,float left)
{   
    if(once)  //常数仅计算一次
    {
        const_frame=wheel_diameter*pi/(line_number*multiplier*deceleration_ratio); // 周长÷（线数×倍频数×减速比）
        const_angle=const_frame/wheel_interval;
        once=0;
    }

    distance_sum = 0.5f*(right+left); //在很短的时间内，小车行驶的路程为两轮速度和
    distance_diff = right-left; //在很短的时间内，小车形式的角度为两轮速度差

    //根据左右轮的方向，纠正短时间内，小车行驶的路程和角度量的正负
    if((odometry_right>0)&&(odometry_left>0))            //左右均正
    {
        delta_distance = distance_sum;
        delta_oriention = distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left<0))       //左右均负
    {
        delta_distance = -distance_sum;
        delta_oriention = -distance_diff;
    }
    else if((odometry_right<0)&&(odometry_left>0))       //左正右负
    {
        delta_distance = -distance_diff;
        delta_oriention = -2.0f*distance_sum;       
    }
    else if((odometry_right>0)&&(odometry_left<0))       //左负右正
    {
        delta_distance = distance_diff;
        delta_oriention = 2.0f*distance_sum;
    }
    else
    {
        delta_distance=0;
        delta_oriention=0;
    }

    oriention_interval = delta_oriention * const_angle; //采样时间内走的角度 
    oriention = oriention + oriention_interval; //计算出里程计方向角
    oriention_1 = oriention + 0.5f * oriention_interval; //里程计方向角数据位数变化，用于角函数计算

    sin_ = sin(oriention_1);//计算出采样时间内y坐标
    cos_ = cos(oriention_1);//计算出采样时间内x坐标

    position_x = position_x + delta_distance * cos_ * const_frame;//计算出里程计x坐标
    position_y = position_y + delta_distance * sin_ * const_frame;//计算出里程计y坐标

    velocity_linear = delta_distance*const_frame/dt;  //计算出里程计线速度
    velocity_angular = oriention_interval/dt;         //计算出里程计角速度

    //方向角角度纠正
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