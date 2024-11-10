/*
*此代码可以使小车沿着直线前进
*By JNU-Fly
*/

#include "headfile.h"
#include "straight.h"

/**
 * @brief 直行入口函数
 * @return uint8 返回值
 */
uint8 straight_entrance(void){
    ips200_clear();
    ips200_show_string(0,0,"Car is running straight");
    PID_control();
    
    // 添加返回值
    return 0;
}

/*PID参数调节器*/
int Kp = 1;
int Ki = 0;
int Kd = 0;
float reactFactor = 2; 

/**
 * @brief PID控制函数
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
        drv8701_control(MOTOR_BOTH, 50);  //暂时用低速跑
    }
}

/**
 * @brief 异常处理函数
 * @param exception_code 异常代码
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
 * @brief 计算线的位置
 * 
 * 根据滤波后的ADC值计算线的位置。
 * 
 * @return int 线的位置，或错误代码
 * @warning 小心使用
 */
int Line_calculate_position()
{
    int adc1, adc2, adc3, adc4;
    int position;

    Line_read_raw();
    // 对 ADC 值进行归一化处理
    adc1 = (adc_value.ADC1 * 1000) / 4095;//范围是0-1000
    adc2 = (adc_value.ADC2 * 1000) / 4095;
    adc3 = (adc_value.ADC3 * 1000) / 4095;
    adc4 = (adc_value.ADC4 * 1000) / 4095;

    //判断线的大致位置，使用两个传感器的值,adc1 和 adc4
    if (adc1 > 1000 || adc4 > 1000)
        return 999; // 系统错误
    if ((adc1 + adc4) == 0)
        return 998; // 除零错误
    if ((adc1 + adc4) < 100)
        return 997; // 线丢失
    if (adc1 > 800 && adc2 > 800)
        return 996;                                 // 十字线
    if ((adc1 - adc2) > 200)                        //左环岛处理
    {
        delay(100);
        if ((adc1 - adc2) > 250)
        {
            return 995;//左环岛
        }
    }
    if ((adc4 - adc3) > 200)                        //右环岛处理
    {
        delay(100);
        if ((adc4 - adc3) > 250)
        {
            return 994;//右环岛
        }
    }
    position = (adc1 - adc4) * 100 / (adc1 + adc4); // 差比和算法
    return position;
}