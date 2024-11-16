#ifndef __HEADFILE_H_
#define __HEADFILE_H_




#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "intrins.h"
#include <math.h>
//------STC32G SDK��
#include "STC32Gxx.h"
#include "board.h"
#include "common.h"

//------��ɿƼ���Ƭ����������ͷ�ļ�
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_iic.h"
#include "zf_adc.h"
#include "zf_spi.h"
#include "zf_tim.h"
#include "zf_pwm.h"
#include "zf_nvic.h"
#include "zf_exti.h"
#include "zf_delay.h"
#include "zf_eeprom.h"
#include "zf_fifo.h"

//------��ɿƼ���Ʒ����ͷ�ļ�
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_FUNCTION.h"
#include "SEEKFREE_AT24C02.h"
#include "SEEKFREE_BLUETOOTH_CH9141.h"
#include "SEEKFREE_CONFIG.h"
#include "SEEKFREE_IMU660RA.h"
#include "SEEKFREE_IMU963RA.h"
#include "SEEKFREE_DL1A.h"
#include "SEEKFREE_DL1B.h"
#include "SEEKFREE_IPS200_SPI.h"

//------��ɿƼ������ͷ�ļ�
#include "seekfree_assistant.h"


//------�û�ͷ�ļ�
#include "servo.h" //���
#include "drv8701_dual.h" //�������
#include "battery.h" //���
#include "encoder.h" //������
#include "line.h" //Ѱ��
#include "straight.h" //ֱ��
#include "ring.h" //ת��

//����
#define Servo_MaxAngle 50
#define Servo_Center 585

#define motor_max_duty 60 //������ռ�ձȣ���Ӧ�ô���60

#define LOW_BATTERY_VOLTAGE 3200 // �����͵�ѹ��������͵�ѹ����λΪmV
#define BATTERY_GROUP_NUM 3     // ����������������м�����ش���
#define ENABLE_LOW_BATTERY_PROTECTION 1 // �Ƿ����õ͵�ѹ������1Ϊ���ã�0Ϊ����
#define encoder_swap //������ұ������ӷ���ȡ��ע�ʹ���
#define encoder_time 10 //��������ȡʱ��������λms

#endif