/*
 * init.c
 *
 *  Created on: 2023年6月3日
 *      Author: Innovator
 */

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "init.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/


void InitAll( void )
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED2 输出 默认高电平 推挽输出模式

    // PWM 有刷电机初始化
    gpio_init(POWER_MOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                  // GPIO 初始化为输出 默认上拉输出高
    pwm_init (POWER_MOTOR_PWM, 17000, 0);                                       // PWM 通道初始化频率 17KHz 占空比初始为 0
    encoder_quad_init(ENCODER_QUADDEC, ENCODER_QUADDEC_A, ENCODER_QUADDEC_B);   // 初始化编码器模块与引脚 正交解码编码器模式

    // PWM 无刷电机初始化
    pwm_init (LEFT_FLYWHEEL_PWM, 17000, LEFT_FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                  // 初始化左侧飞轮PWM信号
    gpio_init(LEFT_FLYWHEEL_DIR, GPO, LEFT_FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                              // 初始化左侧飞轮DIR信号
    gpio_init(LEFT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                  // 初始化左侧飞轮刹车信号
    encoder_quad_init(LEFT_FLYWHEEL_ENCODER_INDEX, LEFT_FLYWHEEL_ENCODER_CH1, LEFT_FLYWHEEL_ENCODER_CH2);   // 初始化左侧飞轮编码器接口

    pwm_init (RIGHT_FLYWHEEL_PWM, 17000, RIGHT_FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                // 初始化右侧飞轮PWM信号
    gpio_init(RIGHT_FLYWHEEL_DIR, GPO, RIGHT_FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                            // 初始化右侧飞轮DIR信号
    gpio_init(RIGHT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                 // 初始化右侧飞轮刹车信号
    encoder_quad_init(RIGHT_FLYWHEEL_ENCODER_INDEX, RIGHT_FLYWHEEL_ENCODER_CH1, RIGHT_FLYWHEEL_ENCODER_CH2);// 初始化右侧飞轮编码器接口


    InitIMU();
    pit_ms_init(CCU60_CH0, SAMPLING_PERIOD);

    // 等待陀螺仪数据稳定
    system_delay_ms(500);

    // 陀螺仪取值平滑处理
    for(int8 i = 0; i < countNum; i++)
    {
        gyroYArray_avge[10] += gyroYArray_avge[i];
        accYArray_avge[10] += accYArray_avge[i];
        gyroZArray_avge[10] += gyroZArray_avge[i];
        accZArray_avge[10] += accZArray_avge[i];
    }
    gyroYArray_avge[10] /= countNum;
    accYArray_avge[10] /= countNum;
    gyroZArray_avge[10] /= countNum;
    accZArray_avge[10] /= countNum;

    gyroYArray_avge[10] -= GYROY_BIAS;
    gyroZArray_avge[10] -= GYROZ_BIAS;

    // 加速的和角速度数值变化
    G2A_y = (float)(asinf((float)(func_limit_ab(gyroYArray_avge[10] / HHHT_Gravity, -1.5, 1.5))) / 2 / PI * 360);
    G2A_z = (float)(asinf((float)(func_limit_ab(gyroZArray_avge[10] / HHHT_Gravity, -1.5, 1.5))) / 2 / PI * 360);

}

void InitIMU ( void )
{
    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");                                 // IMU660RA 初始化失败
       else
           break;
       gpio_toggle_level(LED1);                                                 // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
    }
}
