/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company			成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-11-18
 * @note		    由AI生成 By JNU-FLY
 ********************************************************************************************************************/

#include "headfile.h"

#define BAT_VOL_PIN ADC_P05
#define BEEP P32

uint16 ad_result = 0;           // 引脚电压
uint32 temp;

uint16 battery_voltage = 0;     // 电池电压
uint16 battery_percentage = 0;  // 电池电量百分比

/**
 * @brief 初始化蜂鸣器
 *
 * 该函数将 P3.2 引脚配置为推挽输出模式，用于控制蜂鸣器。
 */
void Beep_init(void){
    gpio_mode(P3_2,GPO_PP);	// 将P6.7设置为推挽输出
    P32 = 0;	// 初始化为低电平
}

/**
 * @brief 设置蜂鸣器状态
 * 
 * 该函数用于设置蜂鸣器状态。
 * 
 * @param status 蜂鸣器状态，0为关闭，1为打开
 */
void Beep_set(uint8 status){
    P32 = status;
}

/**
 * @brief 初始化电池电压检测模块
 * 
 * 该函数初始化用于电池电压检测的ADC模块。
 */
void battery_init(void){
    adc_init(BAT_VOL_PIN, ADC_SYSclk_DIV_32);
}

/**
 * @brief 获取电池电压
 * 
 * 该函数通过ADC读取电池电压，并根据分压电阻计算实际电池电压。
 * 如果电池电压低于6.5V，将显示“LOW BATTERY”并停止电机。可以在battery.h中修改LOW_BATTERY_VOLTAGE的值为-1禁用此功能
 * 
 * @return uint16 返回电池电压，单位为毫伏
 */
uint16 get_battery_voltage(void){
    ad_result = adc_once(BAT_VOL_PIN, ADC_12BIT);
    temp = (((uint32)ad_result * 3300) / 4095);  //计算出当前adc引脚的电压 计算公式为 ad_result*VCC/ADC分辨率    VCC单位为mv
    battery_voltage =  temp * 11; //根据引脚电压  和分压电阻的阻值计算电池电压 计算公司为   引脚电压*(R2+R3)/R3   R3为接地端电阻
    while (battery_voltage < LOW_BATTERY_VOLTAGE){
        ips200_show_string(120,200,"LOW BATTERY");
        drv8701_stop(MOTOR_BOTH); //停止电机防止电池过放
        while(ENABLE_LOW_BATTERY_PROTECTION)
        delay_ms(10000); // 卡死程序防止程序启动
    }
    
    return battery_voltage;
}

uint16 get_battery_percentage(void){
    int max_voltage = 4200 * BATTERY_GROUP_NUM;
    int min_voltage = LOW_BATTERY_VOLTAGE * BATTERY_GROUP_NUM;
    int current_voltage = get_battery_voltage();
    int percentage = (current_voltage - min_voltage) * 100 / (max_voltage - min_voltage);
    if (percentage > 100){
        percentage = 100;
    }
    if (percentage < 0){
        percentage = 0;
    }
    return percentage;
}

/**
 * @brief 更新电池状态
 * 
 * 该函数根据当前电池电压和电量百分比更新电池状态和充电状态。
 */
void update_battery_status(void) {
    battery_percentage = get_battery_percentage();
}


