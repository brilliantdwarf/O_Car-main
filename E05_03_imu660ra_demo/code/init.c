/*
 * init.c
 *
 *  Created on: 2023��6��3��
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
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ

    // PWM ��ˢ�����ʼ��
    gpio_init(POWER_MOTOR_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                  // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init (POWER_MOTOR_PWM, 17000, 0);                                       // PWM ͨ����ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    encoder_quad_init(ENCODER_QUADDEC, ENCODER_QUADDEC_A, ENCODER_QUADDEC_B);   // ��ʼ��������ģ�������� �������������ģʽ

    // PWM ��ˢ�����ʼ��
    pwm_init (LEFT_FLYWHEEL_PWM, 17000, LEFT_FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                  // ��ʼ��������PWM�ź�
    gpio_init(LEFT_FLYWHEEL_DIR, GPO, LEFT_FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                              // ��ʼ��������DIR�ź�
    gpio_init(LEFT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                  // ��ʼ��������ɲ���ź�
    encoder_quad_init(LEFT_FLYWHEEL_ENCODER_INDEX, LEFT_FLYWHEEL_ENCODER_CH1, LEFT_FLYWHEEL_ENCODER_CH2);   // ��ʼ�������ֱ������ӿ�

    pwm_init (RIGHT_FLYWHEEL_PWM, 17000, RIGHT_FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                // ��ʼ���Ҳ����PWM�ź�
    gpio_init(RIGHT_FLYWHEEL_DIR, GPO, RIGHT_FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                            // ��ʼ���Ҳ����DIR�ź�
    gpio_init(RIGHT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                 // ��ʼ���Ҳ����ɲ���ź�
    encoder_quad_init(RIGHT_FLYWHEEL_ENCODER_INDEX, RIGHT_FLYWHEEL_ENCODER_CH1, RIGHT_FLYWHEEL_ENCODER_CH2);// ��ʼ���Ҳ���ֱ������ӿ�


    InitIMU();
    pit_ms_init(CCU60_CH0, SAMPLING_PERIOD);

    // �ȴ������������ȶ�
    system_delay_ms(500);

    // ������ȡֵƽ������
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

    // ���ٵĺͽ��ٶ���ֵ�仯
    G2A_y = (float)(asinf((float)(func_limit_ab(gyroYArray_avge[10] / HHHT_Gravity, -1.5, 1.5))) / 2 / PI * 360);
    G2A_z = (float)(asinf((float)(func_limit_ab(gyroZArray_avge[10] / HHHT_Gravity, -1.5, 1.5))) / 2 / PI * 360);

}

void InitIMU ( void )
{
    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");                                 // IMU660RA ��ʼ��ʧ��
       else
           break;
       gpio_toggle_level(LED1);                                                 // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
    }
}
