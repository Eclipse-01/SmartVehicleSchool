# 对项目和文件的一个大文档

## 硬件驱动类
### 1. 电机驱动
- 电机驱动有三个基本函数，分别是`drv8701_init()`、`drv8701_control()`和`drv8701_stop()`。`drv8701_init()`函数用于初始化电机，`drv8701_control()`函数用于设置电机的速度，`drv8701_stop()`函数用于停止电机。
- `drv8701_control(MOTOR,speed)`函数的参数是电机的速度，速度的范围是-100到100，其中0表示电机不转动，-100表示电机倒转，100表示电机正转。MOTOR是要控制的电机，可以是`MOTOR_L`, `MOTOR_R`或`MOTOR_BOTH`(表示两个电机同时控制)。比如，你可以用`drv8701_control(MOTOR_BOTH, 30)`来控制两个电机以30%的安全功率前进。

### 2. 舵机
- 舵机驱动有两个基本函数，分别是`servo_init()`和`servo_set_position()`。`servo_init()`函数用于初始化舵机，`servo_set_position()`函数用于设置舵机的角度。舵机的角度是一个整数，比如-30~30。最大角度请参见`'headfile.h'`文件中的`'Servo_MaxAngle'`宏定义。同时，舵机居中的位置的占空比写在此文件中的`'SERVO_CENTER'`宏定义中。要获取当前舵机的角度，请使用全局变量`servo_position`。

### 3. 无线通信
- 无线通信模块主要用于与外部设备进行数据交换。在当前的程序中，使用串口向小车发送任何内容都可以使得小车停下，避免撞到什么。

### 4. 传感器数据处理
- 传感器数据处理模块主要用于读取和处理传感器数据。主要函数包括`Line_read_raw()`和`Line_calculate_position()`。`Line_read_raw()`函数用于读取原始传感器数据，`Line_calculate_position()`函数用于计算线的位置。`Position`从-100到100，其中0表示线在中间，-100表示线在最左边，100表示线在最右边。

### 5. 编码器
- 编码器模块用于测量车辆的行驶距离。主要函数包括`encoder_init()`、`encoder_read1()`、`encoder_read2()`、`get_encoder_distance()`和`clear_encoder_distance()`。`encoder_init()`函数用于初始化编码器，`encoder_read1()`和`encoder_read2()`函数用于读取编码器的值，`get_encoder_distance()`函数用于获取行驶距离，`clear_encoder_distance()`函数用于清除编码器记录的行驶距离。

### 6. 电池管理
- 电池管理模块用于监测电池电压和电量。主要函数包括`battery_init()`、`get_battery_voltage()`、`get_battery_percentage()`和`update_battery_status()`。`battery_init()`函数用于初始化电池电压检测模块，`get_battery_voltage()`函数用于获取电池电压，`get_battery_percentage()`函数用于获取电池电量百分比，`update_battery_status()`函数用于更新电池状态。你不需要使用这些东西。
- 你唯一会用到的是`beep_init()` 和 `beep_set(Status)`。`beep_init()`函数用于初始化蜂鸣器，`beep_set(Status)`函数用于设置蜂鸣器的状态。`Status`参数中，0表示关闭蜂鸣器，1表示打开蜂鸣器。

## 算法类
### 1. PID控制
- PID控制算法用于调整电机和舵机的控制参数，以实现精确的控制。主要函数包括`PID_control_straint()`和`PID_control_ring()`。`PID_control_straint()`函数用于循线行驶的PID控制，`PID_control_ring()`函数用于环岛行驶的PID控制。这两个函数的实现并没有什么不同，后者只是为了防止一些不必要的递归调用问题。参数也是通用的。不过需要注意的是，这两个函数都是计算当前状态下的控制量，将舵机设置到一个合适的角度。你将需要在一个循环中不断地以一定周期调用这些函数。

### 2. 环岛处理
- 环岛处理算法用于处理环岛。`ring_handler()`函数用于处理环岛的进入。入环条件于`Line_calculate_position`进行计算，当车辆的电感检测到环岛信号时，`Line_calculate_position`会主动调用`ring_handler()`函数。`ring_handler()`函数会使车辆进入环岛，并在环岛中行驶。

## 项目结构
- `Libraries`目录包含了项目所需的各种库文件。
- `Project/CODE`目录包含了项目的核心代码。
- `Project/USER`目录包含了用户自定义的代码。
- `Project/MDK`目录包含了MDK工程文件。

## 编译和运行
- 使用Keil MDK编译项目。打开`SEEKFREE.uvproj`文件，选择目标设备为`STC32F12K`，然后编译并下载到目标设备上。
