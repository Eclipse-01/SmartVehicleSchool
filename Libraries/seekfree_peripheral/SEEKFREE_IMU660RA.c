/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		IMU660RA
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
 * ���߶��壺
 *                   ------------------------------------
 *                   ģ��ܽ�            ��Ƭ���ܽ�
 *                   // Ӳ�� SPI ����
 *                   SCL/SPC           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SPC_PIN �궨��
 *                   SDA/DSI           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDI_PIN �궨��
 *                   SA0/SDO           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDO_PIN �궨��
 *                   CS                �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_CS_PIN �궨��
 *                   VCC               3.3V��Դ
 *                   GND               ��Դ��
 *                   ������������
 *
 *                   // ��� IIC ����
 *                   SCL/SPC           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SCL_PIN �궨��
 *                   SDA/DSI           �鿴 SEEKFREE_IMU660RA.h �� IMU660RA_SDA_PIN �궨��
 *                   VCC               3.3V��Դ
 *                   GND               ��Դ��
 *                   ������������
 *                   ------------------------------------
********************************************************************************************************************/

#include "SEEKFREE_IMU660RA.h"

#include "SEEKFREE_CONFIG.h"

#include "zf_delay.h"
#include "zf_spi.h"


#pragma warning disable = 177
#pragma warning disable = 183



int16 imu660ra_gyro_x = 0, imu660ra_gyro_y = 0, imu660ra_gyro_z = 0;            // ��������������   gyro (������)
int16 imu660ra_acc_x = 0, imu660ra_acc_y = 0, imu660ra_acc_z = 0;               // ������ٶȼ����� acc  (accelerometer ���ٶȼ�)

#if IMU660RA_USE_SOFT_IIC

#define GET_IMU660RA_SDA   		 		IMU660RA_SDA_PIN
#define IMU660RA_SCL_LOW()          	IMU660RA_SCL_PIN = 0		//IO������͵�ƽ
#define IMU660RA_SCL_HIGH()         	IMU660RA_SCL_PIN = 1		//IO������ߵ�ƽ
#define IMU660RA_SDA_LOW()          	IMU660RA_SDA_PIN = 0		//IO������͵�ƽ
#define IMU660RA_SDA_HIGH()         	IMU660RA_SDA_PIN = 1		//IO������ߵ�ƽ


#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʱ
//  @return     void						
//  @since      v1.0
//  Sample usage:				���IICͨѶʧ�ܿ��Գ�������j��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simiic_delay(void)
{
    uint16 j=IMU660RA_SOFT_IIC_DELAY;   
	while(j--);
}

//�ڲ�ʹ�ã��û��������
static void imu660ra_simiic_start(void)
{
	IMU660RA_SDA_HIGH();
	IMU660RA_SCL_HIGH();
	imu660ra_simiic_delay();
	IMU660RA_SDA_LOW();
	imu660ra_simiic_delay();
	IMU660RA_SCL_LOW();
}

//�ڲ�ʹ�ã��û��������
static void imu660ra_simiic_stop(void)
{
	IMU660RA_SDA_LOW();
	IMU660RA_SCL_LOW();
	imu660ra_simiic_delay();
	IMU660RA_SCL_HIGH();
	imu660ra_simiic_delay();
	IMU660RA_SDA_HIGH();
	imu660ra_simiic_delay();
}

//��Ӧ��(����ack:SDA=0��no_ack:SDA=0)
//�ڲ�ʹ�ã��û��������
static void imu660ra_simiic_sendack(unsigned char ack_dat)
{
    IMU660RA_SCL_LOW();
	imu660ra_simiic_delay();
	if(ack_dat) IMU660RA_SDA_LOW();
    else    	IMU660RA_SDA_HIGH();

    IMU660RA_SCL_HIGH();
    imu660ra_simiic_delay();
    IMU660RA_SCL_LOW();
    imu660ra_simiic_delay();
}


