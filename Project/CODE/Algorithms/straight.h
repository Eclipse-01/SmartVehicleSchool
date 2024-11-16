#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "headfile.h"

// ֱ����ں���
uint8 straight_entrance(void);

// PID���ƺ���
void PID_control_straint(void);
void PID_control_ring(void);

// �쳣������
void exception_handler(int exception_code);

// �����ߵ�λ��
int Line_calculate_position(void);
int Line_calculate_position_ring(void);

extern uint8 RING_FLAG; // �����ⲿ����

#endif // STRAIGHT_H
