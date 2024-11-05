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

void servo_init(void);
void servo_set_position(int16 position);

#endif // SERVO_H
