#ifndef ENCODER_H
#define ENCODER_H

#include "headfile.h"

void encoder_init(void);
int16 ctimer_count_read_v2(CTIMN_enum tim_n);
int16 encoder_read1(void);
int16 encoder_read2(void);
int16 get_encoder_distance(void);
void clear_encoder_distance(void);

#endif // ENCODER_H
