/*
*�˴������ʹС������ֱ��ǰ��
*By JNU-Fly
*/

#include "headfile.h"
#include "straight.h"

/*PID����������*/
float Kp = 0.05;
float Ki = 0;
float Kd = 0.05;
float reactFactor = 1; 

// ��ӻ��ֺ�΢�ֵ����ֵ����
float MAX_INTEGRAL = 100.0;   // ������Ҫ����
float MAX_DERIVATIVE = 1000.0; // ������Ҫ����

/**
 * @brief ֱ����ں���
 * @return uint8 ����ֵ
 */
uint8 straight_entrance(void){
    ips200_clear();
    ips200_show_string(0,0,"Car is running straight");
    PID_control();
    return 0;
}

/**
 * @brief PID���ƺ���
 */
void PID_control(void){
    int position;
    static int last_error = 0;
    static int integral = 0;
    int angle;
    while(1){
    position = Line_calculate_position();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH,0);
    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    if (last_error > MAX_DERIVATIVE)
        last_error = MAX_DERIVATIVE;
    if (last_error < -MAX_DERIVATIVE)
        last_error = -MAX_DERIVATIVE;
    delay_ms(10);
    }
}

/**
 * @brief �쳣������
 * @param exception_code �쳣����
 */
void exception_handler(int exception_code)
{
    ips200_clear();
    ips200_show_string(0,0,"Exception occured");
    switch(exception_code){
        case 999:
            ips200_show_string(16,0,"System error");
            break;
        case 998:
            ips200_show_string(16,0,"Divide by zero");
            break;
        case 997:
            ips200_show_string(16,0,"Line lost");
            break;
        case 996:
            ips200_show_string(16,0,"Cross line");
            return;
        case 995:
            ips200_show_string(16,0,"Left roundabout");
            break;
        case 994:
            ips200_show_string(16,0,"Right roundabout");
            break;
        default:
            ips200_show_string(16,0,"Unknown error");
    }
}

/**
 * @brief �����ߵ�λ��
 * 
 * �����˲����ADCֵ�����ߵ�λ�á�
 * 
 * @return int �ߵ�λ�ã���������
 * @warning С��ʹ��
 */
int Line_calculate_position()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    char string_buffer[50];
    Line_read_raw();
    //��һ��������Χ��0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;

    denominator = adc1 + adc2 + adc3 + adc4;

    // //�ж��ߵĴ���λ�ã�ʹ��������������ֵ,adc1 �� adc4
    // if (adc1 > 1000 || adc4 > 1000)
    //     return 999; // ϵͳ����
    if (denominator == 0)
        return 0; // �������
    if (denominator < 50)
        return 0; // �߶�ʧ
    // if ((adc1 + adc4) < 100)
    //     return 997; // �߶�ʧ
    // if (adc1 > 800 && adc2 > 800)
    //     return 996;                                 // ʮ����
    // if ((adc1 - adc2) > 200)                        //�󻷵�����
    // {
    //     delay_ms(100);
    //     if ((adc1 - adc2) > 250)
    //     {
    //         return 995;//�󻷵�
    //     }
    // }
    // if ((adc4 - adc3) > 200)                        //�һ�������
    // {
    //     delay_ms(100);
    //     if ((adc4 - adc3) > 250)
    //     {
    //         return 994;//�һ���
    //     }
    // }
    // ʹ�ò�Ⱥ��㷨����λ��
    position = ((adc1 - adc4) + (adc2 - adc3)) * 1000.0 / denominator;
    //��Ļ��ʾ
    sprintf(string_buffer, "ADC1:%d    ", (int)adc1);
    ips200_show_string(0, 32, string_buffer);
    sprintf(string_buffer, "ADC2:%d    ", (int)adc2);
    ips200_show_string(0, 48, string_buffer);
    sprintf(string_buffer, "ADC3:%d    ", (int)adc3);
    ips200_show_string(0, 64, string_buffer);
    sprintf(string_buffer, "ADC4:%d    ", (int)adc4);
    ips200_show_string(0, 80, string_buffer);
    sprintf(string_buffer, "Denominator:%d    ", (int)denominator);
    ips200_show_string(0, 96, string_buffer);
    sprintf(string_buffer, "Position:%d    ", (int)position);
    ips200_show_string(0, 112, string_buffer);
    sprintf(string_buffer, "Oringinal adc1:%d    ", adc_value.ADC1);
    ips200_show_string(0, 128, string_buffer);
    sprintf(string_buffer, "Oringinal adc2:%d    ", adc_value.ADC2);
    ips200_show_string(0, 144, string_buffer);
    sprintf(string_buffer, "Oringinal adc3:%d    ", adc_value.ADC3);
    ips200_show_string(0, 160, string_buffer);
    sprintf(string_buffer, "Oringinal adc4:%d    ", adc_value.ADC4);
    ips200_show_string(0, 176, string_buffer);
    return (int)-position;
}