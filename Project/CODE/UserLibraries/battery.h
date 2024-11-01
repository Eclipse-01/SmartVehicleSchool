#ifndef BATTERY_H
#define BATTERY_H

#include "headfile.h"

void battery_init(void);
uint16 get_battery_voltage(void);
uint16 get_battery_percentage(void);

#endif // BATTERY_H
