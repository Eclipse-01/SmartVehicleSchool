#include "headfile.h"
#include "line.h"

ADC_Value adc_value; // ���ڴ˴�����

//���ص��ߵ�λ���Ǹ���adc1��adc4��ֵ�����

#define FILTER_SIZE 5

int filter_buffer1[FILTER_SIZE] = {0};
int filter_buffer2[FILTER_SIZE] = {0};
int filter_buffer3[FILTER_SIZE] = {0};
int filter_buffer4[FILTER_SIZE] = {0};
int filter_index = 0;

/**
 * @brief ��ʼ��ADCͨ��
 * 
 * ��ʼ�����ߴ��������ӵ�ADCͨ����
 */
void Line_init()
{
    adc_init(ADC_P06, 0); // P00����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P11, 0); // P01����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P14, 0); // P05����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P15, 0); // P06����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
}

/**
 * @brief ��ȡԭʼADCֵ�������˲�����
 * 
 * ��ADCͨ����ȡԭʼ���ݣ���ʹ�ü򵥵Ļ��������˲��������ݽ����˲���
 */
void Line_read_raw()
{
    filter_buffer1[filter_index] = adc_once(ADC_P06, ADC_12BIT); // �ɼ�ADC_P00��ѹ������12λ
    filter_buffer2[filter_index] = adc_once(ADC_P11, ADC_12BIT); // �ɼ�ADC_P01��ѹ������12λ
    filter_buffer3[filter_index] = adc_once(ADC_P14, ADC_12BIT); // �ɼ�ADC_P05��ѹ������12λ
    filter_buffer4[filter_index] = adc_once(ADC_P15, ADC_12BIT); // �ɼ�ADC_P06��ѹ������12λ

    filter_index = (filter_index + 1) % FILTER_SIZE;

    adc_value.ADC1 = 0;
    adc_value.ADC2 = 0;
    adc_value.ADC3 = 0;
    adc_value.ADC4 = 0;

    for (int i = 0; i < FILTER_SIZE; i++)
    {
        adc_value.ADC1 += filter_buffer1[i];
        adc_value.ADC2 += filter_buffer2[i];
        adc_value.ADC3 += filter_buffer3[i];
        adc_value.ADC4 += filter_buffer4[i];
    }

    adc_value.ADC1 /= FILTER_SIZE;
    adc_value.ADC2 /= FILTER_SIZE;
    adc_value.ADC3 /= FILTER_SIZE;
    adc_value.ADC4 /= FILTER_SIZE;
}

// /**
//  * @brief �����ߵ�λ��
//  * 
//  * �����˲����ADCֵ�����ߵ�λ�á�
//  * 
//  * @return int �ߵ�λ�ã���������
//  * @warning С��ʹ��
//  */
// int Line_calculate_position()
// {
//     int adc1, adc2, adc3, adc4;
//     int position;

//     Line_read_raw();
//     // �� ADC ֵ���й�һ������
//     adc1 = (adc_value.ADC1 * 1000) / 4095;//��Χ��0-1000
//     adc2 = (adc_value.ADC2 * 1000) / 4095;
//     adc3 = (adc_value.ADC3 * 1000) / 4095;
//     adc4 = (adc_value.ADC4 * 1000) / 4095;

//     //�ж��ߵĴ���λ�ã�ʹ��������������ֵ,adc1 �� adc4
//     if (adc1 > 1000 || adc4 > 1000)
//         return 999; // ϵͳ����
//     if ((adc1 + adc4) == 0)
//         return 998; // �������
//     if ((adc1 + adc4) < 100)
//         return 997; // �߶�ʧ
//     if (adc1 > 800 && adc2 > 800)
//         return 996;                                 // ʮ����
//     if ((adc1 - adc2) > 200)                        //�󻷵�����
//     {
//         delay(100);
//         if ((adc1 - adc2) > 250)
//         {
//             return 995;//�󻷵�
//         }
//     }
//     if ((adc4 - adc3) > 200)                        //�һ�������
//     {
//         delay(100);
//         if ((adc4 - adc3) > 250)
//         {
//             return 994;//�һ���
//         }
//     }
//     position = (adc1 - adc4) * 100 / (adc1 + adc4); // ��Ⱥ��㷨
//     return position;
// }

/**
 * @brief ��ȡADCֵ�ķ�Χ
 * 
 * ������ȡADCֵ����ʾÿ��ͨ������Сֵ�����ֵ��
 */
void Get_ADC_Range()
{
    int min_adc1 = 4095, max_adc1 = 0;
    int min_adc2 = 4095, max_adc2 = 0;
    int min_adc3 = 4095, max_adc3 = 0;
    int min_adc4 = 4095, max_adc4 = 0;
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

        // ��IPS200��ʾ������ʾ��Сֵ�����ֵ
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
        // ��ʾ��ǰֵ
        sprintf(buffer, "ADC1: %d    ", adc_value.ADC1);
        ips200_show_string(0, 160, buffer);
        sprintf(buffer, "ADC2: %d    ", adc_value.ADC2);
        ips200_show_string(0, 180, buffer);
        sprintf(buffer, "ADC3: %d    ", adc_value.ADC3);
        ips200_show_string(0, 200, buffer);
    }
}

void filter_line_data() {
    // ������� i
    for (int i = 0; i < FILTER_SIZE; i++) {
        filter_buffer1[i] = /* �����߼� */;
        // ��������
    }
    // ... existing code ...
}