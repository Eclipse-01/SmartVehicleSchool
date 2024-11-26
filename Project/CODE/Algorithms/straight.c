/*********************
 * straight.c
 * 
 * 作者: COTOMO
 * 项目地址: https://github.com/Eclipse-01/SmartVehicleSchool
 * MIT开源证书
 * JNU-FLY at Jiangnan University
 * 2024 November
 * 
 * 本文件实现了小车直行的PID控制算法和异常处理逻辑。
 ********************************************************************************************************************/

#include "headfile.h"
#include "straight.h"

#define XINGS 1 // 定义十字的数量
#define VELOCITY 40

#define USE_DYNAMIC_SPEED 

#ifdef USE_DYNAMIC_SPEED
#define MOTOR_SPEED dynamic_speed()
#else
#define MOTOR_SPEED VELOCITY
#endif // USE_DYNAMIC_SPEED
int recv = 32;
char str[64];
int dynamic_speed();
/*PID参数调节器*/
float Kp = 0.595;
float Ki = 0.00000;
float Kd = 3.145;
float reactFactor = 1;

// 添加积分和微分的最大值变量
float MAX_INTEGRAL = 100.0;    // 根据需要调整
float MAX_DERIVATIVE = 1000.0; // 根据需要调整

/**
 * @brief 直行入口函数
 * @return uint8 返回值
 */
uint8 straight_entrance(void)
{
    ips200_clear();
    ips200_show_string(0, 0, "Straight mode");
    wireless_uart_send_buff("Straight mode\n", 13);
    while (1)
    {
        Remote_Stop();
        PID_control_straint();
        delay_ms(10);
    }
    return 0;
}

/**
 * @brief PID控制函数
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
    drv8701_control(MOTOR_BOTH, MOTOR_SPEED);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    // if (last_error > MAX_DERIVATIVE)
    //     last_error = MAX_DERIVATIVE;
    // if (last_error < -MAX_DERIVATIVE)
    //     last_error = -MAX_DERIVATIVE;
}

void PID_control_ring(void)
{

    position = Line_calculate_position_ring();
    integral += position;
    angle = (position * Kp + integral * Ki + (position - last_error) * Kd) * reactFactor;
    last_error = position;
    servo_set_position(angle);
    drv8701_control(MOTOR_BOTH,25);

    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL;
    if (integral < -MAX_INTEGRAL)
        integral = -MAX_INTEGRAL;
    // if (last_error > MAX_DERIVATIVE)
    //     last_error = MAX_DERIVATIVE;
    // if (last_error < -MAX_DERIVATIVE)
    //     last_error = -MAX_DERIVATIVE;
}

static float last_speed = 0;
/**
 * @brief 动态速度控制函数
 * @return int 返回速度
 */
int dynamic_speed()
{
    float this_speed;
    this_speed = VELOCITY - abs(servo_position) * 0.2 - abs(position) * 0.07;
    if (this_speed - last_speed > 0.7)
        this_speed = last_speed + 0.7;
    if (this_speed < 30)
        this_speed = 30;
    last_speed = this_speed;
    return this_speed;
}

/**
 * @brief 异常处理函数
 * @param exception_code 异常代码
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
 * @brief 计算线的位置
 *
 * 根据滤波后的ADC值计算线的位置，还能检测环岛
 *
 * @return int 线的位置
 * @warning 小心使用
 */

extern uint8 RING_FLAG = 0; // 声明外部变量
int Line_calculate_position()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // 归一化处理，范围是0-100
    adc1 = ((float)adc_value.ADC1 * 100) / 4095;
    adc2 = ((float)adc_value.ADC2 * 100) / 4095;
    adc3 = ((float)adc_value.ADC3 * 100) / 4095;
    adc4 = ((float)adc_value.ADC4 * 100) / 4095;

    if ((adc1 + adc4 > 160)) // 检测到环岛
    {
        RING_FLAG = RING_FLAG ^ 1;
        if (RING_FLAG == 1){
        Beep_set(1);
        last_speed = 30;
        ring_handler();}
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
 * @brief 计算线的位置
 *
 * 根据滤波后的ADC值计算线的位置，在环岛内使用
 *
 * @return int 线的位置
 * @warning 小心使用
 */

extern uint8 RING_FLAG; // 声明外部变量
int Line_calculate_position_ring()
{
    float adc1, adc2, adc3, adc4, denominator, position;
    Line_read_raw();

    // 归一化处理，范围是0-100
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

