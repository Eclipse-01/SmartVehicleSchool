#ifndef __HEADFILE_H_
#define __HEADFILE_H_




#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "intrins.h"
#include <math.h>
//------STC32G SDK等
#include "STC32Gxx.h"
#include "board.h"
#include "common.h"

//------逐飞科技单片机外设驱动头文件
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

//------逐飞科技产品驱动头文件
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
#include "SEEKFREE_BLE6A20.h"

//------逐飞科技组件层头文件
#include "seekfree_assistant.h"


//------用户头文件
#include "servo.h" //舵机
#include "drv8701_dual.h" //电机驱动
#include "battery.h" //电池


//------小车机械状态
typedef enum {          //电池状态
    FULL = 0,//100%
    NEARLY_FULL = 1,//>90%
    HIGH = 2,//>70%
    HALF = 3,//>50%
    LOW = 4,//>30%
    NEARLY_EMPTY = 5,//>10%
    EMPTY = 6//<10%
} Battery_Level;

typedef enum {          //充电状态
    CHARGING = 0,
    DISCHARGING = 1
} Charge_Status;

extern uint16 battery_voltage; //电池电压
extern uint16 battery_percentage; //电池电量百分比
extern uint16 servo_position; //舵机位置

//参数
#define Servo_MaxAngle 30
#define Servo_Center 1.5

#define motor_max_duty 60 //电机最大占空比

#define LOW_BATTERY_VOLTAGE 3200 // 触发低电压保护的最低电压，单位为mV
#define BATTERY_GROUP_NUM 3     // 电池组数量，代表有几个电池串联
#define ENABLE_LOW_BATTERY_PROTECTION 1 // 是否启用低电压保护，1为启用，0为禁用


#endif