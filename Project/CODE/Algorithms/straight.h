#ifndef STRAIGHT_H
#define STRAIGHT_H

/*********************
 * straight.h
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件定义了直行算法的入口、PID控制及异常处理接口。
 */

#include "headfile.h"

// 直行入口函数
uint8 straight_entrance(void);

// PID控制函数
void PID_control_straint(void);
void PID_control_ring(void);

// 异常处理函数
void exception_handler(int exception_code);

// 计算线的位置
int Line_calculate_position(void);
int Line_calculate_position_ring(void);

extern uint8 RING_FLAG; // 声明外部变量

#endif // STRAIGHT_H
