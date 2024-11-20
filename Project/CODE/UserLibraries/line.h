#ifndef LINE_H
#define LINE_H

#include "headfile.h"

typedef struct {        //µç´Å´«¸ÐÆ÷
    uint16 ADC1;
    uint16 ADC2;
    uint16 ADC3;
    uint16 ADC4;
} ADC_Value;

extern ADC_Value adc_value;

void Line_init(void);
void Line_read_raw(void);
//int Line_calculate_position(void);
void Get_ADC_Range(void);
void Remote_Stop(void);

#endif // LINE_H
