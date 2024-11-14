#include "headfile.h"
#include "line.h"

ADC_Value adc_value; // 仅在此处定义

// 返回的线的位置是根据adc1和adc4的值计算的

#define FILTER_SIZE 5

/**
 * @brief 初始化ADC通道
 *
 * 初始化与线传感器连接的ADC通道。
 */
void Line_init()
{
    adc_init(ADC_P06, 0); // P00引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P11, 0); // P01引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P14, 0); // P05引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P15, 0); // P06引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
}

/**
 * @brief 读取原始ADC值并进行滤波处理
 *
 * 从ADC通道读取原始数据，并使用简单的滑动窗口滤波器对数据进行滤波。
 */
void Line_read_raw()
{
    int i;
    int filter_buffer1 = 0;
    int filter_buffer2 = 0;
    int filter_buffer3 = 0;
    int filter_buffer4 = 0;
    for (i = 0; i < FILTER_SIZE; i++)//对数据进行滤波
    {
        filter_buffer1 += adc_once(ADC_P06, ADC_12BIT);
        filter_buffer2 += adc_once(ADC_P11, ADC_12BIT);
        filter_buffer3 += adc_once(ADC_P14, ADC_12BIT);
        filter_buffer4 += adc_once(ADC_P15, ADC_12BIT);
    }
    // 将滤波后的值存入结构体
    adc_value.ADC1 = filter_buffer1 / FILTER_SIZE;
    adc_value.ADC2 = filter_buffer2 / FILTER_SIZE;
    adc_value.ADC3 = filter_buffer3 / FILTER_SIZE;
    adc_value.ADC4 = filter_buffer4 / FILTER_SIZE;
}

// /**
//  * @brief 计算线的位置
//  *
//  * 根据滤波后的ADC值计算线的位置。
//  *
//  * @return int 线的位置，或错误代码
//  * @warning 小心使用
//  */
// int Line_calculate_position()
// {
//     int adc1, adc2, adc3, adc4;
//     int position;

//     Line_read_raw();
//     // 对 ADC 值进行归一化处理
//     adc1 = (adc_value.ADC1 * 1000) / 4095;//范围是0-1000
//     adc2 = (adc_value.ADC2 * 1000) / 4095;
//     adc3 = (adc_value.ADC3 * 1000) / 4095;
//     adc4 = (adc_value.ADC4 * 1000) / 4095;

//     //判断线的大致位置，使用两个传感器的值,adc1 和 adc4
//     if (adc1 > 1000 || adc4 > 1000)
//         return 999; // 系统错误
//     if ((adc1 + adc4) == 0)
//         return 998; // 除零错误
//     if ((adc1 + adc4) < 100U) // 将常量改为无符号类型
//         return 997; // 线丢失
//     if (adc1 > 800 && adc2 > 800)
//         return 996;                                 // 十字线
//     if ((adc1 - adc2) > 200)                        //左环岛处理
//     {
//         delay(100);
//         if ((adc1 - adc2) > 250)
//         {
//             return 995;//左环岛
//         }
//     }
//     if ((adc4 - adc3) > 200)                        //右环岛处理
//     {
//         delay(100);
//         if ((adc4 - adc3) > 250)
//         {
//             return 994;//右环岛
//         }
//     }
//     position = (adc1 - adc4) * 100 / (adc1 + adc4); // 差比和算法
//     return position;
// }

/**
 * @brief 获取ADC值的范围
 *
 * 持续读取ADC值并显示每个通道的最小值和最大值。
 */
void Get_ADC_Range()
{
    // 删除未使用的变量 'i'
    // int i;

    // 将变量定义为无符号整数
    unsigned int min_adc1 = 4095, max_adc1 = 0;
    unsigned int min_adc2 = 4095, max_adc2 = 0;
    unsigned int min_adc3 = 4095, max_adc3 = 0;
    unsigned int min_adc4 = 4095, max_adc4 = 0;
    char buffer[30];

    while (1)
    {
        Line_read_raw();
        if (adc_value.ADC1 < min_adc1)
            min_adc1 = adc_value.ADC1;
        if (adc_value.ADC1 > max_adc1)
            max_adc1 = adc_value.ADC1;
        if (adc_value.ADC2 < min_adc2)
            min_adc2 = adc_value.ADC2;
        if (adc_value.ADC2 > max_adc2)
            max_adc2 = adc_value.ADC2;
        if (adc_value.ADC3 < min_adc3)
            min_adc3 = adc_value.ADC3;
        if (adc_value.ADC3 > max_adc3)
            max_adc3 = adc_value.ADC3;
        if (adc_value.ADC4 < min_adc4)
            min_adc4 = adc_value.ADC4;
        if (adc_value.ADC4 > max_adc4)
            max_adc4 = adc_value.ADC4;

        // 在IPS200显示屏上显示最小值和最大值
        sprintf(buffer, "Min ADC1: %d    ", min_adc1);
        ips200_show_string(0, 0, buffer);
        sprintf(buffer, "Max ADC1: %d    ", max_adc1);
        ips200_show_string(0, 20, buffer);
        sprintf(buffer, "Min ADC2: %d    ", min_adc2);
        ips200_show_string(0, 40, buffer);
        sprintf(buffer, "Max ADC2: %d    ", max_adc2);
        ips200_show_string(0, 60, buffer);
        sprintf(buffer, "Min ADC3: %d    ", min_adc3);
        ips200_show_string(0, 80, buffer);
        sprintf(buffer, "Max ADC3: %d    ", max_adc3);
        ips200_show_string(0, 100, buffer);
        sprintf(buffer, "Min ADC4: %d    ", min_adc4);
        ips200_show_string(0, 120, buffer);
        sprintf(buffer, "Max ADC4: %d    ", max_adc4);
        ips200_show_string(0, 140, buffer);
        // 显示当前值
        sprintf(buffer, "ADC1: %d    ", adc_once(ADC_P06, ADC_12BIT));
        ips200_show_string(0, 160, buffer);
        sprintf(buffer, "ADC2: %d    ", adc_once(ADC_P11, ADC_12BIT));
        ips200_show_string(0, 180, buffer);
        sprintf(buffer, "ADC3: %d    ", adc_once(ADC_P14, ADC_12BIT));
        ips200_show_string(0, 200, buffer);
        sprintf(buffer, "ADC4: %d    ", adc_once(ADC_P15, ADC_12BIT));
        ips200_show_string(0, 220, buffer);
    }
}