#include "headfile.h"
#include "ring.h"

#define DISTANCE 1000 //触发入环信号到入环的距离
#define RETURN_TRACK 1001 //入环后恢复循迹前的距离
#define SERVO_ENTER_RING_ANGLE 20 //舵机打角

uint8 RING_FLAG = 0; // 定义外部变量

void ring_handler_deprecated(void){
    uint32 distance = 0;
    char str[256];
    int recv = 32;

    while ( distance < DISTANCE ) // 判断是否到达入环点
    {
        distance = get_encoder_distance();
        PID_control_ring();
        delay_ms(10);
    }
    wireless_uart_send_buff("We have reached the ring point, setting servo angle\n", 52);
    servo_set_position(SERVO_ENTER_RING_ANGLE); // 舵机打角

    while ( distance < RETURN_TRACK ) // 判断是否到达入环后恢复循迹前的距离
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
    Beep_set(1); // 发送入环信号
    wireless_uart_send_buff("Ring detected, clearing encoders\n", 34);
    Beep_set(1); // 发送入环信号
    clear_encoder_distance();
    //等待计算机通过串口发送任意字符，报告编码器的值
    while (wireless_uart_read_buff(str,recv) == 0)
    {
        PID_control_ring();
        delay_ms(10);
    }
    //发送编码器的值
    sprintf(str, "Encoder distance: %d\n", get_encoder_distance());
    wireless_uart_send_buff(str, strlen(str));
    servo_set_position(SERVO_ENTER_RING_ANGLE);//舵机固定打角
    //等待计算机通过串口发送任意字符，测试出环岛编码器值
    while (wireless_uart_read_buff(str,recv) == 0)
    {
        PID_control_ring();
        delay_ms(10);
    }
    //发送编码器的值
    sprintf(str, "Encoder distance: %d\n", get_encoder_distance());
    wireless_uart_send_buff(str, strlen(str));
    Beep_set(0);
    delay_ms(300);//出环装死
}