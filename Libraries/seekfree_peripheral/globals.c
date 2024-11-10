#include "headfile.h"

// 全局变量定义

extern enum{
    Debug,  //调试模式
    Wait,
    Straight,  //简单巡线
    Left_Ring, // 左环
    Right_Ring,  // 右环
    RoadBloack,
    Park
}VehicleFlag;