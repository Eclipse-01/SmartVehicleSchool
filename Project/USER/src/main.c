/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"

// �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
// ��board_init��,�Ѿ���P54��������Ϊ��λ
// �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
#define DIR_2 P10
#define DIR_1 P24
#define PWM_2 PWMA_CH2N_P13
#define PWM_1 PWMA_CH4P_P26
uint8  dir = 0;
int16 duty = 0;

void main()
{   
    //int encoder1, encoder2;
	clock_init(SYSTEM_CLOCK_52M);	// ��ʼ��ϵͳƵ��,��ɾ���˾���롣
	board_init();					// ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������
    ips200_init_spi();
    ips200_show_string(0, 0, "Vehicle Booting...");
    // servo_init();
    drv8701_init();
    // //encoder_init(); //������δ����
    // Line_init();
    // straight_entrance();
    wireless_uart_init();
    drv8701_control(MOTOR_BOTH, 100);
    
    // �˴���д�û����� ���������ʼ�������

    while(1)
    {
        wireless_uart_send_buff("Hello World!\n", 13);
        delay_ms(1000);
    }
}



