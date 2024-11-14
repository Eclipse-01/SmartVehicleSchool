/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"

// 关于内核频率的设定，可以查看board.h文件
// 在board_init中,已经将P54引脚设置为复位
// 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
#define DIR_2 P10
#define DIR_1 P24
#define PWM_2 PWMA_CH2N_P13
#define PWM_1 PWMA_CH4P_P26
uint8  dir = 0;
int16 duty = 0;

void main()
{   
    //int encoder1, encoder2;
	clock_init(SYSTEM_CLOCK_52M);	// 初始化系统频率,勿删除此句代码。
	board_init();					// 初始化寄存器,勿删除此句代码。

    // 此处编写用户代码 例如外设初始化代码等
    ips200_init_spi();
    ips200_show_string(0, 0, "Vehicle Booting...");
    // servo_init();
    drv8701_init();
    // //encoder_init(); //编码器未连接
    // Line_init();
    // straight_entrance();
    wireless_uart_init();
    drv8701_control(MOTOR_BOTH, 100);
    
    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        wireless_uart_send_buff("Hello World!\n", 13);
        delay_ms(1000);
    }
}



