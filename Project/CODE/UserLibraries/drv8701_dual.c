/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * 版权所有 (c) 2020, 逐飞科技
 * 保留所有权利。
 * 仅供学习交流使用，未经许可不得用于商业用途。
 *
 * @file           drv8701_dual.c
 * @company        成都逐飞科技有限公司
 * @author         逐飞科技(QQ790875685)
 * @version        查看doc内version文件 版本说明
 * @Software       MDK FOR C251 V5.60
 * @Target core    STC32F12K
 * @Taobao         https://seekfree.taobao.com/
 * @date           2020-12-18
 * @note           由AI生成 By JNU-FLY
 ********************************************************************************************************************/

#include "drv8701_dual.h"
#include "headfile.h"

// 定义DRV8701的相关引脚
#define DIR_1_PIN      P2_4            // 电机1方向控制引脚
#define PWM_1_CHANNEL  PWMA_CH4P_P26   // 电机1 PWM通道

#define DIR_2_PIN      P1_0            // 电机2方向控制引脚
#define PWM_2_CHANNEL  PWMA_CH2N_P13   // 电机2 PWM通道

/**
 * @brief 设置单个电机的方向和速度
 * @param dir_pin 引脚定义宏
 * @param motor_pwm_channel PWM通道定义宏
 * @param speed_mapped 映射后的速度值（-10000到10000）
 */
static void set_motor(uint8 dir_pin, uint8 motor_pwm_channel, int speed_mapped)
{
    // 设置方向
    if(speed_mapped >= 0)
    {
        dir_pin = 0;  // 正转
    }
    else
    {
        dir_pin = 1;  // 反转
        speed_mapped = -speed_mapped;  // 取绝对值
    }

    // 设置PWM占空比
    pwm_duty(motor_pwm_channel, (uint32)speed_mapped);
}

/**
 * @brief 初始化DRV8701库，包括PWM和GPIO的初始化
 */
void drv8701_init(void)
{
    // 初始化PWM通道
    pwm_init(PWM_1_CHANNEL, 17000, 0); // 初始化PWM1，频率17kHz，初始占空比0
    pwm_init(PWM_2_CHANNEL, 17000, 0); // 初始化PWM2，频率17kHz，初始占空比0

    // 初始化GPIO为推挽输出模式
    gpio_mode(DIR_1_PIN, GPO_PP); // 电机1方向控制引脚设置为推挽输出
    gpio_mode(DIR_2_PIN, GPO_PP); // 电机2方向控制引脚设置为推挽输出
}

/**
 * @brief 控制DRV8701电机的方向和速度
 * @param motor_select 电机选择，MOTOR_L、MOTOR_R 或 MOTOR_BOTH
 * @param speed 速度值，范围-100到100
 */
void drv8701_control(Motor_Select motor_select, int speed)
{
    int16 mapped_speed; 
    // 限制速度值在-100到100范围内
    if(speed < -100)
        speed = -100;
    else if(speed > 100)
        speed = 100;

    // 将速度从-100到100映射到-10000到10000
    mapped_speed = (speed * PWM_DUTY_MAX) / 100;

    // 根据选择的电机进行控制
    switch(motor_select)
    {
        case MOTOR_L:
            set_motor(DIR_1_PIN, PWM_1_CHANNEL, mapped_speed);
            break;

        case MOTOR_R:
            set_motor(DIR_2_PIN, PWM_2_CHANNEL, mapped_speed);
            break;

        case MOTOR_BOTH:
            set_motor(DIR_1_PIN, PWM_1_CHANNEL, mapped_speed);
            set_motor(DIR_2_PIN, PWM_2_CHANNEL, mapped_speed);
            break;

        default:
            // 选择无效
            break;
    }
}

/**
 * @brief 停止DRV8701电机
 * @param motor_select 电机选择，MOTOR_L、MOTOR_R 或 MOTOR_BOTH
 */
void drv8701_stop(Motor_Select motor_select)
{
    drv8701_control(motor_select, 0);
}
