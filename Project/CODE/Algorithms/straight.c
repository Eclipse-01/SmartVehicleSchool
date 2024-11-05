/*
*此代码可以使小车沿着直线前进
*By JNU-Fly
*/

#include "headfile.h"

uint8 straight_entrance(void){
    ips200_clear();
    ips200_show_string(0,0,"Car is running straight");
    PID_control();
}

/*PID参数调节器*/
int Kp = 1;
int Ki = 0;
int Kd = 0;
float reactFactor = 2; 

void PID_control(){
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
            return exception_handler(position);
        error = position - 0;
        integral = (integral + error) * Ki;
        derivative = (error - last_error) * Kd;
        output = error * Kp + integral + derivative;
        angle = Servo_Center + output * reactFactor;
        last_error = error;
        servo_set_position(angle);
        drv8701_control(MOTOR_BOTH, 100 - angle);
    }
}

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
            return 0;
        default:
            ips200_show_string(16,0,"Unknown error");
            break;
    }
    while(1);
}