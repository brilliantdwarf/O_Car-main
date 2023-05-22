/*
 * balance.c
 *
 *  Created on: 2023年5月19日
 *      Author: Innovator
 */

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "balance.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/

float duty_outAve[11] = {0};
float duty_outAveIndex = 0;
float gyroXArray_avge[11] = {0};
int16 gyroXArray_avgeIndex = 0;
float accXArray_avge[11] = {0};
int16 accXArray_avgeIndex = 0;
float gyroYArray_avge[11] = {0};
int16 gyroYArray_avgeIndex = 0;
float accYArray_avge[11] = {0};
int16 accYArray_avgeIndex = 0;
float gyroZArray_avge[11] = {0};
int16 gyroZArray_avgeIndex = 0;
float accZArray_avge[11] = {0};
int16 accZArray_avgeIndex = 0;
float SpeedLef_avge[6] = {0};
int16 SpeedLef_avgeIndex = 0;
float SpeedRig_avge[6] = {0};
int16 SpeedRig_avgeIndex = 0;

float angleX_set = 0;
float angleY_set = 0;
float angleZ_set = 0;
float angleSpeedX_set = 0;
float angleSpeedY_set = 0;
float angleSpeedZ_set = 0;
float Speed_MAIN_set = 0;
float Speed_Lef_set = 0;
float Speed_Rig_set = 0;

int16 real_Speed_Lef = 0;                                 // 左侧飞轮编码器值
int16 real_Speed_Rig = 0;                                 // 右侧飞轮编码器值
int16 real_Speed_Turn = 0;
int16 real_Speed_MAIN = 0;

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

static float err_acc_y = 0;
static float err_gyro_y = 0;
static float err_acc_z = 0;
static float err_gyro_z = 0;
static float sum_Speed_Lef = 0;
static float sum_Speed_Rig = 0;
static float sum_Speed_Turn = 0;
static float sum_Speed_MAIN = 0;
static float angle_x = 0;
static float angle_y = 0;
static float angle_z = 0;
static float duty_out_acc = 0;
static float duty_out_gyro = 0;
static float duty_out_turn = 0;
static float duty_out = 0;
static float duty_out_Lef = 0;
static float duty_out_Rig = 0;
static float aSpeed_out_BALANCE_gyro = 0;
static float aSpeed_out_BALANCE_acc = 0;
static float aSpeed_out_BALANCE = 0;
static float duty_out_MAIN_gyro = 0;
static float duty_out_MAIN_acc = 0;
static float duty_out_MAIN = 0;
static float lastErr_acc = 0;
static float lastErr_gyro = 0;
static float lastErr_M_acc = 0;
static float lastErr_M_gyro = 0;
static float lastErr_Turn = 0;
static float integError_z = 0;
static float integError_y = 0;
static float integError_x = 0;
int8 dir = 1;

static int8 numTest[10] = {0x03, 0xFC, 0, 0, 0, 0, 0, 0, 0xFC, 0x03};

/*********************************************************************************************************************/
/*--------------------------------------------------Function Content-------------------------------------------------*/
/*********************************************************************************************************************/

