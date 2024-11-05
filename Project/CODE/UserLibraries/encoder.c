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

#ifdef encoder_swap
#define encoder_Left encoder_read1
#define encoder_Right encoder_read2
#endif

void encoder_init(void)
{
    // ��ʼ��������1������CTIM0_P34��LSB1������������P35��DIR1��
    ctimer_count_init(CTIM0_P34);
    // ��ʼ��������2������CTIM3_P04��LSB2������������P53��DIR2��
    ctimer_count_init(CTIM3_P04);
}

int16 encoder_read1(void)
{
    int16 dat;
    ctimer_count_clean(CTIM0_P34);
    delay_ms(encoder_time);
    if(P35 == 1) // DIR1
    {
        dat = ctimer_count_read(CTIM0_P34);
    }
    else
    {
        dat = -ctimer_count_read(CTIM0_P34);
    }
    return dat;
}

int16 encoder_read2(void)
{
    int16 dat;
    ctimer_count_clean(CTIM3_P04);
    delay_ms(encoder_time);
    if(P53 == 1) // DIR2
    {
        dat = ctimer_count_read(CTIM3_P04);
    }
    else
    {
        dat = -ctimer_count_read(CTIM3_P04);
    }
    return dat;
}
