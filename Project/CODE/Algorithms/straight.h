#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "headfile.h"

// ֱ����ں���
uint8 straight_entrance(void);

// PID���ƺ���
void PID_control(void);

// �쳣������
void exception_handler(int exception_code);

// �����ߵ�λ��
int Line_calculate_position(void);

#endif // STRAIGHT_H
