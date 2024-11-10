#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "headfile.h"

// 直行入口函数
uint8 straight_entrance(void);

// PID控制函数
void PID_control(void);

// 异常处理函数
void exception_handler(int exception_code);

// 计算线的位置
int Line_calculate_position(void);

#endif // STRAIGHT_H
