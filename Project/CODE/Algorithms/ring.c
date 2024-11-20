#include "headfile.h"
#include "ring.h"

#define DISTANCE 7000 //触发入环信号到入环的距离
#define RETURN_TRACK 13000 //入环后恢复循迹前的距离
#define SERVO_ENTER_RING_ANGLE -39 //舵机打角

uint8 RING_FLAG = 0; // 定义外部变量

void ring_handler(void){
    uint32 distance = 0;
    uint32 normal_distance = 0;
    uint32 real_distance = 0;
    char str[64];
    clear_encoder_distance();
    normal_distance = get_encoder_distance();
    sprintf(str, "Normal distance: %d\n", normal_distance);
    wireless_uart_send_buff(str, strlen(str));

    while ( real_distance < DISTANCE ) // 判断是否到达入环点
    {
        sprintf(str, "Distance: %d\n", distance - normal_distance);
        wireless_uart_send_buff(str, strlen(str));
        distance = get_encoder_distance();
        real_distance = distance - normal_distance;
        PID_control_ring();
        delay_ms(10);
    }
    wireless_uart_send_buff("We have reached the ring point, setting servo angle\n", 52);
    servo_set_position(SERVO_ENTER_RING_ANGLE); // 舵机打角
    while ( distance - normal_distance < RETURN_TRACK ) // 判断是否到达入环后恢复循迹前的距离
    {
        distance = get_encoder_distance();
        real_distance = distance - normal_distance;
        delay_ms(10);
    }
    wireless_uart_send_buff("Ring enterance operation finished\n", 36);
}
