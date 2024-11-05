#ifndef BATTERY_H
#define BATTERY_H

#include "headfile.h"

extern uint16 battery_voltage; //��ص�ѹ
extern uint16 battery_percentage; //��ص����ٷֱ�

void battery_init(void);
void update_battery_status(void);
#endif // BATTERY_H
