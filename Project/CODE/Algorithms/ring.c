/*********************
 * ring.c
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件实现了车辆进入和退出环岛的处理逻辑，包括距离计算和舵机角度控制。
 ********************************************************************************************************************/

#include "headfile.h"
#include "ring.h"

#define DISTANCE 4500 // 触发入环信号到入环的距离
#define RETURN_TRACK 12000 // 入环后恢复循迹前的距离
#define SERVO_ENTER_RING_ANGLE -33 // 舵机打角

uint8 RING_FLAG = 0; // 定义外部变量

/**
 * @brief 处理环岛的函数
 *
 * 该函数用于处理车辆进入环岛的逻辑，包括距离计算和舵机角度设置。
 */
void ring_handler(void){
    uint32 distance = 0;
    uint32 normal_distance = 0; // 记录入环前的距离
    uint32 real_distance = 0; // 记录实际距离

    /*
    * 为什么要用三个distance呢？因为有时候在调用clear_encoder_distance()函数时，
    * 这个值并不一定会被清到0，所以需要一个normal_distance来记录入环前的读数，至于real_distance
    * 则是单纯用来减少逻辑表达式的复杂度
    */

    char str[64];
    clear_encoder_distance();
    normal_distance = get_encoder_distance();
    sprintf(str, "Normal distance: %d\n", normal_distance);
    wireless_uart_send_buff(str, strlen(str));

    while ( real_distance < DISTANCE ) // 判断是否到达入环点，这个循环控制了舵机打角的时机
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
    while ( distance - normal_distance < RETURN_TRACK ) // 判断是否到达入环后恢复循迹前的距离，即控制舵机打角的时长
    {
        distance = get_encoder_distance();
        real_distance = distance - normal_distance;
        delay_ms(10);
    }
		Beep_set(0);
    wireless_uart_send_buff("Ring enterance operation finished\n", 36);
    // 退出环岛控制，恢复循迹
}
