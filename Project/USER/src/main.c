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

void main()
{   
    char string_buffer[50];
	clock_init(SYSTEM_CLOCK_52M);	// ��ʼ��ϵͳƵ��,��ɾ���˾���롣
	board_init();					// ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������
    ips200_init_spi();
    ips200_show_string(0, 0, "Vehicle Booting...");
    servo_init();
    sprintf(string_buffer, "Servo OK, position:%d", servo_position);
    ips200_show_string(32, 0, string_buffer);
    drv8701_init();
    ips200_show_string(48, 0, "Motor OK");
    encoder_init();
    ips200_show_string(64, 0, "Encoder OK");
    Line_init();
    ips200_show_string(80, 0, "Line OK");
    
    // �˴���д�û����� ���������ʼ�������

    while(1)
    {
		// �˴���д��Ҫѭ��ִ�еĴ���
        Line_read_raw();
        sprintf(string_buffer, "ADC1: %d", adc_value.ADC1);
        ips200_show_string(16, 0, string_buffer);
        sprintf(string_buffer, "ADC2: %d", adc_value.ADC2);
        ips200_show_string(32, 0, string_buffer);
        sprintf(string_buffer, "ADC3: %d", adc_value.ADC3);
        ips200_show_string(48, 0, string_buffer);
        sprintf(string_buffer, "ADC4: %d", adc_value.ADC4);
        ips200_show_string(64, 0, string_buffer);

        delay_ms(500); // ��ʱ500ms
    }
}



