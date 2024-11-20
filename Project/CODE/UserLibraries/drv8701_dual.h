/**************************************************************************
 * drv8701_dual.h
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件定义了DRV8701双电机驱动的初始化和控制接口。
 */
#ifndef _DRV8701_DUAL_H_
#define _DRV8701_DUAL_H_

#include "headfile.h"

typedef enum {
    MOTOR_L = 0,
    MOTOR_R = 1,
    MOTOR_BOTH = 2
} Motor_Select;

void drv8701_init(void);
void drv8701_control(Motor_Select motor_select, int32 speed);
void drv8701_stop(Motor_Select motor_select);

#endif // _DRV8701_DUAL_H_