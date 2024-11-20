/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		IMU660RA
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
 * ���߶��壺
 *                   ------------------------------------
 *                   ģ��ܽ�            ��Ƭ���ܽ�
 *                   // Ӳ�� SPI ����
 *                   SCL/SPC           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SPC_PIN �궨��
 *                   SDA/DSI           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDI_PIN �궨��
 *                   SA0/SDO           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDO_PIN �궨��
 *                   CS                �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_CS_PIN �궨��
 *                   VCC               3.3V��Դ
 *                   GND               ��Դ��
 *                   ������������
 *
 *                   // ��� IIC ����
 *                   SCL/SPC           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SCL_PIN �궨��
 *                   SDA/DSI           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDA_PIN �궨��
 *                   VCC               3.3V��Դ
 *                   GND               ��Դ��
 *                   ������������
 *                   ------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_IMU660RA_h_
#define _SEEKFREE_IMU660RA_h_

#include "common.h"
#include "board.h"


#define IMU660RA_USE_SOFT_IIC       	(0)         // Ĭ��ʹ����� SPI ��ʽ����

#if IMU660RA_USE_SOFT_IIC                                         
//=====================================================��� IIC ����====================================================
	#define IMU660RA_SCL_PIN            (P40)     	// ��� IIC SCL ���� ���� IMU660RA �� SCL ����
	#define IMU660RA_SDA_PIN            (P41)      	// ��� IIC SDA ���� ���� IMU660RA �� SDA ����
	#define IMU660RA_SOFT_IIC_DELAY     (0 )   		// ��� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
//=====================================================��� IIC ����====================================================
#else
//=====================================================��� SPI ����====================================================
	#define IMU660RA_SPC_PIN            (P40)      	// ��� SPI SCK ����
	#define IMU660RA_SDI_PIN            (P41)      	// ��� SPI MOSI ����
	#define IMU660RA_SDO_PIN            (P42)      	// ��� SPI MISO ����
	#define IMU660RA_CS_PIN             (P43)      	// ��� SPI CS ����
//=====================================================��� SPI ����====================================================
#endif

               
#define IMU660RA_TIMEOUT_COUNT      (0x00FF)                                    // IMU660RA ��ʱ����

#define IMU660RA_DEV_ADDR           (0x69)                                      // SA0�ӵأ�0x68 SA0������0x69 ģ��Ĭ������
#define IMU660RA_SPI_W              (0x00)
#define IMU660RA_SPI_R              (0x80)

#define IMU660RA_CHIP_ID            (0x00)
#define IMU660RA_PWR_CONF           (0x7C)
#define IMU660RA_PWR_CTRL           (0x7D)
#define IMU660RA_INIT_CTRL          (0x59)
#define IMU660RA_INIT_DATA          (0x5E)
#define IMU660RA_INT_STA            (0x21)
#define IMU660RA_ACC_ADDRESS        (0x0C)
#define IMU660RA_GYRO_ADDRESS       (0x12)
#define IMU660RA_ACC_CONF           (0x40)
#define IMU660RA_ACC_RANGE          (0x41)
#define IMU660RA_GYR_CONF           (0x42)
#define IMU660RA_GYR_RANGE          (0x43)

#define IMU660RA_ACC_SAMPLE         (0x02)                                      // ���ٶȼ�����
// ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g         ��ȡ���ļ��ٶȼ����� ���� 16384   ����ת��Ϊ������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x01 ���ٶȼ�����Ϊ:��4g         ��ȡ���ļ��ٶȼ����� ���� 8192    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x02 ���ٶȼ�����Ϊ:��8g         ��ȡ���ļ��ٶȼ����� ���� 4096    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
// ����Ϊ:0x03 ���ٶȼ�����Ϊ:��16g        ��ȡ���ļ��ٶȼ����� ���� 2048    ����ת��Ϊ������λ������ ��λ��g(m/s^2)

#define IMU660RA_GYR_SAMPLE         (0x00)                                      // ����������
// ����Ϊ:0x00 ����������Ϊ:��2000dps     ��ȡ�������������� ���� 16.4       ����ת��Ϊ������λ������ ��λΪ����/s
// ����Ϊ:0x01 ����������Ϊ:��1000dps     ��ȡ�������������� ���� 32.8       ����ת��Ϊ������λ������ ��λΪ����/s
// ����Ϊ:0x02 ����������Ϊ:��500 dps     ��ȡ�������������� ���� 65.6       ����ת��Ϊ������λ������ ��λΪ����/s
// ����Ϊ:0x03 ����������Ϊ:��250 dps     ��ȡ�������������� ���� 131.2      ����ת��Ϊ������λ������ ��λΪ����/s
// ����Ϊ:0x04 ����������Ϊ:��125 dps     ��ȡ�������������� ���� 262.4      ����ת��Ϊ������λ������ ��λΪ����/s


extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;                 // ��������������      gyro (������)
extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;                    // ������ٶȼ�����     acc (accelerometer ���ٶȼ�)


void  imu660ra_get_acc              (void);                                     // ��ȡ IMU660RA ���ٶȼ�����
void  imu660ra_get_gyro             (void);                                     // ��ȡ IMU660RA ����������
float imu660ra_acc_transition       (int16 acc_value);                          // �� IMU660RA ���ٶȼ�����ת��Ϊʵ����������
float imu660ra_gyro_transition      (int16 gyro_value);                         // �� IMU660RA ����������ת��Ϊʵ����������
uint8 imu660ra_init                 (void);                                     // ��ʼ�� IMU660RA

#endif

