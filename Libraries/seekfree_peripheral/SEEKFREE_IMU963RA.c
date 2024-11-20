/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		IMU963RA
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
 *                   SCL/SPC             �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_SPC_PIN �궨��
 *                   SDA/DSI             �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_SDI_PIN �궨��
 *                   SA0/SDO             �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_SDO_PIN �궨��
 *                   CS                  �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_CS_PIN  �궨��
 *                   VCC                 3.3V��Դ
 *                   GND                 ��Դ��
 *                   ������������
 *
 *                   // ��� IIC ����
 *                   SCL/SPC             �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_SCL_PIN �궨��
 *                   SDA/DSI             �鿴 SEEKFREE_IMU963RA.h �� IMU963RA_SDA_PIN �궨��
 *                   VCC                 3.3V��Դ
 *                   GND                 ��Դ��
 *                   ������������
 *                   ------------------------------------
********************************************************************************************************************/

#include "SEEKFREE_IMU963RA.h"

#include "zf_delay.h"
#include "zf_spi.h"



#pragma warning disable = 177
#pragma warning disable = 183


int16 imu963ra_gyro_x = 0, imu963ra_gyro_y = 0, imu963ra_gyro_z = 0;
int16 imu963ra_acc_x = 0,  imu963ra_acc_y = 0,  imu963ra_acc_z = 0;
int16 imu963ra_mag_x = 0,  imu963ra_mag_y = 0,  imu963ra_mag_z = 0;

#if IMU963RA_USE_SOFT_IIC


#define GET_IMU963RA_SDA   		 	IMU963RA_SDA_PIN
#define IMU963RA_SDA_LOW()         	IMU963RA_SDA_PIN = 0		//IO������͵�ƽ
#define IMU963RA_SDA_HIGH()         IMU963RA_SDA_PIN = 1		//IO������ߵ�ƽ

#define IMU963RA_SCL_LOW()          IMU963RA_SCL_PIN = 0		//IO������͵�ƽ
#define IMU963RA_SCL_HIGH()         IMU963RA_SCL_PIN = 1		//IO������ߵ�ƽ

#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʱ
//  @return     void						
//  @since      v1.0
//  Sample usage:				���IICͨѶʧ�ܿ��Գ�������j��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_simiic_delay(void)
{
    uint16 j=IMU963RA_SOFT_IIC_DELAY;   
	while(j--);
}

//�ڲ�ʹ�ã��û��������
static void imu963ra_simiic_start(void)
{
	IMU963RA_SDA_HIGH();
	IMU963RA_SCL_HIGH();
	imu963ra_simiic_delay();
	IMU963RA_SDA_LOW();
	imu963ra_simiic_delay();
	IMU963RA_SCL_LOW();
}

//�ڲ�ʹ�ã��û��������
static void imu963ra_simiic_stop(void)
{
	IMU963RA_SDA_LOW();
	IMU963RA_SCL_LOW();
	imu963ra_simiic_delay();
	IMU963RA_SCL_HIGH();
	imu963ra_simiic_delay();
	IMU963RA_SDA_HIGH();
	imu963ra_simiic_delay();
}

//��Ӧ��(����ack:SDA=0��no_ack:SDA=0)
//�ڲ�ʹ�ã��û��������
static void imu963ra_simiic_sendack(unsigned char ack_dat)
{
    IMU963RA_SCL_LOW();
	imu963ra_simiic_delay();
	if(ack_dat) IMU963RA_SDA_LOW();
    else    	IMU963RA_SDA_HIGH();

    IMU963RA_SCL_HIGH();
    imu963ra_simiic_delay();
    IMU963RA_SCL_LOW();
    imu963ra_simiic_delay();
}


static int imu963ra_sccb_waitack(void)
{
    IMU963RA_SCL_LOW();

	imu963ra_simiic_delay();
	
	IMU963RA_SCL_HIGH();
    imu963ra_simiic_delay();
	
    if(GET_IMU963RA_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {

        IMU963RA_SCL_LOW();
        return 0;
    }

    IMU963RA_SCL_LOW();
	imu963ra_simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
static void imu963ra_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	IMU963RA_SDA_HIGH();//SDA �������
        else			IMU963RA_SDA_LOW();
        c <<= 1;
        imu963ra_simiic_delay();
        IMU963RA_SCL_HIGH();                //SCL ���ߣ��ɼ��ź�
        imu963ra_simiic_delay();
        IMU963RA_SCL_LOW();                //SCL ʱ��������
    }
	imu963ra_sccb_waitack();
}


