/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留???飞科技的版权声明。
 *
 * @file       		main
 * @company		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#include "servo.h"

/*
 * 关于内核频率的设定，可以查看board.h文件
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

int8 servo_position = 0;  // 舵机位置

// 定义Kp和Kd参数
float servo_Kp = 1.0f;
float servo_Kd = 0.1f;

// 定义前一次的误差
static int16 previous_error = 0;

/**
 *@brief  舵机初始化
 *@note   要调节初始化的位置，可以修改Center的值
 */
void servo_init(void)
{
    uint32 duty = Servo_Center; // 初始化为中间位置，宏定义了占空比
    pwm_init(PWMB_CH3_P33, 50, duty);            // PWM频率是50Hz，即20ms。
}

/**
 * @brief 将舵机设置到指定角度。
 *
 * 此函数接受一个以度为单位的角度，并将舵机调整到相应的位置。
 * 角度应在舵机支持的有效范围内，见MaxAngle的宏定义。
 *
 * @param angle 要设置的舵机角度，以占空比为单位。
 */
void servo_set_position(int16 angle)
{
    uint32 pulseWide = Servo_Center;
    if (angle > Servo_MaxAngle)
    {
        angle = Servo_MaxAngle;
    }
    if (angle < -Servo_MaxAngle)
    {
        angle = -Servo_MaxAngle;
    }
    pulseWide = (Servo_Center + angle);
    pwm_duty(PWMB_CH3_P33, pulseWide);
    servo_position = angle;
}