void balance( void )
{

    // 陀螺仪取值平滑处理
    for(int8 i = 0; i < 10; i++)
    {
        gyroXArray_avge[10] += gyroXArray_avge[i];
        accXArray_avge[10] += accXArray_avge[i];
        gyroYArray_avge[10] += gyroYArray_avge[i];
        accYArray_avge[10] += accYArray_avge[i];
        gyroZArray_avge[10] += gyroZArray_avge[i];
        accZArray_avge[10] += accZArray_avge[i];
    }
    gyroXArray_avge[10] /= 10;
    accXArray_avge[10] /= 10;
    gyroYArray_avge[10] /= 10;
    accYArray_avge[10] /= 10;
    gyroZArray_avge[10] /= 10;
    accZArray_avge[10] /= 10;

    for(int8 i = 0; i < 5; i++)
    {
        SpeedLef_avge[5] += SpeedLef_avge[i];
        SpeedRig_avge[5] += SpeedRig_avge[i];
    }
    SpeedLef_avge[5] /= 5;
    SpeedRig_avge[5] /= 5;
    // 根据重力加速度解算 Y Z 偏航角，以竖直状态为0
    angle_x = (float)(asinf((float)(func_limit_ab(accXArray_avge[10] / 0.98, -1.5, 1.5))) / 2 / PI * 360);
    angle_y = (float)(asinf((float)(func_limit_ab(accYArray_avge[10] / 0.98, -1.5, 1.5))) / 2 / PI * 360);
    angle_z = (float)(asinf((float)(func_limit_ab(accZArray_avge[10] / 0.98, -1.5, 1.5))) / 2 / PI * 360);


    // angle speed -----------------------------------------内环------------------------------------------------- //
#if     aS2out
    // pitch 轴 —— Y 轴 角速度环
    if(func_abs((int)(10 * (angleSpeedY_set - gyroYArray_avge[10]))) > 8)
    {
       err_gyro_y = 10 * (angleSpeedY_set - gyroYArray_avge[10]);
    }
    else
    {
//           duty_out_gyro *= 0.5;
       err_gyro_y = 0;
    }
    duty_out_gyro = err_gyro_y * 30;//28;
    duty_out_gyro += (err_gyro_y - lastErr_gyro) * 2;                   // 正负决定瞬时对于kp是补偿还是削弱
    lastErr_gyro = err_gyro_y;
    duty_out_gyro = func_limit_ab(duty_out_gyro, -8500, 8500);

    // roll 轴 —— Z 轴 角速度环
    if(func_abs((int)(10 * (angleSpeedZ_set - gyroZArray_avge[10]))) > 8)
    {
       err_gyro_z = 10 * (angleSpeedZ_set - gyroZArray_avge[10]);
    }
    else
    {
//       duty_out_MAIN_gyro *= 0.4;
       err_gyro_z = 0;
    }
    duty_out_MAIN_gyro = err_gyro_z * 1.3;
    duty_out_MAIN_gyro += (err_gyro_z - lastErr_M_gyro) * 0.7;
    lastErr_M_gyro = err_gyro_z;

    duty_out_MAIN_gyro = func_limit_ab(duty_out_MAIN_gyro, -2500, 2500);

//        // yaw 轴 —— X 轴 角速度环
//        if(func_abs((int)(10 * (angleSpeedX_set - gyroXArray_avge[10]))) > 8)
//        {
//            err_gyro_x = 10 * (angleSpeedX_set - gyroXArray_avge[10]);
//        }
//        else
//        {
//           err_gyro_x = 0;
//        }
//        duty_out_turn = err_gyro_x * 1;
//        duty_out_turn += (err_gyro_x - lastErr_Turn) * 1.5;
//
//        lastErr_Turn = err_gyro_x;
//
//        duty_out_turn = func_limit_ab(duty_out_turn, -1000, 1000);
#else
#endif


    // angle to angle speed------------------------------------外环------------------------------------------------ //
#if     angle2aSpeed
    // Z 方向偏航角 用 Y 方向角速度弥补 角度环
//        angle_z = 0;
    if(func_abs(10 * (angleZ_set - 6 * angle_z)) > 5 * 6)
    {
       err_acc_z = angleZ_set - angle_z;
    }
    else
    {
//           duty_out_acc *= 0.8;
       err_acc_z = 0;
    }
    if(func_abs(angle_z) > 20)
    {
        duty_out = 0;
        goto Dumping_protection;
    }
    aSpeed_out_BALANCE_acc = err_acc_z * 2;//2.5;// 3
    aSpeed_out_BALANCE_acc += (err_acc_z - lastErr_acc) * 1;

//    if(func_abs(10 * err_acc_z) < 55)
//    {
       integError_z += err_acc_z * 0.12;
       aSpeed_out_BALANCE_acc += integError_z;
//    }
//    else
//       integError_z *= 0.9;

    integError_z = func_limit_ab(integError_z, -12, 12);
    lastErr_acc = err_acc_z;

    aSpeed_out_BALANCE_acc = func_limit_ab(aSpeed_out_BALANCE_acc, -300 * 2, 300 * 2);

    angleSpeedY_set = - aSpeed_out_BALANCE_acc;

    // Y 方向偏航角 用 Z 方向角速度弥补 角度环
    if(func_abs(10 * (angleY_set - angle_y)) > 2)
    {
       err_acc_y = angleY_set - angle_y;
    }
    else
    {
//           aSpeed_out_BALANCE_acc *= 0.8;
       err_acc_y = 0;
    }
    if(func_abs(angle_y) > 30)
    {
        duty_out = 0;
        duty_out_MAIN = 0;
        goto Dumping_protection;
    }
    duty_out_MAIN_acc = err_acc_y * 2;// 5
//        duty_out_MAIN_acc -= (err_acc_y - lastErr_M_acc) * 0.5;
//    if(func_abs(10 * err_acc_y) < 55)
//    {
    duty_out_MAIN_acc = 0;
           integError_y += err_acc_y * -0.8;
           duty_out_MAIN_acc += integError_y;
//    }
//    else
       integError_y *= 0.9;

    integError_y = func_limit_ab(integError_y, -13, 13);
    lastErr_M_acc = err_acc_y;
    duty_out_MAIN_acc = func_limit_ab(duty_out_MAIN_acc, -200, 200);
    angleSpeedZ_set = duty_out_MAIN_acc;
#else
#endif


    // speed to angle ---------------------------------------外外环------------------------------------------------ //
#if     Speed2angle
    sum_Speed_Lef = (Speed_Lef_set - SpeedLef_avge[5]) * func_abs(angle_z) * 0.04;//0.15;
    sum_Speed_Rig = (Speed_Rig_set + SpeedRig_avge[5]) * func_abs(angle_z) * 0.04;//0.15;
    sum_Speed_Turn = (sum_Speed_Lef + sum_Speed_Rig) / 2;
    sum_Speed_Turn = func_limit_ab(sum_Speed_Turn, -30, 30);
//        sum_Speed_MAIN = (Speed_MAIN_set + real_Speed_MAIN) * 1;
//        duty_out_gyro -= sum_Speed_Turn;

#else
#endif
    angleZ_set = + sum_Speed_Turn;


    // 输出 和 限幅
//        duty_out += (duty_out_gyro - aSpeed_out_BALANCE_acc);
//        duty_out += duty_out_gyro;
    duty_out = func_limit_ab(duty_out_gyro, -8500, 8500);

//        duty_out_MAIN = - duty_out_MAIN_acc - duty_out_MAIN_gyro;
    duty_out_MAIN = - duty_out_MAIN_gyro;
    duty_out_MAIN = func_limit_ab(duty_out_MAIN, -2500, 2500);
    Dumping_protection:
    duty_out_Lef = duty_out + duty_out_turn;
    duty_out_Rig = duty_out - duty_out_turn;

#if     !TURN_ENABLE
    duty_out_Lef = 0;
    duty_out_Rig = 0;
#endif

#if     !MAIN_ENABLE
    duty_out_MAIN = 0;
#endif

    // PWM 输出
    if(duty_out_Lef >= 0)
    {
        gpio_set_level(LEFT_FLYWHEEL_DIR, !LEFT_FLYWHEEL_CLOCKWISE);
        pwm_set_duty(LEFT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out_Lef));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
    }
    else
    {
        gpio_set_level(LEFT_FLYWHEEL_DIR, LEFT_FLYWHEEL_CLOCKWISE);
        pwm_set_duty(LEFT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out_Lef));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
    }
    if(duty_out_Rig >= 0)
    {
        gpio_set_level(RIGHT_FLYWHEEL_DIR, RIGHT_FLYWHEEL_CLOCKWISE);
        pwm_set_duty(RIGHT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out_Rig));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
    }
    else
    {
        gpio_set_level(RIGHT_FLYWHEEL_DIR, !RIGHT_FLYWHEEL_CLOCKWISE);
        pwm_set_duty(RIGHT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out_Rig));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
    }

