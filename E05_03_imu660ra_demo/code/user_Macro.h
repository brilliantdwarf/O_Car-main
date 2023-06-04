
/**********************************************************************************************************************
 * \file    user_Macro.h
 * \brief
 * \version V1.0.0
 * \date    2023年5月19日
 * \author  Innovator
 *********************************************************************************************************************/


#ifndef CODE_USER_MACRO_H_
#define CODE_USER_MACRO_H_

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define LED1                    (P20_9)
#define LED2                    (P20_8)

// PWM 无刷电机部分
#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // 左侧飞轮PWM控制引脚
#define LEFT_FLYWHEEL_DUTY_LEVEL        (0                      )     // 左侧飞轮占空比有效电平
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // 左侧飞轮方向控制引脚
#define LEFT_FLYWHEEL_CLOCKWISE         (1                      )     // 左侧飞轮顺时针旋转方向
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // 左侧飞轮刹车控制引脚
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // 左侧飞轮编码器编号
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // 左侧飞轮编码器通道1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // 左侧飞轮编码器通道2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // 右侧飞轮PWM控制引脚
#define RIGHT_FLYWHEEL_DUTY_LEVEL       (0                      )     // 右侧飞轮占空比有效电平
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // 右侧飞轮方向控制引脚
#define RIGHT_FLYWHEEL_CLOCKWISE        (1                      )     // 右侧飞轮顺时针旋转方向
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // 右侧飞轮刹车控制引脚
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // 右侧飞轮编码器编号
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // 右侧飞轮编码器通道1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // 右侧飞轮编码器通道2

// PWM 有刷电机部分
#define MAX_DUTY                        (20 )                        // 最大 MAX_DUTY% 占空比
#define POWER_MOTOR_DIR                 (P21_5)
#define POWER_MOTOR_PWM                 (ATOM0_CH2_P21_4)
#define ENCODER_QUADDEC                 (TIM4_ENCODER)               // 正交编码器对应使用的编码器接口
#define ENCODER_QUADDEC_A               (TIM4_ENCODER_CH1_P02_8)     // A 相对应的引脚
#define ENCODER_QUADDEC_B               (TIM4_ENCODER_CH2_P00_9)     // B 相对应的引脚

// 采样周期
#define SAMPLING_PERIOD                 5                            // ms
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/


#endif /* CODE_USER_MACRO_H_ */