static int imu660ra_sccb_waitack(void)
{
    IMU660RA_SCL_LOW();

	imu660ra_simiic_delay();
	
	IMU660RA_SCL_HIGH();
    imu660ra_simiic_delay();
	
    if(GET_IMU660RA_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {

        IMU660RA_SCL_LOW();
        return 0;
    }

    IMU660RA_SCL_LOW();
	imu660ra_simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
static void imu660ra_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	IMU660RA_SDA_HIGH();//SDA �������
        else			IMU660RA_SDA_LOW();
        c <<= 1;
        imu660ra_simiic_delay();
        IMU660RA_SCL_HIGH();                //SCL ���ߣ��ɼ��ź�
        imu660ra_simiic_delay();
        IMU660RA_SCL_LOW();                //SCL ʱ��������
    }
	imu660ra_sccb_waitack();
}


//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|ʹ��
//�ڲ�ʹ�ã��û��������
static uint8 imu660ra_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    IMU660RA_SCL_LOW();
    imu660ra_simiic_delay();
    IMU660RA_SDA_HIGH();             

    for(i=0;i<8;i++)
    {
        imu660ra_simiic_delay();
        IMU660RA_SCL_LOW();         //��ʱ����Ϊ�ͣ�׼����������λ
        imu660ra_simiic_delay();
        IMU660RA_SCL_HIGH();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        imu660ra_simiic_delay();
        c<<=1;
        if(GET_IMU660RA_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
        }
    }

	IMU660RA_SCL_LOW();
	imu660ra_simiic_delay();
	imu660ra_simiic_sendack(ack_x);
	
    return c;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	imu660ra_simiic_start();
    imu660ra_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	imu660ra_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	imu660ra_send_ch( dat );   				 //������Ҫд�������
	imu660ra_simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simiic_write_regs(uint8 dev_add, uint8 reg, uint8 *dat, uint32 len)
{
	uint16 i = 0;
	imu660ra_simiic_start();
    imu660ra_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	imu660ra_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ

	while(len--)
	{
		imu660ra_send_ch( *dat++ );   				 //������Ҫд�������
	}

	
	imu660ra_simiic_stop();
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC���豸�Ĵ�����ȡ����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660ra_simiic_read_reg(uint8 dev_add, uint8 reg)
{
	uint8 dat;
	imu660ra_simiic_start();
    imu660ra_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	imu660ra_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	
	imu660ra_simiic_start();
	imu660ra_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = imu660ra_read_ch(no_ack);   				//��ȡ����
	imu660ra_simiic_stop();
	
	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat_add			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint32 num)
{
	imu660ra_simiic_start();
    imu660ra_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	imu660ra_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	
	imu660ra_simiic_start();
	imu660ra_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = imu660ra_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = imu660ra_read_ch(no_ack); //��ȡ����
	imu660ra_simiic_stop();
}

#define imu660ra_write_register(reg, dat)        (imu660ra_simiic_write_reg(IMU660RA_DEV_ADDR, (reg), (dat)))
#define imu660ra_write_registers(reg, dat, len)  (imu660ra_simiic_write_regs(IMU660RA_DEV_ADDR, (reg), (dat), (len)))
#define imu660ra_read_register(reg)              (imu660ra_simiic_read_reg(IMU660RA_DEV_ADDR, (reg)))
#define imu660ra_read_registers(reg, dat, len)   (imu660ra_simiic_read_regs(IMU660RA_DEV_ADDR, (reg), (dat), (len)))

#else


#define IMU660RA_SCK(x)				IMU660RA_SPC_PIN  = x
#define IMU660RA_MOSI(x) 			IMU660RA_SDI_PIN = x
#define IMU660RA_CS(x)  			IMU660RA_CS_PIN  = x
#define IMU660RA_MISO    			IMU660RA_SDO_PIN 


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
//  @param      byte        ���͵�����    
//  @return     uint8       return ����status״̬
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660ra_simspi_wr_byte(uint8 byte)
{
    uint8 i;
	
    for(i=0; i<8; i++)
    {
        IMU660RA_MOSI(byte&0x80);
        byte <<= 1;
		IMU660RA_SCK (0);
		IMU660RA_SCK (0);
		IMU660RA_SCK (1);
		IMU660RA_SCK (1);
		
		byte |= IMU660RA_MISO; 
    }	
    return(byte);                                      		
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simspi_w_reg_byte(uint8 cmd, uint8 val)
{
    cmd |= IMU660RA_SPI_W;
    imu660ra_simspi_wr_byte(cmd);                      	
    imu660ra_simspi_wr_byte(val);                               	                                 	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simspi_w_reg_bytes(uint8 cmd, uint8 *dat_addr, uint32 len)
{
    cmd |= IMU660RA_SPI_W;
    imu660ra_simspi_wr_byte(cmd);   
	while(len--)
	{
		imu660ra_simspi_wr_byte(*dat_addr++);   
	}                           	                                  	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
//static void imu660ra_simspi_r_reg_byte(uint8 cmd, uint8 *val)
//{
//    IMU660RA_CS(0);
//    cmd |= IMU660RA_SPI_R;
//    imu660ra_simspi_wr_byte(cmd);                               	
//    *val = imu660ra_simspi_wr_byte(0);                           	
//    IMU660RA_CS(1);                                    	
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @param      num         ��ȡ������
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_simspi_r_reg_bytes(uint8 cmd, uint8 *val, uint32 num)
{
    cmd |= IMU660RA_SPI_R;
    imu660ra_simspi_wr_byte(cmd);
	
	while(num--)
	{
		*val++ = imu660ra_simspi_wr_byte(0);
	}
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RA д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ����
// ���ز���     void
// ʹ��ʾ��     imu660ra_write_register(IMU660RA_PWR_CONF, 0x00);                   // �رո߼�ʡ��ģʽ
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_write_register(uint8 reg, uint8 dat)
{
    IMU660RA_CS(0);
    imu660ra_simspi_w_reg_byte(reg | IMU660RA_SPI_W, dat);
    IMU660RA_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RA д����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ����
// ���ز���     void
// ʹ��ʾ��     imu660ra_write_registers(IMU660RA_INIT_dat, imu660ra_config_file, sizeof(imu660ra_config_file));
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_write_registers(uint8 reg, const uint8 *dat, uint32 len)
{
    IMU660RA_CS(0);

    imu660ra_simspi_w_reg_bytes(reg | IMU660RA_SPI_W, dat, len);
    IMU660RA_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RA ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu660ra_read_register(IMU660RA_CHIP_ID);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660ra_read_register(uint8 reg)
{
    uint8 dat[2];
    IMU660RA_CS(0);
    imu660ra_simspi_r_reg_bytes(reg | IMU660RA_SPI_R, dat, 2);
    IMU660RA_CS(1);
    return dat[1];
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RA ������
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu660ra_read_registers(IMU660RA_ACC_ADDRESS, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660ra_read_registers(uint8 reg, uint8 *dat, uint32 len)
{
    IMU660RA_CS(0);
    imu660ra_simspi_r_reg_bytes(reg | IMU660RA_SPI_R, dat, len);
	IMU660RA_CS(1);
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RA �Լ�
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu660ra_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660ra_self_check (void)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;
    do
    {
        if(timeout_count ++ > IMU660RA_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
        dat = imu660ra_read_register(IMU660RA_CHIP_ID);
        delay_ms(1);
    }while(0x24 != dat);                                                        // ��ȡ�豸ID�Ƿ����0X24���������0X24����Ϊû��⵽�豸
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RA ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660ra_get_acc();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us        �ɼ����ٶȼƵ�ʱ����ɼ������ǵ�ʱ��һ�µ�ԭ���Ƕ�ֻ�Ƕ�ȡ�Ĵ�������
//-------------------------------------------------------------------------------------------------------------------
void imu660ra_get_acc (void)
{
#if IMU660RA_USE_SOFT_IIC
	uint8 dat[6];
    imu660ra_read_registers(IMU660RA_ACC_ADDRESS, dat, 6);
    imu660ra_acc_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660ra_acc_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660ra_acc_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
#else
	// SPI��ȡ��һ����ַΪ��
	uint8 dat[7];
	imu660ra_read_registers(IMU660RA_ACC_ADDRESS, dat, 7);
    imu660ra_acc_x = (int16)(((uint16)dat[2]<<8 | dat[1]));
    imu660ra_acc_y = (int16)(((uint16)dat[4]<<8 | dat[3]));
    imu660ra_acc_z = (int16)(((uint16)dat[6]<<8 | dat[5]));
#endif
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RA ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660ra_get_gyro();                                            // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us
//-------------------------------------------------------------------------------------------------------------------
void imu660ra_get_gyro (void)
{
#if IMU660RA_USE_SOFT_IIC
    uint8 dat[6];
    imu660ra_read_registers(IMU660RA_GYRO_ADDRESS, dat, 6);
    imu660ra_gyro_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660ra_gyro_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660ra_gyro_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
#else
	// SPI��ȡ��һ����ַΪ��
	uint8 dat[7];
	imu660ra_read_registers(IMU660RA_GYRO_ADDRESS, dat, 7);
    imu660ra_gyro_x = (int16)(((uint16)dat[2]<<8 | dat[1]));
    imu660ra_gyro_y = (int16)(((uint16)dat[4]<<8 | dat[3]));
    imu660ra_gyro_z = (int16)(((uint16)dat[6]<<8 | dat[5]));
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RA ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     gyro_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float dat = imu660ra_acc_transition(imu660ra_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660ra_acc_transition (int16 acc_value)
{
    float acc_dat = 0;
    switch((uint8)IMU660RA_ACC_SAMPLE)
    {
        case 0x00: acc_dat = (float)acc_value / 16384; break;                  // 0x00 ���ٶȼ�����Ϊ:��2g     ��ȡ���ļ��ٶȼ����� ���� 16384     ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        case 0x01: acc_dat = (float)acc_value / 8192; break;                   // 0x01 ���ٶȼ�����Ϊ:��4g     ��ȡ���ļ��ٶȼ����� ���� 8192      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        case 0x02: acc_dat = (float)acc_value / 4096; break;                   // 0x02 ���ٶȼ�����Ϊ:��8g     ��ȡ���ļ��ٶȼ����� ���� 4096      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        case 0x03: acc_dat = (float)acc_value / 2048; break;                   // 0x03 ���ٶȼ�����Ϊ:��16g    ��ȡ���ļ��ٶȼ����� ���� 2048      ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        default: break;
    }
    return acc_dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RA ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float dat = imu660ra_gyro_transition(imu660ra_gyro_x);         // ��λΪ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660ra_gyro_transition (int16 gyro_value)
{
    float gyro_dat = 0;
    switch(IMU660RA_GYR_SAMPLE)
    {
        case 0x00: gyro_dat = (float)gyro_value / 16.4f;  break;               //  0x00 ����������Ϊ:��2000dps    ��ȡ�������������ݳ��� 16.4    ����ת��Ϊ������λ������ ��λΪ����/s
        case 0x01: gyro_dat = (float)gyro_value / 32.8f;  break;               //  0x01 ����������Ϊ:��1000dps    ��ȡ�������������ݳ��� 32.8    ����ת��Ϊ������λ������ ��λΪ����/s
        case 0x02: gyro_dat = (float)gyro_value / 65.6f;  break;               //  0x02 ����������Ϊ:��500 dps    ��ȡ�������������ݳ��� 65.6    ����ת��Ϊ������λ������ ��λΪ����/s
        case 0x03: gyro_dat = (float)gyro_value / 131.2f; break;               //  0x03 ����������Ϊ:��250 dps    ��ȡ�������������ݳ��� 131.2   ����ת��Ϊ������λ������ ��λΪ����/s
        case 0x04: gyro_dat = (float)gyro_value / 262.4f; break;               //  0x04 ����������Ϊ:��125 dps    ��ȡ�������������ݳ��� 262.4   ����ת��Ϊ������λ������ ��λΪ����/s
        default: break;
    }
    return gyro_dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� IMU660RA
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     imu660ra_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660ra_init (void)
{
    uint8 return_state = 0;
	
    delay_ms(20);                                                        		// �ȴ��豸�ϵ�ɹ�

//#if IMU660RA_USE_SOFT_IIC 

//#else
//	imu660ra_read_register(IMU660RA_CHIP_ID);                                   // ��ȡһ���豸ID ���豸����ΪSPIģʽ
//	imu660ra_read_register(IMU660RA_CHIP_ID);                                   // ��ȡһ���豸ID ���豸����ΪSPIģʽ
//	imu660ra_read_register(IMU660RA_CHIP_ID);                                   // ��ȡһ���豸ID ���豸����ΪSPIģʽ

//#endif
	
    do{
        if(imu660ra_self_check())                                               // IMU660RA �Լ�
        {
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� IMU660RA �Լ������ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���
			while(1)
			{
				printf("imu660ra self check error.\r\n");
				delay_ms(200);
			};
            return_state = 1;
            //break;
        }

        imu660ra_write_register(IMU660RA_PWR_CONF, 0x00);                       // �رո߼�ʡ��ģʽ
        delay_ms(10);
        imu660ra_write_register(IMU660RA_INIT_CTRL, 0x00);                      // ��ʼ��ģ����г�ʼ������
        imu660ra_write_registers(IMU660RA_INIT_DATA, imu660ra_config_file, sizeof(imu660ra_config_file));   // ��������ļ�
        imu660ra_write_register(IMU660RA_INIT_CTRL, 0x01);                      // ��ʼ�����ý���
        delay_ms(20);
        if(imu660ra_read_register(IMU660RA_INT_STA) != 1)                       // ����Ƿ��������
        {
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� IMU660RA ���ó�ʼ���ļ�������
            // ���IMU660RA��ʼ��֮ǰ���Ƿ���˶�ʱ���жϣ��ڶ�ʱ���ж�����ɼ���660RA������
//			while(1)
//			{
				printf("imu660ra init error.\r\n");
//				delay_ms(200);
//			}
            return_state = 1;
            //break;
        }
        imu660ra_write_register(IMU660RA_PWR_CTRL, 0x0E);                       // ��������ģʽ  ʹ�������ǡ����ٶȡ��¶ȴ�����
        imu660ra_write_register(IMU660RA_ACC_CONF, 0xA7);                       // ���ٶȲɼ����� ����ģʽ �����ɼ� 50Hz  ����Ƶ��
        imu660ra_write_register(IMU660RA_GYR_CONF, 0xA9);                       // �����ǲɼ����� ����ģʽ �����ɼ� 200Hz ����Ƶ��
		
        imu660ra_write_register(IMU660RA_ACC_RANGE, IMU660RA_ACC_SAMPLE);       // ���ٶ��������� ��������Ϊ:��8g
		// IMU660RA_ACC_SAMPLE �Ĵ���
        // ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g         ��ȡ���ļ��ٶȼ����� ���� 16384   ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        // ����Ϊ:0x01 ���ٶȼ�����Ϊ:��4g         ��ȡ���ļ��ٶȼ����� ���� 8192    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        // ����Ϊ:0x02 ���ٶȼ�����Ϊ:��8g         ��ȡ���ļ��ٶȼ����� ���� 4096    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
        // ����Ϊ:0x03 ���ٶȼ�����Ϊ:��16g        ��ȡ���ļ��ٶȼ����� ���� 2048    ����ת��Ϊ������λ������ ��λ��g(m/s^2)
		
        imu660ra_write_register(IMU660RA_GYR_RANGE, IMU660RA_GYR_SAMPLE);       // �������������� ��������Ϊ:��2000dps
        // IMU660RA_GYR_RANGE �Ĵ���
        // ����Ϊ:0x00 ����������Ϊ:��2000dps     ��ȡ�������������� ���� 16.4       ����ת��Ϊ������λ������ ��λΪ����/s
        // ����Ϊ:0x01 ����������Ϊ:��1000dps     ��ȡ�������������� ���� 32.8       ����ת��Ϊ������λ������ ��λΪ����/s
        // ����Ϊ:0x02 ����������Ϊ:��500 dps     ��ȡ�������������� ���� 65.6       ����ת��Ϊ������λ������ ��λΪ����/s
        // ����Ϊ:0x03 ����������Ϊ:��250 dps     ��ȡ�������������� ���� 131.2      ����ת��Ϊ������λ������ ��λΪ����/s
        // ����Ϊ:0x04 ����������Ϊ:��125 dps     ��ȡ�������������� ���� 262.4      ����ת��Ϊ������λ������ ��λΪ����/s
		

	
	}while(0);
	

	
    return return_state;
}



