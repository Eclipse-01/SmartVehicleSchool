/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * ��Ȩ���� (c) 2020, ��ɿƼ�
 * ��������Ȩ����
 * ����ѧϰ����ʹ�ã�δ����ɲ���������ҵ��;��
 *
 * @file           drv8701_dual.c
 * @company        �ɶ���ɿƼ����޹�˾
 * @author         ��ɿƼ�(QQ790875685)
 * @version        �鿴doc��version�ļ� �汾˵��
 * @Software       MDK FOR C251 V5.60
 * @Target core    STC32F12K
 * @Taobao         https://seekfree.taobao.com/
 * @date           2020-12-18
 * @note           ��AI���� By JNU-FLY
 ********************************************************************************************************************/

#include "drv8701_dual.h"
#include "headfile.h"

// ����Ϊ��ʾ������һ�µ����Ŷ���
#define PWM_1  PWMA_CH4P_P26   // ���1 PWMͨ��
#define PWM_2  PWMA_CH2N_P13   // ���2 PWMͨ��

#define DIR_1 P24    // ���1�����������
#define DIR_2 P10    // ���2�����������
    
/**
 * @brief ��ʼ��DRV8701�������������PWM��GPIO�ĳ�ʼ��
 */
void drv8701_init(void)
{
    // ��ʼ��PWMͨ����ʹ��17kHzƵ�ʣ�ռ�ձ�Ϊ0
    pwm_init(PWM_1, 17000, 0);
    pwm_init(PWM_2, 17000, 0);

    // ��ʼ�������������Ϊ�������ģʽ
    gpio_mode(DIR_1, GPO_PP);
    gpio_mode(DIR_2, GPO_PP);
}

/**
 * @brief ����DRV8701����ķ�����ٶ�
 * @param motor_select ���ѡ��MOTOR_L��MOTOR_R �� MOTOR_BOTH
 * @param speed �ٶ�ֵ����Χ-100��100
 */
void drv8701_control(Motor_Select motor_select, int32 speed)//speed��Χ-100��100
{
    if (speed > 100) // �����ٶȷ�Χ
    {
        speed = 100;
    }
    else if (speed < -100)
    {
        speed = -100;
    }
    speed = ((speed * PWM_DUTY_MAX) / 200); // ���ٶ�ֵת��ΪPWMռ�ձ�
    if (motor_select == MOTOR_L || motor_select == MOTOR_BOTH)//������߶��
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
    if (motor_select == MOTOR_R || motor_select == MOTOR_BOTH)//�����ұ߶��
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
 * @brief ֹͣDRV8701���
 * @param motor_select ���ѡ��MOTOR_L��MOTOR_R �� MOTOR_BOTH
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
