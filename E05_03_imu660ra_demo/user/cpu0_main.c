/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "isr_config.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� interrupt_global_enable(0); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� interrupt_global_disable(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� interrupt_global_enable(0); �������жϵ���Ӧ��

// *************************** ����Ӳ������˵�� ***************************
// ʹ����ɿƼ� Ӣ����TriCore ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ
// ���� MPU6050
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL/SPC             �鿴 zf_device_imu660ra.h �� IMU660RA_SPC_PIN �궨�� Ĭ�� P20_11
//      SDA/DSI             �鿴 zf_device_imu660ra.h �� IMU660RA_SDI_PIN �궨�� Ĭ�� P20_14
//      SA0/SDO             �鿴 zf_device_imu660ra.h �� IMU660RA_SDO_PIN �궨�� Ĭ�� P20_12
//      CS                  �鿴 zf_device_imu660ra.h �� IMU660RA_CS_PIN  �궨�� Ĭ�� P20_13
//      GND                 ��Դ�� GND
//      3V3                 ��Դ 3V3



// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�����Ӻñ��������ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��� ����ϵ� ���� H2 LED ����˸
// 3.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.�����ڴ��������Ͽ������´�����Ϣ��
//      IMU660RA acc data: x-..., y-..., z-...
//      IMU660RA gyro data: x-..., y-..., z-...
// 5.�ƶ���ת IMU660RA �ͻῴ����ֵ�仯
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�


// **************************** �������� ****************************

float realGYRO_x    = 0, realGYRO_y     = 0, realGYRO_z     = 0;
float realACC_x     = 0, realACC_y      = 0, realACC_z      = 0;
float G2A_x         = 0, G2A_y          = 0, G2A_z          = 0;

int core0_main(void)
{
    InitAll();

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        balance();

        gpio_toggle_level(LED1);                                                // ��ת LED ���������ƽ ���� LED ����
        system_delay_ms(10);

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);

    imu660ra_get_acc();                                                         // ��ȡ IMU660RA �ļ��ٶȲ�����ֵ
    imu660ra_get_gyro();                                                        // ��ȡ IMU660RA �Ľ��ٶȲ�����ֵ
    // ���ٵĺͽ��ٶ���ֵ�仯


    realGYRO_x = imu660ra_gyro_transition(imu660ra_gyro_x);
    realGYRO_y = imu660ra_gyro_transition(imu660ra_gyro_y);
    realGYRO_z = imu660ra_gyro_transition(imu660ra_gyro_z);

    realACC_x = imu660ra_acc_transition(imu660ra_acc_x);
    realACC_y = imu660ra_acc_transition(imu660ra_acc_y);
    realACC_z = imu660ra_acc_transition(imu660ra_acc_z);

    gyroXArray_avge[gyroXArray_avgeIndex++] = realGYRO_x;
    gyroXArray_avgeIndex %= countNum;
    accXArray_avge[accXArray_avgeIndex++] = realACC_x;
    accXArray_avgeIndex %= countNum;
    gyroYArray_avge[gyroYArray_avgeIndex++] = realGYRO_y;
    gyroYArray_avgeIndex %= countNum;
    accYArray_avge[accYArray_avgeIndex++] = realACC_y;
    accYArray_avgeIndex %= countNum;
    gyroZArray_avge[gyroZArray_avgeIndex++] = realGYRO_z;
    gyroZArray_avgeIndex %= countNum;
    accZArray_avge[accZArray_avgeIndex++] = realACC_z;
    accZArray_avgeIndex %= countNum;

    real_Speed_Lef  = encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);               // ��ȡ��������ֵ
    encoder_clear_count(LEFT_FLYWHEEL_ENCODER_INDEX);                               // ������������� �����´βɼ�
    real_Speed_Rig = encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);               // ��ȡ�Ҳ������ֵ
    encoder_clear_count(RIGHT_FLYWHEEL_ENCODER_INDEX);                              // ������������� �����´βɼ�
    real_Speed_MAIN = encoder_get_count(ENCODER_QUADDEC);                           // ��ȡ�����������������
    encoder_clear_count(ENCODER_QUADDEC);                                           // ��ձ���������

    SpeedLef_avge[SpeedLef_avgeIndex++] = real_Speed_Lef;
    SpeedLef_avgeIndex %= 5;
    SpeedRig_avge[SpeedRig_avgeIndex++] = real_Speed_Rig;
    SpeedRig_avgeIndex %= 5;

    G2A_x -= (realGYRO_x - GYROX_BIAS) * SAMPLING_PERIOD / 1000 * 1;
    G2A_y -= (realGYRO_y - GYROY_BIAS) * SAMPLING_PERIOD / 1000 * 1;
    G2A_z += (realGYRO_z - GYROZ_BIAS) * SAMPLING_PERIOD / 1000 * 1;
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� Ӣ����TriCore �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
// ����3��������� imu660ra init error.����imu660ra self check error.
//      ��� IMU660RA �Ľ����Ƿ���ȷ
//      ��� IMU660RA ��ģ���ǲ��ǻ���
//      ���ź��߼���������
// ����4��IMU660RA ��ֵ�쳣
//      �����ǲ��������� �����ź��߱���·��
//      ����ģ�鲿������

