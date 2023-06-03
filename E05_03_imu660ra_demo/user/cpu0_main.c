/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 英飞凌TriCore 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 P14_0
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 P14_1
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源
// 接入 MPU6050
//      模块管脚            单片机管脚
//      SCL/SPC             查看 zf_device_imu660ra.h 中 IMU660RA_SPC_PIN 宏定义 默认 P20_11
//      SDA/DSI             查看 zf_device_imu660ra.h 中 IMU660RA_SDI_PIN 宏定义 默认 P20_14
//      SA0/SDO             查看 zf_device_imu660ra.h 中 IMU660RA_SDO_PIN 宏定义 默认 P20_12
//      CS                  查看 zf_device_imu660ra.h 中 IMU660RA_CS_PIN  宏定义 默认 P20_13
//      GND                 电源地 GND
//      3V3                 电源 3V3



// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，并连接好编码器，在断电情况下完成连接
// 2.将调试下载器或者 USB-TTL 模块连接电脑 完成上电 正常 H2 LED 会闪烁
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 4.可以在串口助手上看到如下串口信息：
//      IMU660RA acc data: x-..., y-..., z-...
//      IMU660RA gyro data: x-..., y-..., z-...
// 5.移动旋转 IMU660RA 就会看到数值变化
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************

float realGYRO_x    = 0, realGYRO_y     = 0, realGYRO_z     = 0;
float realACC_x     = 0, realACC_y      = 0, realACC_z      = 0;

int core0_main(void)
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

//    static int16 maxAcc_y = 0;
//    static int16 err_acc_x = 0;
//    static int16 err_gyro_x = 0;


    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");                                 // IMU660RA 初始化失败
       else
           break;
       gpio_toggle_level(LED1);                                                 // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
    }
    pit_ms_init(CCU60_CH0, 5);

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        balance();

        gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭
        system_delay_ms(10);

        // 此处编写需要循环执行的代码
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    imu660ra_get_acc();                                                         // 获取 IMU660RA 的加速度测量数值
    imu660ra_get_gyro();                                                        // 获取 IMU660RA 的角速度测量数值
    // 加速的和角速度数值变化


    realGYRO_x = imu660ra_gyro_transition(imu660ra_gyro_x);
    realGYRO_y = imu660ra_gyro_transition(imu660ra_gyro_y);
    realGYRO_z = imu660ra_gyro_transition(imu660ra_gyro_z);

    realACC_x = imu660ra_acc_transition(imu660ra_acc_x);
    realACC_y = imu660ra_acc_transition(imu660ra_acc_y);
    realACC_z = imu660ra_acc_transition(imu660ra_acc_z);

    gyroXArray_avge[gyroXArray_avgeIndex++] = realGYRO_x;
    gyroXArray_avgeIndex %= 10;
    accXArray_avge[accXArray_avgeIndex++] = realACC_x;
    accXArray_avgeIndex %= 10;
    gyroYArray_avge[gyroYArray_avgeIndex++] = realGYRO_y;
    gyroYArray_avgeIndex %= 10;
    accYArray_avge[accYArray_avgeIndex++] = realACC_y;
    accYArray_avgeIndex %= 10;
    gyroZArray_avge[gyroZArray_avgeIndex++] = realGYRO_z;
    gyroZArray_avgeIndex %= 10;
    accZArray_avge[accZArray_avgeIndex++] = realACC_z;
    accZArray_avgeIndex %= 10;

    real_Speed_Lef  = encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);               // 获取左侧编码器值
    encoder_clear_count(LEFT_FLYWHEEL_ENCODER_INDEX);                               // 清除编码器计数 方便下次采集
    real_Speed_Rig = encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);               // 获取右侧编码器值
    encoder_clear_count(RIGHT_FLYWHEEL_ENCODER_INDEX);                              // 清除编码器计数 方便下次采集
    real_Speed_MAIN = encoder_get_count(ENCODER_QUADDEC);                           // 获取驱动电机编码器计数
    encoder_clear_count(ENCODER_QUADDEC);                                           // 清空编码器计数

    SpeedLef_avge[SpeedLef_avgeIndex++] = real_Speed_Lef;
    SpeedLef_avgeIndex %= 5;
    SpeedRig_avge[SpeedRig_avgeIndex++] = real_Speed_Rig;
    SpeedRig_avgeIndex %= 5;
}

#pragma section all restore
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 英飞凌TriCore 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
// 问题2：串口数据乱码
//      查看串口助手设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
// 问题3：串口输出 imu660ra init error.或者imu660ra self check error.
//      检查 IMU660RA 的接线是否正确
//      检查 IMU660RA 的模块是不是坏了
//      给信号线加上拉看看
// 问题4：IMU660RA 数值异常
//      看看是不是线松了 或者信号线被短路了
//      可能模块部分受损

