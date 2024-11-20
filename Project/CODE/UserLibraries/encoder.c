/*********************
 * encoder.c
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件实现了编码器的初始化、读取及距离计算功能。
 *
 * 许可协议详见 LICENSE 文件。
 ********************************************************************************************************************/

#include "headfile.h"

static uint32 TravelDistanceL = 0, TravelDistanceR = 0, TravelDistance = 0;

/**
 * @brief 初始化编码器
 * 
 * 初始化编码器1和编码器2的计数引脚和方向引脚
 */
void encoder_init(void)
{
    // 初始化编码器1，引脚CTIM0_P34（LSB1），方向引脚P35（DIR1）
    ctimer_count_init(CTIM0_P34);
    // 初始化编码器2，引脚CTIM3_P04（LSB2），方向引脚P53（DIR2）
    ctimer_count_init(CTIM3_P04);
}

/**
 * @brief 读取编码器1的值
 * 
 * 清除计数器，延时后根据方向引脚读取编码器1的计数值
 * 
 * @return int16 编码器1的计数值
 */
int16 encoder_read1(void)
{
    int16 dat;
    if(P35 == 1) // DIR1 注意出于安装原因，左侧编码器方向与右侧编码器方向相反
    {
        dat = ctimer_count_read(CTIM0_P34);
    }
    else
    {
        dat = -ctimer_count_read(CTIM0_P34);
    }
    TravelDistanceL += dat;
    ctimer_count_clean(CTIM0_P34);
    delay_ms(encoder_time);
    if(P35 == 1) // DIR1 注意出于安装原因，左侧编码器方向与右侧编码器方向相反
    {
        dat = ctimer_count_read(CTIM0_P34);
    }
    else
    {
        dat = -ctimer_count_read(CTIM0_P34);
    }
    TravelDistanceL += dat;
    return dat;
}

/**
 * @brief 读取编码器2的值
 * 
 * 清除计数器，延时后根据方向引脚读取编码器2的计数值
 * 
 * @return int16 编码器2的计数值
 */
int16 encoder_read2(void)
{
    int16 dat;
    if(P53 == 0) // DIR2
    {
        dat = ctimer_count_read(CTIM3_P04);
    }
    else
    {
        dat = -ctimer_count_read(CTIM3_P04);
    }
    TravelDistanceR += dat;
    ctimer_count_clean(CTIM3_P04);
    delay_ms(encoder_time);
    if(P53 == 0) // DIR2
    {
        dat = ctimer_count_read(CTIM3_P04);
    }
    else
    {
        dat = -ctimer_count_read(CTIM3_P04);
    }
    TravelDistanceR += dat;
    return dat;
}

/**
 * @brief 获取平均行驶距离
 * 
 * 计算左侧和右侧编码器的平均行驶距离
 * 
 * @return int16 平均行驶距离
 */
int16 get_encoder_distance(void)
{
    encoder_read1();
    encoder_read2(); // 更新左右编码器的行驶距离
    TravelDistance = (TravelDistanceL + TravelDistanceR) / 2;
    return TravelDistance;
}

/**
 * @brief 清除编码器行驶距离
 * 
 * 将左侧、右侧和总行驶距离清零
 */
void clear_encoder_distance(void)
{
    TravelDistanceL = 0;
    TravelDistanceR = 0;
    TravelDistance = 0;
}