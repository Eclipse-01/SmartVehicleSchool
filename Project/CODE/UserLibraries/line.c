#include "headfile.h"
#include "line.h"

ADC_Value adc_value;  // ���ڴ˴�����

void Line_init(){
    adc_init(ADC_P06, 0);                        // P00����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P11, 0);                        // P01����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P14, 0);                        // P05����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
    adc_init(ADC_P15, 0);                        // P06����        ����ͨ�������Ŷ�Ӧ��ϵ���Բ鿴zf_adc.h�ļ�
}                           

void Line_read_raw(){  
        adc_value.ADC1 = adc_once(ADC_P06, ADC_8BIT);    // �ɼ�ADC_P00��ѹ������12λ
        adc_value.ADC2 = adc_once(ADC_P11, ADC_8BIT);    // �ɼ�ADC_P01��ѹ������12λ
        adc_value.ADC3 = adc_once(ADC_P14, ADC_8BIT);    // �ɼ�ADC_P05��ѹ������12λ
        adc_value.ADC4 = adc_once(ADC_P15, ADC_8BIT);    // �ɼ�ADC_P06��ѹ������12λ
}

int Line_calculate_position(){
    int adc1, adc2, adc3, adc4;
    int position;
    
    Line_read_raw();
    adc1 = adc_value.ADC1;
    adc2 = adc_value.ADC2;
    adc3 = adc_value.ADC3;
    adc4 = adc_value.ADC4;
    
    // �ж��ߵĴ���λ�ã�ʹ��������������ֵ,adc1 �� adc2
    if(adc1 > 100 && adc2 > 100)
        return 999;
    if((adc1 + adc4) == 0)
        return 998;
    if ((adc1 + adc4) < 10)
        return 997;
    position = (adc1 - adc4) * 100 / (adc1 + adc4);//��Ⱥ��㷨
    return position;
}
