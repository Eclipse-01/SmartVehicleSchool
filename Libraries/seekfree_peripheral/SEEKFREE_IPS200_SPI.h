/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		IPS200_SPI
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32F12K
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-15
 * @note		
					���߶��壺
					------------------------------------ 
					2.0��IPSģ��ܽ�       	��Ƭ���ܽ�
					SCL                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_SCL		�궨��     Ӳ��SPI���Ų��������л�
					SDA                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_SDA		�궨��     Ӳ��SPI���Ų��������л�
					RES                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_REST_PIN �궨��    
					DC                  	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_DC_PIN	�궨��  
					CS                  	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_CS		�궨��     Ӳ��SPI���Ų��������л�
					BL  					�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_BL_PIN	�궨��
					
					��Դ����
					VCC 3.3V��Դ
					GND ��Դ��
					���ֱ���320*240
					------------------------------------ 
 ********************************************************************************************************************/
#ifndef _SEEKRE_IPS200_h_
#define _SEEKRE_IPS200_h_

#include "common.h"
#include "board.h"


#define IPS200_SPIN_PIN     SPI_CH2           //����ʹ�õ�SPI��
#define	IPS200_SCL_PIN 		SPI_CH2_SCLK_P25	//����SPI_SCK����
#define	IPS200_SDA_PIN		SPI_CH2_MOSI_P23   //����SPI_MOSI����
#define	IPS200_SDA_IN_PIN	SPI_NULL_PIN   //����SPI_MISO����  IPS��Ļû��MISO���ţ�����������Ȼ��Ҫ���壬��spi�ĳ�ʼ��ʱ��Ҫʹ��
#define IPS200_RST_PIN  	P20
#define IPS200_DC_PIN 	 	P21   	//Һ������λ���Ŷ���
#define IPS200_CS_PIN    	P22   	//����SPI_CS����
#define IPS200_BL_PIN    	P27     //Һ���������Ŷ��� 


#define IPS200_RST(x)      	(IPS200_RST_PIN = x)
#define IPS200_DC(x)        (IPS200_DC_PIN = x)
#define IPS200_CS(x)        (IPS200_CS_PIN = x)
#define IPS200_BL(x)        (IPS200_BL_PIN = x)


#define IPS200_DEFAULT_DISPLAY_DIR      (IPS200_PORTAIT)                        // Ĭ�ϵ���ʾ����
#define IPS200_DEFAULT_PENCOLOR         (RED    )                        // Ĭ�ϵĻ�����ɫ
#define IPS200_DEFAULT_BGCOLOR          (WHITE  )                        // Ĭ�ϵı�����ɫ
#define IPS200_DEFAULT_DISPLAY_FONT     (IPS200_8X16_FONT)                      // Ĭ�ϵ�����ģʽ


typedef enum
{
    IPS200_TYPE_SPI,                                                            // SPI ����
    IPS200_TYPE_PARALLEL8,                                                      // ��������
}ips200_type_enum;

typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // ����ģʽ
    IPS200_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    IPS200_CROSSWISE                    = 2,                                    // ����ģʽ
    IPS200_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}ips200_dir_enum;

typedef enum
{
    IPS200_6X8_FONT                     = 0,                                    // 6x8      ����
    IPS200_8X16_FONT                    = 1,                                    // 8x16     ����
    IPS200_16X16_FONT                   = 2,                                    // 16x16    ���� Ŀǰ��֧��
}ips200_font_size_enum;

void    ips200_clear                    (void);
void    ips200_full                     (const uint16 color);
void    ips200_set_dir                  (ips200_dir_enum dir);
void    ips200_set_font                 (ips200_font_size_enum font);
void    ips200_set_color                (const uint16 pen, const uint16 bgcolor);
void    ips200_draw_point               (uint16 x, uint16 y, const uint16 color);
void    ips200_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips200_show_char                (uint16 x, uint16 y, const char dat);
void    ips200_show_string              (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int                 (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint                (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float               (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);


void    ips200_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    ips200_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);



void    ips200_init_spi                     ();

#endif
