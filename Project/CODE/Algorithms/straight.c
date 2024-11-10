/*
*�˴������ʹС������ֱ��ǰ��
*By JNU-Fly
*/

#include "headfile.h"
#include "straight.h"

/**
 * @brief ֱ����ں���
 * @return uint8 ����ֵ
 */
uint8 straight_entrance(void){
    ips200_clear();
    ips200_show_string(0,0,"Car is running straight");
    PID_control();
    
    // ��ӷ���ֵ
    return 0;
}

/*PID����������*/
int Kp = 1;
int Ki = 0;
int Kd = 0;
float reactFactor = 2; 

/**
 * @brief PID���ƺ���
 */
void PID_control(void){
    int position;
    int error;
    int last_error = 0;
    int integral = 0;
    int derivative = 0;
    int output = 0;
    int angle = 0;
    while(1){
        position = Line_calculate_position();
        if (position > 100 || position < -100)
            exception_handler(position);
        error = position;
        integral = (integral + error) * Ki;
        derivative = (error - last_error) * Kd;
        output = error * Kp + integral + derivative;
        angle = Servo_Center + output * reactFactor;
        last_error = error;
        servo_set_position(angle);
        drv8701_control(MOTOR_BOTH, 50);  //��ʱ�õ�����
    }
}

/**
 * @brief �쳣������
 * @param exception_code �쳣����
 */
void exception_handler(int exception_code){
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
            break;
    }
    while(1);
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
    int adc1, adc2, adc3, adc4;
    int position;

    Line_read_raw();
    // �� ADC ֵ���й�һ������
    adc1 = (adc_value.ADC1 * 1000) / 4095;//��Χ��0-1000
    adc2 = (adc_value.ADC2 * 1000) / 4095;
    adc3 = (adc_value.ADC3 * 1000) / 4095;
    adc4 = (adc_value.ADC4 * 1000) / 4095;

    //�ж��ߵĴ���λ�ã�ʹ��������������ֵ,adc1 �� adc4
    if (adc1 > 1000 || adc4 > 1000)
        return 999; // ϵͳ����
    if ((adc1 + adc4) == 0)
        return 998; // �������
    if ((adc1 + adc4) < 100)
        return 997; // �߶�ʧ
    if (adc1 > 800 && adc2 > 800)
        return 996;                                 // ʮ����
    if ((adc1 - adc2) > 200)                        //�󻷵�����
    {
        delay(100);
        if ((adc1 - adc2) > 250)
        {
            return 995;//�󻷵�
        }
    }
    if ((adc4 - adc3) > 200)                        //�һ�������
    {
        delay(100);
        if ((adc4 - adc3) > 250)
        {
            return 994;//�һ���
        }
    }
    position = (adc1 - adc4) * 100 / (adc1 + adc4); // ��Ⱥ��㷨
    return position;
}