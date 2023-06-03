
/**********************************************************************************************************************
 * \file    balance.h
 * \brief
 * \version V1.0.0
 * \date    2023年5月19日
 * \author  Innovator
 *********************************************************************************************************************/


#ifndef CODE_BALANCE_H_
#define CODE_BALANCE_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "isr_config.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
// PID 环
#define acc2out                         1
#define aS2out                          1
#define angle2aSpeed                    1
#define Speed2angle                     1

// 有刷 无刷单独调试
#define TURN_ENABLE                     1
#define MAIN_ENABLE                     1

// 串口调试方式
#define CURVE_OUT                       1
#define CHARACTER_OUT                   0

// 飞轮刹车
#define LEF_STOP()    gpio_set_level(LEFT_FLYWHEEL_BRAKE, 0)
#define RIG_STOP()    gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 0)

// 飞轮刹车取消
#define LEF_START()    gpio_set_level(LEFT_FLYWHEEL_BRAKE, 1)
#define RIG_START()    gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 1)
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
extern float duty_outAve[11];
extern float duty_outAveIndex;
extern float gyroXArray_avge[11];
extern int16 gyroXArray_avgeIndex;
extern float accXArray_avge[11];
extern int16 accXArray_avgeIndex;
extern float gyroYArray_avge[11];
extern int16 gyroYArray_avgeIndex;
extern float accYArray_avge[11];
extern int16 accYArray_avgeIndex;
extern float gyroZArray_avge[11];
extern int16 gyroZArray_avgeIndex;
extern float accZArray_avge[11];
extern int16 accZArray_avgeIndex;
extern float SpeedLef_avge[6];
extern int16 SpeedLef_avgeIndex;
extern float SpeedRig_avge[6];
extern int16 SpeedRig_avgeIndex;

extern float anaccX_set;
extern float anaccY_set;
extern float anaccZ_set;
extern float angleX_set;
extern float angleY_set;
extern float angleZ_set;
extern float angleSpeedX_set;
extern float angleSpeedY_set;
extern float angleSpeedZ_set;
extern float Speed_MAIN_set;
extern float Speed_Lef_set;
extern float Speed_Rig_set;

extern int16 real_Speed_Lef;                                 // 左侧飞轮编码器值
extern int16 real_Speed_Rig;                                 // 右侧飞轮编码器值
extern int16 real_Speed_Turn;
extern int16 real_Speed_MAIN;
/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
 
/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
static void dataAveraging       ( void );
static void debugOutput         ( void );
       void balance             ( void );
       void balance2            ( void );
       void balance3            ( void );
       void balance4            ( void );

#endif /* CODE_BALANCE_H_ */
