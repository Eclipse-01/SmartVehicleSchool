/*********************
 * servo.h
 * COPYRIGHT NOTICE
 * JNU-FLY 2024 at Jiangnan University
 * All rights reserved.
 * www.flysworld.top
 */
#ifndef SERVO_H
#define SERVO_H

#include "zf_pwm.h"

#define Center 1.5
#define MaxAngle 30
#define TurnSpeed 0.14 //Deprecated


void servo_init(void);
void servo_set_position(uint16 position);
uint16 servo_get_position(void);

#endif // SERVO_H
