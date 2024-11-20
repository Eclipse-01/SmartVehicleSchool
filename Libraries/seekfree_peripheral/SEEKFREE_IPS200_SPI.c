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
					2.0��IPSģ��ܽ�       ��Ƭ���ܽ�
					SCL                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_SCL		�궨��     Ӳ��SPI���Ų��������л�
					SDA                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_SDA		�궨��     Ӳ��SPI���Ų��������л�
					RES                 	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_RST_PIN �궨��    
					DC                  	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_DC_PIN	�궨��  
					CS                  	�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_CS		�궨��     Ӳ��SPI���Ų��������л�
					BL  					�鿴SEEKFREE_IPS200_SPI.h�ļ��ڵ�IPS200_BL_PIN	�궨��
					
					��Դ����
					VCC 3.3V��Դ
					GND ��Դ��
					���ֱ���320*240
					------------------------------------ 
 ********************************************************************************************************************/


#include "SEEKFREE_IPS200_SPI.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_FUNCTION.h"


#include "zf_spi.h"
#include "zf_delay.h"

static uint16 ips200_pencolor = IPS200_DEFAULT_PENCOLOR;
static uint16 ips200_bgcolor  = IPS200_DEFAULT_BGCOLOR;

static ips200_type_enum         ips200_display_type = IPS200_TYPE_SPI;
static ips200_dir_enum          ips200_display_dir  = IPS200_DEFAULT_DISPLAY_DIR;
static ips200_font_size_enum    ips200_display_font = IPS200_DEFAULT_DISPLAY_FONT;

