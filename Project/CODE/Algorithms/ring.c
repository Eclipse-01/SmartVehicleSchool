#include "headfile.h"
#include "ring.h"

#define DISTANCE 1000 //�����뻷�źŵ��뻷�ľ���
#define RETURN_TRACK 1001 //�뻷��ָ�ѭ��ǰ�ľ���
#define SERVO_ENTER_RING_ANGLE 20 //������

uint8 RING_FLAG = 0; // �����ⲿ����

void ring_handler_deprecated(void){
    uint32 distance = 0;
    char str[256];
    int recv = 32;

    while ( distance < DISTANCE ) // �ж��Ƿ񵽴��뻷��
    {
        distance = get_encoder_distance();
        PID_control_ring();
        delay_ms(10);
    }
    wireless_uart_send_buff("We have reached the ring point, setting servo angle\n", 52);
    servo_set_position(SERVO_ENTER_RING_ANGLE); // ������

    while ( distance < RETURN_TRACK ) // �ж��Ƿ񵽴��뻷��ָ�ѭ��ǰ�ľ���
    {
        distance = get_encoder_distance();
        delay_ms(10);
    }
    wireless_uart_send_buff("Ring enterance operation finished\n", 36);
}

void ring_handler(void)
{
    uint32 distance = 0;
    char str[256];
    int recv = 32;
    Beep_set(1); // �����뻷�ź�
    wireless_uart_send_buff("Ring detected, clearing encoders\n", 34);
    Beep_set(1); // �����뻷�ź�
    clear_encoder_distance();
    //�ȴ������ͨ�����ڷ��������ַ��������������ֵ
    while (wireless_uart_read_buff(str,recv) == 0)
    {
        PID_control_ring();
        delay_ms(10);
    }
    //���ͱ�������ֵ
    sprintf(str, "Encoder distance: %d\n", get_encoder_distance());
    wireless_uart_send_buff(str, strlen(str));
    servo_set_position(SERVO_ENTER_RING_ANGLE);//����̶����
    //�ȴ������ͨ�����ڷ��������ַ������Գ�����������ֵ
    while (wireless_uart_read_buff(str,recv) == 0)
    {
        PID_control_ring();
        delay_ms(10);
    }
    //���ͱ�������ֵ
    sprintf(str, "Encoder distance: %d\n", get_encoder_distance());
    wireless_uart_send_buff(str, strlen(str));
    Beep_set(0);
    delay_ms(300);//����װ��
}