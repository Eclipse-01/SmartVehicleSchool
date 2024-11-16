#include "headfile.h"

#define DISTANCE 1000 //触发入环信号到入环的距离
#define RETURN_TRACK 1000 //入环后恢复循迹前的距离

uint8 RING_FLAG = 0; // 定义外部变量

void ring_handler(void){
    uint32 distance = 0;
    Beep_set(1);//发送入环信号
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
    //恢复循迹
    RING_FLAG = 0;
}