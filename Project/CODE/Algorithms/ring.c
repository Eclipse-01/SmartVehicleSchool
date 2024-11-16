#include "headfile.h"

#define DISTANCE 1000 //�����뻷�źŵ��뻷�ľ���
#define RETURN_TRACK 1000 //�뻷��ָ�ѭ��ǰ�ľ���

uint8 RING_FLAG = 0; // �����ⲿ����

void ring_handler(void){
    uint32 distance = 0;
    Beep_set(1);//�����뻷�ź�
    Beep_set(0);
    clear_encoder_distance();
    while (distance < DISTANCE){
        distance = get_encoder_distance();
        PID_control_ring();
        delay_ms(10);
    }
    Beep_set(0);
    servo_set_position(20);
    clear_encoder_distance();
    while (distance < RETURN_TRACK){
        distance = get_encoder_distance();
        delay_ms(10);
    }
    //�ָ�ѭ��
    RING_FLAG = 0;
}