static uint16 ips200_x_max = 240;
static uint16 ips200_y_max = 320;

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 SPI д 8bit ����
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     ips200_write_8bit_data_spi(command);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
void ips200_write_8bit_data_spi(uint8 dat)    
{
	spi_mosi(dat);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 SPI д 16bit ����
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     ips200_write_16bit_data_spi(dat);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
void ips200_write_16bit_data_spi(uint16 dat)   
{
    uint8 dat1[2];
    dat1[0] = dat >> 8;
    dat1[1] = (uint8)dat;
	
	spi_mosi(dat1[0]);	//д������  ��λ��ǰ  ��λ�ں�
	spi_mosi(dat1[1]);
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 д����
// ����˵��     command         ����
// ���ز���     void
// ʹ��ʾ��     ips200_write_command(0x2a);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_command (const uint8 command)
{

	IPS200_CS(1);
	IPS200_CS(0);
	IPS200_DC(0);
	ips200_write_8bit_data_spi(command);
	IPS200_DC(1);
	IPS200_CS(1);
	IPS200_CS(0);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��Һ����д 8bit ����
// ����˵��     dat             ����
// ���ز���     void
// ʹ��ʾ��     ips200_write_8bit_data(0x0C);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_8bit_data (const uint8 dat)
{
	IPS200_CS(0);
    IPS200_DC(1);
	ips200_write_8bit_data_spi(dat);
	IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��Һ����д 16bit ����
// ����˵��     dat             ����
// ���ز���     void
// ʹ��ʾ��     ips200_write_16bit_data(x1);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_16bit_data (const uint16 dat)
{
	IPS200_CS(0);
    IPS200_DC(1);
	ips200_write_16bit_data_spi(dat);
	IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     x1              ��ʼx������
// ����˵��     y1              ��ʼy������
// ����˵��     x2              ����x������
// ����˵��     y2              ����y������
// ���ز���     void
// ʹ��ʾ��     ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_set_region (uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{


    ips200_write_command(0x2a);
    ips200_write_16bit_data(x1);
    ips200_write_16bit_data(x2);
    
    ips200_write_command(0x2b);
    ips200_write_16bit_data(y1);
    ips200_write_16bit_data(y2);
    
    ips200_write_command(0x2c);
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     ips200_clear();
// ��ע��Ϣ     ����Ļ��ճɱ�����ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear (void)
{
    uint16 i, j;
	IPS200_CS(0);

    ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
    for(i = 0; i < ips200_x_max; i ++)
    {
        for (j = 0; j < ips200_y_max; j ++)
        {
            ips200_write_16bit_data(ips200_bgcolor);                 
        }
    }

	IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��Ļ��亯��
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips200_full(RGB565_BLACK);
// ��ע��Ϣ     ����Ļ����ָ����ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips200_full (const uint16 color)
{
    uint16 i, j;

	IPS200_CS(0);

    ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
    for(i = 0; i < ips200_x_max; i ++)
    {
        for (j = 0; j < ips200_y_max; j ++)
        {
            ips200_write_16bit_data(color);                 
        }
    }

	IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     dir             ��ʾ����  ���� zf_device_ips200.h �� ips200_dir_enum ö���嶨��
// ���ز���     void
// ʹ��ʾ��     ips200_set_dir(IPS200_PORTAIT);
// ��ע��Ϣ     �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_dir (ips200_dir_enum dir)
{
    ips200_display_dir = dir;
    if(dir < 2)
    {
        ips200_x_max = 240;
        ips200_y_max = 320;
    }
    else
    {
        ips200_x_max = 320;
        ips200_y_max = 240;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     dir             ��ʾ����  ���� zf_device_ips200.h �� ips200_font_size_enum ö���嶨��
// ���ز���     void
// ʹ��ʾ��     ips200_set_font(IPS200_8x16_FONT);
// ��ע��Ϣ     ���������ʱ�������� ���ú���Ч ������ʾ�����µ������С
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_font (ips200_font_size_enum font)
{
    ips200_display_font = font;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ��ɫ
// ����˵��     pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ����˵��     bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips200_set_color(RGB565_RED, RGB565_GRAY);
// ��ע��Ϣ     ������ɫ�ͱ�����ɫҲ������ʱ�������� ���ú���Ч
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_color (const uint16 pen, const uint16 bgcolor)
{
    ips200_pencolor = pen;
    ips200_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ����
// ����˵��     x               ����x�������� [0, ips200_x_max-1]
// ����˵��     y               ����y�������� [0, ips200_y_max-1]
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips200_draw_point(0, 0, RGB565_RED);            //����0,0��һ����ɫ�ĵ�
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_point (uint16 x, uint16 y, const uint16 color)
{


	IPS200_CS(0);

    ips200_set_region(x, y, x, y);
    ips200_write_16bit_data(color);

	IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ����
// ����˵��     x_start         ����x�������� [0, ips200_x_max-1]
// ����˵��     y_start         ����y�������� [0, ips200_y_max-1]
// ����˵��     x_end           ����x������յ� [0, ips200_x_max-1]
// ����˵��     y_end           ����y������յ� [0, ips200_y_max-1]
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips200_draw_line(0, 0, 10, 10, RGB565_RED);     // ���� 0,0 �� 10,10 ��һ����ɫ����
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_line (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{

    int16 x_dir = (x_start < x_end ? 1 : -1);
    int16 y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;

    do
    {
        if(x_start != x_end)
        {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        }
        else
        {
            while(y_start != y_end)
            {
                ips200_draw_point(x_start, y_start, color);
                y_start += y_dir;
            }
            break;
        }
		
        if((func_abs(y_start - y_end)) > (func_abs(x_start - x_end)))
        {
            while(y_start != y_end)
            {
                ips200_draw_point(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16)(((float)y_start - temp_b) / temp_rate);
            }
        }
        else
        {
            while(x_start != x_end)
            {
                ips200_draw_point(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16)((float)x_start * temp_rate + temp_b);
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ�ַ�
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ���ַ�
// ���ز���     void
// ʹ��ʾ��     ips200_show_char(0, 0, 'x');                     // ����0,0дһ���ַ�x
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_char (uint16 x, uint16 y, const char dat)
{
    uint8 i, j;
	uint8 temp_top;
	uint8 temp_bottom;

	IPS200_CS(0);

    switch(ips200_display_font)
    {
        case IPS200_6X8_FONT:
            for(i = 0; i < 6; i ++)
            {
                ips200_set_region(x + i, y, x + i, y + 8);
                // �� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
                temp_top = ascii_font_6x8[dat - 32][i];
                for(j = 0; j < 8; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        ips200_write_16bit_data(ips200_pencolor);
                    }
                    else
                    {
                        ips200_write_16bit_data(ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
            }
            break;
        case IPS200_8X16_FONT:
            for(i = 0; i < 8; i ++)
            {
                ips200_set_region(x + i, y, x + i, y + 15);
                // �� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
                temp_top = ascii_font_8x16[dat - 32][i];
                temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; j < 8; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        ips200_write_16bit_data(ips200_pencolor);
                    }
                    else
                    {
                        ips200_write_16bit_data(ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
                for(j = 0; j < 8; j ++)
                {
                    if(temp_bottom & 0x01)
                    {
                        ips200_write_16bit_data(ips200_pencolor);
                    }
                    else
                    {
                        ips200_write_16bit_data(ips200_bgcolor);
                    }
                    temp_bottom >>= 1;
                }
            }
            break;
        case IPS200_16X16_FONT:
            // �ݲ�֧��
            break;
    }

	IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ�ַ���
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ���ַ���
// ���ز���     void
// ʹ��ʾ��     ips200_show_string(0, 0, "seekfree");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_string (uint16 x, uint16 y, const char dat[])
{

    uint16 j = 0;
    while(dat[j] != '\0')
    {
        switch(ips200_display_font)
        {
            case IPS200_6X8_FONT:
                ips200_show_char(x + 6 * j, y, dat[j]);
                j ++;
                break;
            case IPS200_8X16_FONT:
                ips200_show_char(x + 8 * j, y, dat[j]);
                j ++;
                break;
            case IPS200_16X16_FONT:
                // �ݲ�֧��
                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ32λ�з��� (ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı��� �������� int32
// ����˵��     num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// ���ز���     void
// ʹ��ʾ��     ips200_show_int(0, 0, x, 3);                    // x ����Ϊ int32 int16 int8 ����
// ��ע��Ϣ     ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int (uint16 x, uint16 y, const int32 dat, uint8 num)
{


    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(num < 10)
    {
        for(; num > 0; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    ips200_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ32λ�޷��� (ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, IPS200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, IPS200_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı��� �������� uint32
// ����˵��     num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// ���ز���     void
// ʹ��ʾ��     ips200_show_uint(0, 0, x, 3);                   // x ����Ϊ uint32 uint16 uint8 ����
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_uint (uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(num < 10)
    {
        for(; num > 0; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    ips200_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ������(ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı�������������float��double
// ����˵��     num             ����λ��ʾ����   ���8λ
// ����˵��     pointnum        С��λ��ʾ����   ���6λ
// ���ز���     void
// ʹ��ʾ��     ips200_show_float(0, 0, x, 2, 3);               // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// ��ע��Ϣ     �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_float (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum)
{

    double dat_temp = dat;
    double offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    if(num < 10)
    {
        for(; num > 0; num --)
        {
            offset *= 10;
        }
        dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    }
    func_double_to_str(data_buffer, dat_temp, pointnum);
    ips200_show_string(x, y, data_buffer);
}




//-------------------------------------------------------------------------------------------------------------------
// �������     IPS200 ��ʾ����
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     *wave           ��������ָ��
// ����˵��     width           ����ʵ�ʿ��
// ����˵��     value_max       ����ʵ�����ֵ
// ����˵��     dis_width       ������ʾ��� ������Χ [0, ips200_x_max]
// ����˵��     dis_value_max   ������ʾ���ֵ ������Χ [0, ips200_y_max]
// ���ز���     void
// ʹ��ʾ��     ips200_show_wave(0, 0, data, 128, 64, 64, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_wave (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max)
{

    uint32 i = 0, j = 0;
    uint32 width_index = 0, value_max_index = 0;


	IPS200_CS(0);

    ips200_set_region(x, y, x + dis_width - 1, y + dis_value_max - 1);          // ������ʾ����
    for(i = 0; i < dis_value_max; i ++)
    {
        for(j = 0; j < dis_width; j ++)
        {
            ips200_write_16bit_data(ips200_bgcolor); 
        }
    }

	IPS200_CS(1);


    for(i = 0; i < dis_width; i ++)
    {
        width_index = i * width / dis_width;
        value_max_index = *(wave + width_index) * (dis_value_max - 1) / value_max;
        ips200_draw_point(i + x, (dis_value_max - 1) - value_max_index + y, ips200_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ
// ����˵��     x               ����x�������� ������Χ [0, ips200_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips200_y_max-1]
// ����˵��     size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// ����˵��     *chinese_buffer ��Ҫ��ʾ�ĺ�������
// ����˵��     number          ��Ҫ��ʾ����λ
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips200_show_chinese(0, 0, 16, chinese_test[0], 4, RGB565_RED);//��ʾfont�ļ������ ʾ��
// ��ע��Ϣ     ʹ��PCtoLCD2002���ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_chinese (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color)
{


    int i, j, k;
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size / 8;
    

	IPS200_CS(0);

    ips200_set_region(x, y, number * size - 1 + x, y + size - 1);
    
    for(i = 0; i < size; i ++)
    {
        temp1 = number;
        p_data = chinese_buffer + i * temp2;
        while(temp1 --)
        {
            for(k = 0; k < temp2; k ++)
            {
                for(j = 8; j > 0; j --)
                {
                    temp = (*p_data >> (j - 1)) & 0x01;
                    if(temp)
                    {
                        ips200_write_16bit_data(color);
                    }
                    else
                    {
                        ips200_write_16bit_data(ips200_bgcolor);
                    }
                }
                p_data ++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }   
    }

	IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// �������     2�� IPSҺ����ʼ��
// ����˵��     type_select     �������ӿ����� IPS200_TYPE_SPI Ϊ SPI �ӿڴ��������� IPS200_TYPE_PARALLEL8 Ϊ 8080 Э���λ����������
// ���ز���     void
// ʹ��ʾ��     ips200_init(IPS200_TYPE_PARALLEL8);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips200_init_spi ()
{
	IPS200_BL(1);
	IPS200_DC(0);
	IPS200_RST(0);
	

	spi_init(IPS200_SPIN_PIN, IPS200_SCL_PIN, IPS200_SDA_PIN, IPS200_SDA_IN_PIN, 0, MASTER, SPI_SYSclk_DIV_2);


  

    ips200_set_dir(ips200_display_dir);
    ips200_set_color(ips200_pencolor, ips200_bgcolor);

    
    IPS200_BL(1);
    IPS200_RST(0);  
    delay_ms(5);
    IPS200_RST(1);      
    delay_ms(120);
    

	IPS200_CS(0);

    ips200_write_command(0x11);
    delay_ms(120);

    ips200_write_command(0x36);
    if(ips200_display_dir == 0)
    {
        ips200_write_8bit_data(0x00);
    }
    else if(ips200_display_dir == 1)
    {
        ips200_write_8bit_data(0xC0);
    }
    else if(ips200_display_dir == 2)
    {
        ips200_write_8bit_data(0x70);
    }
    else
    {
        ips200_write_8bit_data(0xA0);
    }

    ips200_write_command(0x3A);            
    ips200_write_8bit_data(0x05);
    
    ips200_write_command(0xB2);            
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x0C); 
    ips200_write_8bit_data(0x00); 
    ips200_write_8bit_data(0x33); 
    ips200_write_8bit_data(0x33);           

    ips200_write_command(0xB7);            
    ips200_write_8bit_data(0x35);

    ips200_write_command(0xBB);            
    ips200_write_8bit_data(0x29);                                               // 32 Vcom=1.35V
                                                            
    ips200_write_command(0xC2);            
    ips200_write_8bit_data(0x01);

    ips200_write_command(0xC3);            
    ips200_write_8bit_data(0x19);                                               // GVDD=4.8V 
                                                            
    ips200_write_command(0xC4);            
    ips200_write_8bit_data(0x20);                                               // VDV, 0x20:0v

    ips200_write_command(0xC5);            
    ips200_write_8bit_data(0x1A);                                               // VCOM Offset Set

    ips200_write_command(0xC6);            
    ips200_write_8bit_data(0x01F);                                              // 0x0F:60Hz          

    ips200_write_command(0xD0);            
    ips200_write_8bit_data(0xA4);
    ips200_write_8bit_data(0xA1);                                                                                                                                                                                           
                
    ips200_write_command(0xE0);     
    ips200_write_8bit_data(0xD0);   
    ips200_write_8bit_data(0x08);   
    ips200_write_8bit_data(0x0E);   
    ips200_write_8bit_data(0x09);   
    ips200_write_8bit_data(0x09);   
    ips200_write_8bit_data(0x05);   
    ips200_write_8bit_data(0x31);   
    ips200_write_8bit_data(0x33);   
    ips200_write_8bit_data(0x48);   
    ips200_write_8bit_data(0x17);   
    ips200_write_8bit_data(0x14);   
    ips200_write_8bit_data(0x15);   
    ips200_write_8bit_data(0x31);   
    ips200_write_8bit_data(0x34);   

    ips200_write_command(0xE1);     
    ips200_write_8bit_data(0xD0);   
    ips200_write_8bit_data(0x08);   
    ips200_write_8bit_data(0x0E);   
    ips200_write_8bit_data(0x09);   
    ips200_write_8bit_data(0x09); 
    ips200_write_8bit_data(0x15);   
    ips200_write_8bit_data(0x31);   
    ips200_write_8bit_data(0x33);   
    ips200_write_8bit_data(0x48);   
    ips200_write_8bit_data(0x17);   
    ips200_write_8bit_data(0x14);   
    ips200_write_8bit_data(0x15);   
    ips200_write_8bit_data(0x31);   
    ips200_write_8bit_data(0x34);   

    ips200_write_command(0x21);
    
    ips200_write_command(0x29);

	IPS200_CS(1);


    ips200_clear();                                                             // ��ʼ��Ϊ����
}
