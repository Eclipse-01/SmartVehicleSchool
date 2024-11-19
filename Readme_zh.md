# ����Ŀ���ļ���һ�����ĵ�

## Ӳ��������
### 1. �������
- ������������������������ֱ���`drv8701_init()`��`drv8701_control()`��`drv8701_stop()`��`drv8701_init()`�������ڳ�ʼ�������`drv8701_control()`�����������õ�����ٶȣ�`drv8701_stop()`��������ֹͣ�����
- `drv8701_control(MOTOR,speed)`�����Ĳ����ǵ�����ٶȣ��ٶȵķ�Χ��-100��100������0��ʾ�����ת����-100��ʾ�����ת��100��ʾ�����ת��MOTOR��Ҫ���Ƶĵ����������`MOTOR_L`, `MOTOR_R`��`MOTOR_BOTH`(��ʾ�������ͬʱ����)�����磬�������`drv8701_control(MOTOR_BOTH, 30)`���������������30%�İ�ȫ����ǰ����

### 2. ���
- ������������������������ֱ���`servo_init()`��`servo_set_position()`��`servo_init()`�������ڳ�ʼ�������`servo_set_position()`�����������ö���ĽǶȡ�����ĽǶ���һ������������-30~30�����Ƕ���μ�`'headfile.h'`�ļ��е�`'Servo_MaxAngle'`�궨�塣ͬʱ��������е�λ�õ�ռ�ձ�д�ڴ��ļ��е�`'SERVO_CENTER'`�궨���С�Ҫ��ȡ��ǰ����ĽǶȣ���ʹ��ȫ�ֱ���`servo_position`��

### 3. ����ͨ��
- ����ͨ��ģ����Ҫ�������ⲿ�豸�������ݽ������ڵ�ǰ�ĳ����У�ʹ�ô�����С�������κ����ݶ�����ʹ��С��ͣ�£�����ײ��ʲô��

### 4. ���������ݴ���
- ���������ݴ���ģ����Ҫ���ڶ�ȡ�ʹ������������ݡ���Ҫ��������`Line_read_raw()`��`Line_calculate_position()`��`Line_read_raw()`�������ڶ�ȡԭʼ���������ݣ�`Line_calculate_position()`�������ڼ����ߵ�λ�á�`Position`��-100��100������0��ʾ�����м䣬-100��ʾ��������ߣ�100��ʾ�������ұߡ�

### 5. ������
- ������ģ�����ڲ�����������ʻ���롣��Ҫ��������`encoder_init()`��`encoder_read1()`��`encoder_read2()`��`get_encoder_distance()`��`clear_encoder_distance()`��`encoder_init()`�������ڳ�ʼ����������`encoder_read1()`��`encoder_read2()`�������ڶ�ȡ��������ֵ��`get_encoder_distance()`�������ڻ�ȡ��ʻ���룬`clear_encoder_distance()`�������������������¼����ʻ���롣

### 6. ��ع���
- ��ع���ģ�����ڼ���ص�ѹ�͵�������Ҫ��������`battery_init()`��`get_battery_voltage()`��`get_battery_percentage()`��`update_battery_status()`��`battery_init()`�������ڳ�ʼ����ص�ѹ���ģ�飬`get_battery_voltage()`�������ڻ�ȡ��ص�ѹ��`get_battery_percentage()`�������ڻ�ȡ��ص����ٷֱȣ�`update_battery_status()`�������ڸ��µ��״̬���㲻��Ҫʹ����Щ������
- ��Ψһ���õ�����`beep_init()` �� `beep_set(Status)`��`beep_init()`�������ڳ�ʼ����������`beep_set(Status)`�����������÷�������״̬��`Status`�����У�0��ʾ�رշ�������1��ʾ�򿪷�������

## �㷨��
### 1. PID����
- PID�����㷨���ڵ�������Ͷ���Ŀ��Ʋ�������ʵ�־�ȷ�Ŀ��ơ���Ҫ��������`PID_control_straint()`��`PID_control_ring()`��`PID_control_straint()`��������ѭ����ʻ��PID���ƣ�`PID_control_ring()`�������ڻ�����ʻ��PID���ơ�������������ʵ�ֲ�û��ʲô��ͬ������ֻ��Ϊ�˷�ֹһЩ����Ҫ�ĵݹ�������⡣����Ҳ��ͨ�õġ�������Ҫע����ǣ��������������Ǽ��㵱ǰ״̬�µĿ���������������õ�һ�����ʵĽǶȡ��㽫��Ҫ��һ��ѭ���в��ϵ���һ�����ڵ�����Щ������

### 2. ��������
- ���������㷨���ڴ�����������Ҫ��������`ring_handler()`��`ring_handler_deprecated()`��`ring_handler()`�������ڴ��������Ľ�����˳���`ring_handler_deprecated()`�����Ǿɰ汾�Ļ�������������������ʶ��д��`Straight.c`��`Caculate_Line_Position()`�����С���`if (adc1 + adc4 > 160)`�У�`160`��һ����ֵ�������ж��Ƿ���뻷��������Ը���ʵ��������������ֵ���������������Ǿͻ����`ring_handler()`��������������ᴦ�������Ľ�����˳���

## ��Ŀ�ṹ
- `Libraries`Ŀ¼��������Ŀ����ĸ��ֿ��ļ���
- `Project/CODE`Ŀ¼��������Ŀ�ĺ��Ĵ��롣
- `Project/USER`Ŀ¼�������û��Զ���Ĵ��롣
- `Project/MDK`Ŀ¼������MDK�����ļ���

## ���������
- ʹ��Keil MDK������Ŀ����`SEEKFREE.uvproj`�ļ���ѡ��Ŀ���豸Ϊ`STC32F12K`��Ȼ����벢���ص�Ŀ���豸�ϡ