#ifndef BATTERY_H
#define BATTERY_H

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