//        if(duty_out >= 0)
//        {
//            gpio_set_level(LEFT_FLYWHEEL_DIR, !LEFT_FLYWHEEL_CLOCKWISE);
//            gpio_set_level(RIGHT_FLYWHEEL_DIR, RIGHT_FLYWHEEL_CLOCKWISE);
//            pwm_set_duty(LEFT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
//            pwm_set_duty(RIGHT_FLYWHEEL_PWM, PWM_DUTY_MAX - func_abs(duty_out));
//        }
//        else
//        {
//            gpio_set_level(LEFT_FLYWHEEL_DIR, LEFT_FLYWHEEL_CLOCKWISE);
//            gpio_set_level(RIGHT_FLYWHEEL_DIR, !RIGHT_FLYWHEEL_CLOCKWISE);
//            pwm_set_duty(LEFT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(duty_out));  // 输出占空比 占空比必须为正值 因此此处使用绝对值
//            pwm_set_duty(RIGHT_FLYWHEEL_PWM, PWM_DUTY_MAX - func_abs(duty_out));
//        }
    if(duty_out_MAIN >= 0)                                                  // 正转
    {
        gpio_set_level(POWER_MOTOR_DIR, GPIO_HIGH);                                     // DIR输出高电平
        pwm_set_duty(POWER_MOTOR_PWM, func_abs(duty_out_MAIN) + 200);                         // 计算占空比
    }
    else                                                                    // 反转
    {
        gpio_set_level(POWER_MOTOR_DIR, GPIO_LOW);                                      // DIR输出低电平
        pwm_set_duty(POWER_MOTOR_PWM, func_abs(duty_out_MAIN) + 200);                         // 计算占空比
    }

