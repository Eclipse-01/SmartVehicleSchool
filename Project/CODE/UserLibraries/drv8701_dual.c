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

// ����DRV8701���������
#define DIR_1_PIN      P2_4            // ���1�����������
#define PWM_1_CHANNEL  PWMA_CH4P_P26   // ���1 PWMͨ��

#define DIR_2_PIN      P1_0            // ���2�����������
#define PWM_2_CHANNEL  PWMA_CH2N_P13   // ���2 PWMͨ��

/**
 * @brief ���õ�������ķ�����ٶ�
 * @param dir_pin ���Ŷ����
 * @param motor_pwm_channel PWMͨ�������
 * @param speed_mapped ӳ�����ٶ�ֵ��-10000��10000��
 */
static void set_motor(uint8 dir_pin, uint8 motor_pwm_channel, int speed_mapped)
{
    // ���÷���
    if(speed_mapped >= 0)
    {
        dir_pin = 0;  // ��ת
    }
    else
    {
        dir_pin = 1;  // ��ת
        speed_mapped = -speed_mapped;  // ȡ����ֵ
    }

    // ����PWMռ�ձ�
    pwm_duty(motor_pwm_channel, (uint32)speed_mapped);
}

/**
 * @brief ��ʼ��DRV8701�⣬����PWM��GPIO�ĳ�ʼ��
 */
void drv8701_init(void)
{
    // ��ʼ��PWMͨ��
    pwm_init(PWM_1_CHANNEL, 17000, 0); // ��ʼ��PWM1��Ƶ��17kHz����ʼռ�ձ�0
    pwm_init(PWM_2_CHANNEL, 17000, 0); // ��ʼ��PWM2��Ƶ��17kHz����ʼռ�ձ�0

    // ��ʼ��GPIOΪ�������ģʽ
    gpio_mode(DIR_1_PIN, GPO_PP); // ���1���������������Ϊ�������
    gpio_mode(DIR_2_PIN, GPO_PP); // ���2���������������Ϊ�������
}

/**
 * @brief ����DRV8701����ķ�����ٶ�
 * @param motor_select ���ѡ��MOTOR_L��MOTOR_R �� MOTOR_BOTH
 * @param speed �ٶ�ֵ����Χ-100��100
 */
void drv8701_control(Motor_Select motor_select, int speed)
{
    int16 mapped_speed; 
    // �����ٶ�ֵ��-100��100��Χ��
    if(speed < -100)
        speed = -100;
    else if(speed > 100)
        speed = 100;

    // ���ٶȴ�-100��100ӳ�䵽-10000��10000
    mapped_speed = (speed * PWM_DUTY_MAX) / 100;
    mapped_speed = motor_max_duty * mapped_speed / 100;

    // ����ѡ��ĵ�����п���
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
            // ѡ����Ч
            break;
    }
}

/**
 * @brief ֹͣDRV8701���
 * @param motor_select ���ѡ��MOTOR_L��MOTOR_R �� MOTOR_BOTH
 */
void drv8701_stop(Motor_Select motor_select)
{
    drv8701_control(motor_select, 0);
}
