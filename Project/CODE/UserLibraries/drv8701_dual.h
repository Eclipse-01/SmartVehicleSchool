/**************************************************************************
*COPYRIGHT NOTICE
*JNU-FLY 2024
*All rights reserved
*Modified from SEEKFREE libs
*
*@file drv8701_dual.h
*@author JNU-FLY
*@version 1.0
*@Software MDK FOR C251 V5.60
*@Target core STC32F12K
*@Website www.flysworld.top
*/
#ifndef _DRV8701_DUAL_H_
#define _DRV8701_DUAL_H_

#include "headfile.h"

void drv8701_init(void);
void drv8701_control(Motor_Select motor_select, int speed);
void drv8701_stop(Motor_Select motor_select);

#endif // _DRV8701_DUAL_H_