#ifndef __BSP_LCD_H
#define	__BSP_LCD_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/** 
  * @brief  LCDҺ������
  */   
typedef struct
{
  /*����Һ�������ֲ�Ĳ�������*/
  uint8_t hbp;  //HSYNC�����Ч����
  uint8_t vbp;  //VSYNC�����Ч����

  uint8_t hsw;  	//HSYNC���
  uint8_t vsw;   //VSYNC���

  uint8_t hfp;  	//HSYNCǰ����Ч����
  uint8_t vfp;  	//VSYNCǰ����Ч����
  
  uint8_t comment_clock_2byte; //rgb565/argb4444��˫�ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��
  uint8_t comment_clock_4byte; //Argb8888�����ֽ�����ʱ�Ƽ�ʹ�õ�Һ��ʱ��Ƶ��

  uint16_t lcd_pixel_width; //Һ���ֱ��ʣ���
  uint16_t lcd_pixel_height;//Һ���ֱ��ʣ���

}LCD_PARAM_TypeDef;


/** 
  * @brief  LCDҺ������
  */   
typedef enum
{ 
  INCH_5  = 0x00, /* Ұ��5���� */
  INCH_7,     /* Ұ��7���� */
  INCH_4_3,  /* Ұ��4.3���� */
	INCH_3_5,  /* Ұ��4.3���� */
  
  LCD_TYPE_NUM /* LCD��������*/
}LCD_TypeDef;

/* ��ǰʹ�õ�LCD��Ĭ��Ϊ5���� */
extern LCD_TypeDef cur_lcd;
/* ��ͬҺ�����Ĳ��� */
extern const LCD_PARAM_TypeDef lcd_param[];

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
/*����Һ�������ֲ�Ĳ�������*/
#define HBP  lcd_param[cur_lcd].hbp		//HSYNC�����Ч����
#define VBP  lcd_param[cur_lcd].vbp		//VSYNC�����Ч����

#define HSW  lcd_param[cur_lcd].hsw		//HSYNC���
#define VSW  lcd_param[cur_lcd].vsw		//VSYNC���

#define HFP  lcd_param[cur_lcd].hfp		//HSYNCǰ����Ч����
#define VFP  lcd_param[cur_lcd].vfp		//VSYNCǰ����Ч����


/* LCD Size (Width and Height) */
#define  LCD_PIXEL_WIDTH        ((uint16_t)800)
#define  LCD_PIXEL_HEIGHT       ((uint16_t)480)

#define  LCD_FRAME_BUFFER       ((uint32_t)0xD0000000)






#endif