/*********************
 * drv8701_dual.c
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件实现了DRV8701双电机驱动的初始化、控制和停止功能。
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
	gpio_mode(P1_0, GPO_PP);	// P10引脚设置为推挽输出
	gpio_mode(P2_4, GPO_PP);	// P24引脚设置为推挽输出
}

/**
 * @brief 控制DRV8701电机的方向和速度
 * @param motor_select 电机选择，MOTOR_L、MOTOR_R 或 MOTOR_BOTH
 * @param speed 速度值，范围-100到100
 */
void drv8701_control(Motor_Select motor_select, int32 speed)//speed范围-100到100
{
    int dir = speed > 0 ? 1 : 0;//若速度大于0，speed_sign为1，否则为0
    speed = abs(speed);//取绝对值
    if (speed > 100) // 限制速度范围
    {
        speed = 100;
    }
    speed = ((speed * PWM_DUTY_MAX) / 200); // 将速度值转换为PWM占空比
    if (motor_select == MOTOR_L || motor_select == MOTOR_BOTH)//控制左边舵机
    {
        if (dir)
        {
            DIR_1 = 0;
            pwm_duty(PWM_1, speed);
        }
        else
        {
            DIR_1 = 1;
            pwm_duty(PWM_1, speed);
        }
    }
    if (motor_select == MOTOR_R || motor_select == MOTOR_BOTH)//控制右边舵机
    {
        if (dir)
        {
            DIR_2 = 1;
            pwm_duty(PWM_2, speed);
        }
        else
        {
            DIR_2 = 0;
            pwm_duty(PWM_2, speed);
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
