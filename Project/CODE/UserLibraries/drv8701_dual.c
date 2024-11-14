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

// 更新为与示例代码一致的引脚定义
#define PWM_1  PWMA_CH4P_P26   // 电机1 PWM通道
#define PWM_2  PWMA_CH2N_P13   // 电机2 PWM通道

#define DIR_1 P24    // 电机1方向控制引脚
#define DIR_2 P10    // 电机2方向控制引脚
    
/**
 * @brief 初始化DRV8701电机驱动，包括PWM和GPIO的初始化
 */
void drv8701_init(void)
{
    // 初始化PWM通道，使用17kHz频率，占空比为0
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);

    // 初始化方向控制引脚为推挽输出模式
    gpio_mode(DIR_1, GPO_PP);
    gpio_mode(DIR_2, GPO_PP);
}

/**
 * @brief 控制DRV8701电机的方向和速度
 * @param motor_select 电机选择，MOTOR_L、MOTOR_R 或 MOTOR_BOTH
 * @param speed 速度值，范围-100到100
 */
void drv8701_control(Motor_Select motor_select, int32 speed)//speed范围-100到100
{
    if (speed > 100) // 限制速度范围
    {
        speed = 100;
    }
    else if (speed < -100)
    {
        speed = -100;
    }
    speed = ((speed * PWM_DUTY_MAX) / 200); // 将速度值转换为PWM占空比
    if (motor_select == MOTOR_L || motor_select == MOTOR_BOTH)//控制左边舵机
    {
        if (speed >= 0)
        {
            DIR_1 = 1;
            pwm_duty(PWM_1, speed);
        }
        else
        {
            DIR_1 = 0;
            pwm_duty(PWM_1, -speed);
        }
    }
    if (motor_select == MOTOR_R || motor_select == MOTOR_BOTH)//控制右边舵机
    {
        if (speed >= 0)
        {
            DIR_2 = 1;
            pwm_duty(PWM_2, speed);
        }
        else
        {
            DIR_2 = 0;
            pwm_duty(PWM_2, -speed);
        }
    }
}

/**
 * @brief 停止DRV8701电机
 * @param motor_select 电机选择，MOTOR_L、MOTOR_R 或 MOTOR_BOTH
 */
void drv8701_stop(Motor_Select motor_select)
{
    if (motor_select == MOTOR_L || motor_select == MOTOR_BOTH)
    {
        pwm_duty(PWM_1, 0);
    }

    if (motor_select == MOTOR_R || motor_select == MOTOR_BOTH)
    {
        pwm_duty(PWM_2, 0);
    }
}
