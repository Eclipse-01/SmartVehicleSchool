#include "headfile.h"
#include "line.h"

ADC_Value adc_value;  // 仅在此处定义

void Line_init(){
    adc_init(ADC_P06, 0);                        // P00引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P11, 0);                        // P01引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P14, 0);                        // P05引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
    adc_init(ADC_P15, 0);                        // P06引脚        具体通道与引脚对应关系可以查看zf_adc.h文件
}                           

void Line_read_raw(){  
        adc_value.ADC1 = adc_once(ADC_P06, ADC_8BIT);    // 采集ADC_P00电压，精度8位
        adc_value.ADC2 = adc_once(ADC_P11, ADC_8BIT);    // 采集ADC_P01电压，精度8位
        adc_value.ADC3 = adc_once(ADC_P14, ADC_8BIT);    // 采集ADC_P05电压，精度8位
        adc_value.ADC4 = adc_once(ADC_P15, ADC_8BIT);    // 采集ADC_P06电压，精度8位
}

int Line_calculate_position(){
    int adc1, adc2, adc3, adc4;
    int position;
    
    Line_read_raw();
    adc1 = adc_value.ADC1;//左侧横传感器
    adc2 = adc_value.ADC2;//左侧竖传感器
    adc3 = adc_value.ADC3;//右侧竖传感器
    adc4 = adc_value.ADC4;//右侧横传感器
    
    // 判断线的大致位置，使用两个传感器的值,adc1 和 adc4
    if(adc1 > 100 || adc4 > 100)
        return 999;//系统错误
    if((adc1 + adc4) == 0)
        return 998;//除零错误
    if ((adc1 + adc4) < 10)
        return 997;//线丢失
    if (adc1 > 80 && adc2 > 80)
        return 996;//十字线
    position = (adc1 - adc4) * 100 / (adc1 + adc4);//差比和算法
    return position;
}
