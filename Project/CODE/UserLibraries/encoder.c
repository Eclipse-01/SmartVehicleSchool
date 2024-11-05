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

#ifdef encoder_swap
#define encoder_Left encoder_read1
#define encoder_Right encoder_read2
#endif

void encoder_init(void)
{
    // 初始化编码器1，引脚CTIM0_P34（LSB1），方向引脚P35（DIR1）
    ctimer_count_init(CTIM0_P34);
    // 初始化编码器2，引脚CTIM3_P04（LSB2），方向引脚P53（DIR2）
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
