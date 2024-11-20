/*********************
 * servo.h
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件定义了舵机的初始化和位置设置功能。
 *
 * 许可协议详见 LICENSE 文件。
 */
#ifndef SERVO_H
#define SERVO_H

#include "zf_pwm.h"

extern int8 servo_position; // 舵机位置

void servo_init(void);
void servo_set_position(int16 position);

#endif // SERVO_H
