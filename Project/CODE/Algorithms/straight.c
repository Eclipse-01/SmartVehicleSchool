/*
 *�˴������ʹС������ֱ��ǰ��
 *By JNU-Fly
 */

#include "headfile.h"
#include "straight.h"

#define XINGS 1 // ����ʮ�ֵ�����
int recv = 32;
char str[64];
/*PID����������*/
float Kp = 0.5;
float Ki = 0;
float Kd = 0.35;
float reactFactor = 1;

// ��ӻ��ֺ�΢�ֵ����ֵ����
float MAX_INTEGRAL = 100.0;    // ������Ҫ����
float MAX_DERIVATIVE = 1000.0; // ������Ҫ����

/**
 * @brief ֱ����ں���
 * @return uint8 ����ֵ
 */
uint8 straight_entrance(void)
{
    ips200_clear();
    wireless_uart_send_buff("Straight mode\n", 13);
    while (1)
    {
        PID_control_straint();
        delay_ms(10);
    }
    return 0;
}

/**
 * @brief PID���ƺ���
 */

int position;
static int last_error = 0;
static int integral = 0;
int angle;

void PID_control_straint(void)
{

    position = Line_calculate_position();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH, 35);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    if (last_error > MAX_DERIVATIVE)
        last_error = MAX_DERIVATIVE;
    if (last_error < -MAX_DERIVATIVE)
        last_error = -MAX_DERIVATIVE;
}

void PID_control_ring(void)
{

    position = Line_calculate_position_ring();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH, 35);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    if (last_error > MAX_DERIVATIVE)
        last_error = MAX_DERIVATIVE;
    if (last_error < -MAX_DERIVATIVE)
        last_error = -MAX_DERIVATIVE;
}

/**
 * @brief �쳣������
 * @param exception_code �쳣����
 */
void exception_handler(int exception_code)
{
    ips200_clear();
    ips200_show_string(0, 0, "Exception occured");
    switch (exception_code)
    {
    case 999:
        ips200_show_string(16, 0, "System error");
        break;
    case 998:
        ips200_show_string(16, 0, "Divide by zero");
        break;
    case 997:
        ips200_show_string(16, 0, "Line lost");
        break;
    case 996:
        ips200_show_string(16, 0, "Cross line");
        return;
    case 995:
        ips200_show_string(16, 0, "Left roundabout");
        break;
    case 994:
        ips200_show_string(16, 0, "Right roundabout");
        break;
    default:
        ips200_show_string(16, 0, "Unknown error");
    }
}

/**
 * @brief �����ߵ�λ��
 *
 * �����˲����ADCֵ�����ߵ�λ�ã����ܼ�⻷��
 *
 * @return int �ߵ�λ��
 * @warning С��ʹ��
 */

extern uint8 RING_FLAG; // �����ⲿ����
int Line_calculate_position()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // ��һ��������Χ��0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;

    if ((adc1 + adc4 > 160) && RING_FLAG == 0) // ��⵽����
    {
        Beep_set(1);
        ring_handler();
    }
    denominator = adc1 + adc4;
    if (denominator == 0)
        denominator = 1;
    position = (adc4 - adc1) / denominator * 100;
    sprintf(str, "Position: %f, adc1: %f, adc4: %f\n", position, adc1, adc4);
    wireless_uart_send_buff(str, strlen(str));
    return position;
}

/**
 * @brief �����ߵ�λ��
 *
 * �����˲����ADCֵ�����ߵ�λ�ã��ڻ�����ʹ��
 *
 * @return int �ߵ�λ��
 * @warning С��ʹ��
 */

extern uint8 RING_FLAG; // �����ⲿ����
int Line_calculate_position_ring()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // ��һ��������Χ��0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;
    denominator = adc1 + adc4;
    if (denominator == 0)
        denominator = 1;
    position = (adc4 - adc1) / denominator * 100;
    sprintf(str, "Position: %f\n, adc1: %f, adc4: %f\n", position, adc1, adc4);
    wireless_uart_send_buff(str, strlen(str));
    return position;
}