#ifndef BATTERY_H
#define BATTERY_H

/*********************
 * battery.h
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件定义了电池电压检测和蜂鸣器控制的接口。
 */

#include "headfile.h"

extern uint16 battery_voltage; //电池电压
extern uint16 battery_percentage; //电池电量百分比

void battery_init(void);
void update_battery_status(void);
void Beep_init(void);
void Beep_set(uint8 status);
uint16 get_battery_voltage(void);
uint16 get_battery_percentage(void);
#endif // BATTERY_H
