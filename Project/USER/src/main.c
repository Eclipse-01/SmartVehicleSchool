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


void main(void)
{
    clock_init(SYSTEM_CLOCK_52M); // 初始化系统频率,勿删除此句代码。
    board_init();                 // 初始化寄存器,勿删除此句代码。

    // 此处编写用户代码 例如外设初始化代码等
    ips200_init_spi();
    servo_init();
    drv8701_init();
    encoder_init(); 
    Line_init();
    Beep_init();
    wireless_uart_init();
    // 此处编写用户代码 例如外设初始化代码等
    code_entrance();
}

void code_entrance()
{   
    char str[64];
    int recv = 32;

    straight_entrance(); //这是小车的主要代码，直线行驶，要看小车怎么跑的点这里
    
    sprintf(str, "Vehicle init finished\n");
    wireless_uart_send_buff(str, strlen(str));
    while (1)
    {
        delay_ms(100);
        Get_ADC_Range();
    }
}


//真扯淡，串口只要发中文，管他UTF-8还是GBK，都会乱码，这个串口库真是个垃圾
//老子服了，用英语吧