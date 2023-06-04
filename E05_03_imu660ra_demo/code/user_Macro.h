
/**********************************************************************************************************************
 * \file    user_Macro.h
 * \brief
 * \version V1.0.0
 * \date    2023��5��19��
 * \author  Innovator
 *********************************************************************************************************************/


#ifndef CODE_USER_MACRO_H_
#define CODE_USER_MACRO_H_

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define LED1                    (P20_9)
#define LED2                    (P20_8)

// PWM ��ˢ�������
#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // ������PWM��������
#define LEFT_FLYWHEEL_DUTY_LEVEL        (0                      )     // ������ռ�ձ���Ч��ƽ
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // �����ַ����������
#define LEFT_FLYWHEEL_CLOCKWISE         (1                      )     // ������˳ʱ����ת����
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // ������ɲ����������
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // �����ֱ��������
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // �����ֱ�����ͨ��1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // �����ֱ�����ͨ��2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // �Ҳ����PWM��������
#define RIGHT_FLYWHEEL_DUTY_LEVEL       (0                      )     // �Ҳ����ռ�ձ���Ч��ƽ
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // �Ҳ���ַ����������
#define RIGHT_FLYWHEEL_CLOCKWISE        (1                      )     // �Ҳ����˳ʱ����ת����
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // �Ҳ����ɲ����������
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // �Ҳ���ֱ��������
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // �Ҳ���ֱ�����ͨ��1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // �Ҳ���ֱ�����ͨ��2

// PWM ��ˢ�������
#define MAX_DUTY                        (20 )                        // ��� MAX_DUTY% ռ�ձ�
#define POWER_MOTOR_DIR                 (P21_5)
#define POWER_MOTOR_PWM                 (ATOM0_CH2_P21_4)
#define ENCODER_QUADDEC                 (TIM4_ENCODER)               // ������������Ӧʹ�õı������ӿ�
#define ENCODER_QUADDEC_A               (TIM4_ENCODER_CH1_P02_8)     // A ���Ӧ������
#define ENCODER_QUADDEC_B               (TIM4_ENCODER_CH2_P00_9)     // B ���Ӧ������

// ��������
#define SAMPLING_PERIOD                 5                            // ms
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/


#endif /* CODE_USER_MACRO_H_ */