#if     CURVE_OUT
    numTest[2] = - real_Speed_Lef;
    numTest[3] = sum_Speed_Rig;
    numTest[4] = duty_out * 100 / 8000;
    numTest[5] = duty_out_MAIN * 100 / 2000;
    numTest[6] = sum_Speed_Lef;
    numTest[7] = real_Speed_Rig;

//        printf("%c%c%c%c%c", 0x03, 0x0FC, numTest, 0x0FC, 0x03);
    uart_write_buffer(UART_0, numTest, 10);
//        uart_write_buffer(UART_2, numTest, 10);

#elif   CHARACTER_OUT

//        printf("\r\n org IMU660RA acc data:  x=%.2f, y=%.2f, z=%.2f\r\n", realGYRO_x,  realGYRO_y,  realGYRO_z);
//        printf("\r\n org IMU660RA gyro data: x=%.0f, y=%.0f, z=%.0f, duty=%d\r\n", 1000 * imu660ra_acc_transition(imu660ra_acc_x), 1000 * imu660ra_acc_transition(imu660ra_acc_y), 1000 * imu660ra_acc_transition(imu660ra_acc_z), duty_out);
    printf("\r\n  sum_Speed_Turn=%4.4f, z_s=%4.4f, Y_a=%4.4f, duty=%4.4f\r\n", sum_Speed_Turn, SpeedLef_avge[5], SpeedRig_avge[5], duty_out);
    printf("asin(alpha) = %2.4f, asin(beta) = %2.4f, asin(gamma) = %2.4f\r", angle_x, angle_y, angle_z);

//        printf("\r\n%5d\r\n", maxAcc_y);
//        printf("\r\n real IMU660RA acc data:  x=%5d, y=%5d, z=%5d\r\n", imu660ra_acc_x,  imu660ra_acc_y,  imu660ra_acc_z);
//        printf("\r\n real IMU660RA gyro data: x=%5d, y=%5d, z=%5d\r\n", imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z);

#endif
}