//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|ʹ��
//�ڲ�ʹ�ã��û��������
static uint8 imu963ra_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    IMU963RA_SCL_LOW();
    imu963ra_simiic_delay();
    IMU963RA_SDA_HIGH();             

    for(i=0;i<8;i++)
    {
        imu963ra_simiic_delay();
        IMU963RA_SCL_LOW();         //��ʱ����Ϊ�ͣ�׼����������λ
        imu963ra_simiic_delay();
        IMU963RA_SCL_HIGH();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        imu963ra_simiic_delay();
        c<<=1;
        if(GET_IMU963RA_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
        }
    }

	IMU963RA_SCL_LOW();
	imu963ra_simiic_delay();
	imu963ra_simiic_sendack(ack_x);
	
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
static void imu963ra_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	imu963ra_simiic_start();
    imu963ra_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	imu963ra_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	imu963ra_send_ch( dat );   				 //������Ҫд�������
	imu963ra_simiic_stop();
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
//static uint8 imu963ra_simiic_read_reg(uint8 dev_add, uint8 reg)
//{
//	uint8 dat;
//	imu963ra_simiic_start();
//    imu963ra_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
//	imu963ra_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ

//	
//	imu963ra_simiic_start();
//	imu963ra_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
//	dat = imu963ra_read_ch(no_ack);   				//��ȡ����
//	imu963ra_simiic_stop();
//	
//	return dat;
//}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC���豸�Ĵ�����ȡ����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_simiic_sccb_read_reg(uint8 dev_add, uint8 reg)
{
	uint8 dat;
	imu963ra_simiic_start();
    imu963ra_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	imu963ra_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ
	imu963ra_simiic_stop();
	
	imu963ra_simiic_start();
	imu963ra_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = imu963ra_read_ch(no_ack);   				//��ȡ����
	imu963ra_simiic_stop();
	
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
static void imu963ra_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint32 num)
{
	imu963ra_simiic_start();
    imu963ra_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	imu963ra_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ

	
	imu963ra_simiic_start();
	imu963ra_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = imu963ra_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = imu963ra_read_ch(no_ack); //��ȡ����
	imu963ra_simiic_stop();
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ����
// ���ز���     void
// ʹ��ʾ��     imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x00);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu963ra_write_acc_gyro_register(reg,dat)       (imu963ra_simiic_write_reg(IMU963RA_DEV_ADDR,reg,dat))

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu963ra_read_acc_gyro_register(reg)             (imu963ra_simiic_sccb_read_reg(IMU963RA_DEV_ADDR,reg))

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ������ �ڲ�����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_A, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu963ra_read_acc_gyro_registers(reg,dat,len)   (imu963ra_simiic_read_regs(IMU963RA_DEV_ADDR,reg,dat,len))


#else



#define IMU963RA_SCK(x)				IMU963RA_SPC_PIN  = x
#define IMU963RA_MOSI(x) 			IMU963RA_SDI_PIN = x
#define IMU963RA_CS(x)  			IMU963RA_CS_PIN  = x
#define IMU963RA_MISO    			IMU963RA_SDO_PIN 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
//  @param      byte        ���͵�����    
//  @return     uint8       return ����status״̬
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_simspi_wr_byte(uint8 byte)
{
    uint8 i;
	
    for(i=0; i<8; i++)
    {
        IMU963RA_MOSI(byte&0x80);
        byte <<= 1;
		IMU963RA_SCK (0);
		IMU963RA_SCK (0);
		
		IMU963RA_SCK (1);
		IMU963RA_SCK (1);
		byte |= IMU963RA_MISO; 
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
static void imu963ra_simspi_w_reg_byte(uint8 cmd, uint8 val)
{
    IMU963RA_CS(0);
    cmd |= IMU963RA_SPI_W;
    imu963ra_simspi_wr_byte(cmd);                      	
    imu963ra_simspi_wr_byte(val);                               	
    IMU963RA_CS(1);                                    	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
//static void imu963ra_simspi_w_reg_bytes(uint8 cmd, uint8 *dat_addr, uint32 len)
//{

//	
//    IMU963RA_CS(0);
//    cmd |= IMU963RA_SPI_W;
//    imu963ra_simspi_wr_byte(cmd);   
//	while(len--)
//	{
//		imu963ra_simspi_wr_byte(*dat_addr++); 
//	}                	
//    IMU963RA_CS(1);                                    	
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_simspi_r_reg_byte(uint8 cmd, uint8 *val)
{
    IMU963RA_CS(0);
    cmd |= IMU963RA_SPI_R;
    imu963ra_simspi_wr_byte(cmd);                               	
    *val = imu963ra_simspi_wr_byte(0);                           	
    IMU963RA_CS(1);                                    	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @param      num         ��ȡ������
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_simspi_r_reg_bytes(uint8 cmd, uint8 *val, uint32 num)
{

    cmd |= IMU963RA_SPI_R;
    imu963ra_simspi_wr_byte(cmd);

	while(num--)
	{
		*val++ = imu963ra_simspi_wr_byte(0);
	}          
}



//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ����
// ���ز���     void
// ʹ��ʾ��     imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x00);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_write_acc_gyro_register(uint8 reg, uint8 dat)
{
    IMU963RA_CS(0);
    imu963ra_simspi_w_reg_byte(reg | IMU963RA_SPI_W, dat);

    IMU963RA_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_read_acc_gyro_register(uint8 reg)
{
    uint8 dat = 0;
    IMU963RA_CS(0);
	imu963ra_simspi_r_reg_byte(reg | IMU963RA_SPI_R , &dat);
    IMU963RA_CS(1);
    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ������ �ڲ�����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     dat            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_A, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_read_acc_gyro_registers(uint8 reg, uint8 *dat, uint32 len)
{
    IMU963RA_CS(0);
    imu963ra_simspi_r_reg_bytes( reg | IMU963RA_SPI_R, dat, len);

    IMU963RA_CS(1);
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ��Ϊ IIC �����������д����
// ����˵��     addr            Ŀ���ַ
// ����˵��     reg             Ŀ��Ĵ���
// ����˵��     dat            ����
// ���ز���     uint8           1-ʧ�� 0-�ɹ�
// ʹ��ʾ��     imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x80);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_write_mag_register (uint8 addr, uint8 reg, uint8 dat)
{
    uint8 return_state = 0;
    uint16 timeout_count = 0;

    addr = addr << 1;
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x00);               // �ӻ�0�������
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 0);              // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);                // ��Ҫд��ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_DATAWRITE_SLV0, dat);            // ��Ҫд�������
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x4C);             // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
    
    // �ȴ�ͨѶ�ɹ�
    while(0 == (0x80 & imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER)))
    {
        if(timeout_count ++ > IMU963RA_TIMEOUT_COUNT)
        {
            return_state = 1;
            break;
        }
        delay_ms(2);
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ��Ϊ IIC ����������ƶ�����
// ����˵��     addr            Ŀ���ַ
// ����˵��     reg             Ŀ��Ĵ���
// ���ز���     uint8           ��ȡ������
// ʹ��ʾ��     imu963ra_read_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_read_mag_register (uint8 addr, uint8 reg)
{
    uint16 timeout_count = 0;

    addr = addr << 1;
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 1);              // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);                // ��Ҫ��ȡ�ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x01);    
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x4C);             // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
    
    // �ȴ�ͨѶ�ɹ�
    while(0 == (0x01 & imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER)))
    {
        if(timeout_count ++ > IMU963RA_TIMEOUT_COUNT)
        {
            break;
        }
        delay_ms(2);
    }
    
    return (imu963ra_read_acc_gyro_register(IMU963RA_SENSOR_HUB_1));            // ���ض�ȡ��������
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA ��Ϊ IIC ������������Զ�д����
// ����˵��     addr            Ŀ���ַ
// ����˵��     reg             Ŀ��Ĵ���
// ���ز���     void
// ʹ��ʾ��     imu963ra_connect_mag(IMU963RA_MAG_ADDR, IMU963RA_MAG_OUTX_L);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_connect_mag (uint8 addr, uint8 reg)
{
    addr = addr << 1;
    
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 1);              // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);                // ��Ҫ��ȡ�ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x06);    
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x6C);             // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
}   


//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA �����Լ� �ڲ�����
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu963ra_acc_gyro_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_acc_gyro_self_check (void)
{
    uint8 return_state = 0;
    uint8 dat = 0;
    uint16 timeout_count = 0;

    while(0x6B != dat)                                                          // �ж� ID �Ƿ���ȷ
    {
        if(timeout_count++ > IMU963RA_TIMEOUT_COUNT)
        {
            return_state = 1;
            break;
        }
        dat = imu963ra_read_acc_gyro_register(IMU963RA_WHO_AM_I);
        delay_ms(10);
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU963RA �������Լ� �ڲ�����
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu963ra_mag_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_mag_self_check (void)
{
    uint8 return_state = 0;
    uint8 dat = 0;
    uint16 timeout_count = 0;

    while(0xff != dat)                                                          // �ж� ID �Ƿ���ȷ
    {
        if(timeout_count++ > IMU963RA_TIMEOUT_COUNT)
        {
            return_state = 1;
            break;
        }
        dat = imu963ra_read_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID);
        delay_ms(10);
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU963RA ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu963ra_get_acc();
// ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_acc (void)
{
    uint8 dat[6];

    imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_A, dat, 6);
    imu963ra_acc_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu963ra_acc_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu963ra_acc_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡIMU963RA����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu963ra_get_gyro();
// ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_gyro (void)
{
    uint8 dat[6];

    imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_G, dat, 6);
    imu963ra_gyro_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu963ra_gyro_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu963ra_gyro_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU963RA ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu963ra_get_mag();
// ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_mag (void)
{
    uint8 temp_status;
    uint8 dat[6];

    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x40);
    temp_status = imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER);
    if(0x01 & temp_status)
    {
        imu963ra_read_acc_gyro_registers(IMU963RA_SENSOR_HUB_1, dat, 6);
        imu963ra_mag_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
        imu963ra_mag_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
        imu963ra_mag_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
    }
    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU963RA ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     gyro_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float dat = imu963ra_acc_transition(imu963ra_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu963ra_acc_transition (int16 acc_value)
{
    float acc_dat = 0;
    switch(IMU963RA_ACC_SAMPLE)
    {
        case 0x30: acc_dat = (float)acc_value / 16393; break;                  // 0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ���� 16393 ������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x38: acc_dat = (float)acc_value / 8197;  break;                  // 0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ���� 8197 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x3C: acc_dat = (float)acc_value / 4098;  break;                  // 0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ���� 4098 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x34: acc_dat = (float)acc_value / 2049;  break;                  // 0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ���� 2049 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        default: break;
    }
    return acc_dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU963RA ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float dat = imu963ra_gyro_transition(imu963ra_gyro_x);         // ��λΪ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu963ra_gyro_transition (int16 gyro_value)
{
    float gyro_dat = 0;
    switch(IMU963RA_GYR_SAMPLE)
    {
        case 0x52: gyro_dat = (float)gyro_value / 228.6f;  break;              //  0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ��� 228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x50: gyro_dat = (float)gyro_value / 114.3f;  break;              //  0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ��� 114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x54: gyro_dat = (float)gyro_value / 57.1f;   break;              //  0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ��� 57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x58: gyro_dat = (float)gyro_value / 28.6f;   break;              //  0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ��� 28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x5C: gyro_dat = (float)gyro_value / 14.3f;   break;              //  0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ��� 14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x51: gyro_dat = (float)gyro_value / 7.1f;    break;              //  0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ��� 7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s
        default: break;
    }
    return gyro_dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU963RA �شż�����ת��Ϊʵ����������
// ����˵��     mag_value       ������ĵشż�����
// ���ز���     void
// ʹ��ʾ��     float dat = imu963ra_mag_transition(imu963ra_mag_x);          // ��λΪG(��˹)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu963ra_mag_transition (int16 mag_value)
{
    float mag_dat = 0;
    switch(IMU963RA_MAG_SAMPLE)
    {
        case 0x19: mag_dat = (float)mag_value / 3000;  break;                  //  0x19 ����������Ϊ:8G     ��ȡ���ļ��ٶȼ����� ����3000�� ����ת��Ϊ������λ�����ݣ���λ��G(��˹)
        case 0x09: mag_dat = (float)mag_value / 12000; break;                  //  0x09 ����������Ϊ:2G     ��ȡ���ļ��ٶȼ����� ����12000������ת��Ϊ������λ�����ݣ���λ��G(��˹)
        default: break;
    }
    return mag_dat;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� IMU963RA
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     imu963ra_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_init (void)
{
    uint8 return_state = 0;
    delay_ms(10);                                                        // �ϵ���ʱ

//#if IMU963RA_USE_SOFT_IIC
//    soft_iic_init(&imu963ra_iic_struct, IMU963RA_DEV_ADDR, IMU963RA_SOFT_IIC_DELAY, IMU963RA_SCL_PIN, IMU963RA_SDA_PIN);
//#else
//    spi_init(IMU963RA_SPI, SPI_MODE0, IMU963RA_SPI_SPEED, IMU963RA_SPC_PIN, IMU963RA_SDI_PIN, IMU963RA_SDO_PIN, SPI_CS_NULL);
//    gpio_init(IMU963RA_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
//#endif

    do
    {
        imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);       // �ر�HUB�Ĵ�������
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL3_C, 0x01);               // ��λ�豸
        delay_ms(2);                             
        imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);       // �ر�HUB�Ĵ�������
        if(imu963ra_acc_gyro_self_check())                   
        {                
//			while(1)
//			{
				printf( "IMU963RA acc and gyro self check error.\r\n");                    
//				delay_ms(200);
//			};
			
            
            return_state = 1;
            break;            
        }                   
                            
        imu963ra_write_acc_gyro_register(IMU963RA_INT1_CTRL, 0x03);             // ���������� ���ٶ����ݾ����ж�
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL1_XL, IMU963RA_GYR_SAMPLE);   // ���ü��ٶȼ����� ��8G �Լ������������ 52hz �Լ����ٶ���Ϣ�ӵ�һ���˲������
        // IMU963RA_CTRL1_XL �Ĵ���
        // ����Ϊ:0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ����16393������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ����8197�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ����4098�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ����2049�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL2_G, IMU963RA_ACC_SAMPLE);    // ���������Ǽ����� ��2000dps �Լ������������ 208hz
        // IMU963RA_CTRL2_G �Ĵ���
        // ����Ϊ:0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ���228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ���114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ���57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ���28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ���14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ���7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s
        
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL3_C, 0x44);               // ʹ�����������ֵ�ͨ�˲���
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL4_C, 0x02);               // ʹ�����ֵ�ͨ�˲���
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL5_C, 0x00);               // ���ٶȼ�����������������
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL6_C, 0x00);               // �������ٶȼƸ�����ģʽ �����ǵ�ͨ�˲� 133hz
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL7_G, 0x00);               // ���������Ǹ�����ģʽ �رո�ͨ�˲�
        imu963ra_write_acc_gyro_register(IMU963RA_CTRL9_XL, 0x01);              // �ر�I3C�ӿ�

        imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x40);       // ����HUB�Ĵ������� �������õشż�
        imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x80);         // ��λI2C����
        delay_ms(2);                             
        imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x00);         // �����λ��־
        delay_ms(2);
        
        imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x80);// ��λ���ӵ�����
        delay_ms(2);
        imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x00);
        delay_ms(2);
        
        
        if(imu963ra_mag_self_check())
        {
//			while(1)
//			{
				printf("IMU963RA mag self check error.\r\n");
//				delay_ms(200);
//			};
			
            
            return_state = 1;
            break;            
        }

        imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, IMU963RA_MAG_SAMPLE); // ���ô���������8G �������100hz ����ģʽ
        // IMU963RA_MAG_ADDR �Ĵ���
        // ����Ϊ:0x19 ����������Ϊ:8G     ��ȡ���ļ��ٶȼ����� ����3000�� ����ת��Ϊ������λ�����ݣ���λ��G(��˹)
        // ����Ϊ:0x09 ����������Ϊ:2G     ��ȡ���ļ��ٶȼ����� ����12000������ת��Ϊ������λ�����ݣ���λ��G(��˹)
        
        imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_FBR, 0x01);
        imu963ra_connect_mag(IMU963RA_MAG_ADDR, IMU963RA_MAG_OUTX_L);
        
        imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);       // �ر�HUB�Ĵ�������

        delay_ms(20);                                                    // �ȴ������ƻ�ȡ����
    }while(0);
    return return_state;
}
