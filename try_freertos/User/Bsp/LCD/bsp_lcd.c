#include "./Bsp/LCD/bsp_lcd.h"

/* ��ͬҺ�����Ĳ��� */
const LCD_PARAM_TypeDef lcd_param[LCD_TYPE_NUM]={

  /* 5�������� */
  {
    /*����Һ�������ֲ�Ĳ�������*/
    .hbp = 46,  //HSYNC�����Ч����
    .vbp = 23,  //VSYNC�����Ч����

    .hsw = 1,   //HSYNC���
    .vsw = 3,   //VSYNC���

    .hfp = 40,  //HSYNCǰ����Ч����
    .vfp = 13,  //VSYNCǰ����Ч����
    
    .comment_clock_2byte = 33, //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
    .comment_clock_4byte = 21, //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��

    
    .lcd_pixel_width = LCD_PIXEL_WIDTH,//Һ���ֱ��ʣ���
    .lcd_pixel_height = LCD_PIXEL_HEIGHT,//Һ���ֱ��ʣ���
  },
  
   /* 7������������5��һ���� */
  {
    /*����Һ�������ֲ�Ĳ�������*/
    .hbp = 46,  //HSYNC�����Ч����
    .vbp = 23,  //VSYNC�����Ч����

    .hsw = 1,  	//HSYNC���
    .vsw = 1,   //VSYNC���

    .hfp = 22,  	//HSYNCǰ����Ч����
    .vfp = 22,  	//VSYNCǰ����Ч����
    
    .comment_clock_2byte = 33, //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
    .comment_clock_4byte = 21, //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��

    
    .lcd_pixel_width = LCD_PIXEL_WIDTH,//Һ���ֱ��ʣ���
    .lcd_pixel_height = LCD_PIXEL_HEIGHT,//Һ���ֱ��ʣ���
  },

  /* 4.3�������� */
  {//4.3����ĻGUI�ݲ�֧��
      /*����Һ�������ֲ�Ĳ�������*/
    .hbp = 8,  //HSYNC�����Ч����
    .vbp = 2,  //VSYNC�����Ч����

    .hsw = 41,  	//HSYNC���
    .vsw = 10,   //VSYNC���

    .hfp = 4,  	//HSYNCǰ����Ч����
    .vfp = 4,  	//VSYNCǰ����Ч����
    
    .comment_clock_2byte = 15, //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
    .comment_clock_4byte = 15, //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
    
    .lcd_pixel_width = 480,//Һ���ֱ��ʣ���
    .lcd_pixel_height = 272,//Һ���ֱ��ʣ���
  },
	
	 /* 3.5�������� */
  {
    /*����Һ�������ֲ�Ĳ�������*/
    .hbp = 46,  //HSYNC�����Ч����
    .vbp = 23,  //VSYNC�����Ч����

    .hsw = 1,   //HSYNC���
    .vsw = 3,   //VSYNC���

    .hfp = 40,  //HSYNCǰ����Ч����
    .vfp = 13,  //VSYNCǰ����Ч����
    
    .comment_clock_2byte = 33, //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
    .comment_clock_4byte = 21, //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��

    
    .lcd_pixel_width = 320,//Һ���ֱ��ʣ���
    .lcd_pixel_height = 240,//Һ���ֱ��ʣ���
  }
};

/* ��ǰʹ�õ�LCD��Ĭ��Ϊ5/7����
  * �ڴ���������ʼ��ʱ�ɸ��ݴ���оƬ���ͺ����ֲ�ͬ��LCD
*/
LCD_TypeDef cur_lcd = INCH_5;


