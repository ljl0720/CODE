#ifndef __BSP_LCD_H
#define	__BSP_LCD_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/** 
  * @brief  LCD液晶参数
  */   
typedef struct
{
  /*根据液晶数据手册的参数配置*/
  uint8_t hbp;  //HSYNC后的无效像素
  uint8_t vbp;  //VSYNC后的无效行数

  uint8_t hsw;  	//HSYNC宽度
  uint8_t vsw;   //VSYNC宽度

  uint8_t hfp;  	//HSYNC前的无效像素
  uint8_t vfp;  	//VSYNC前的无效行数
  
  uint8_t comment_clock_2byte; //rgb565/argb4444等双字节像素时推荐使用的液晶时钟频率
  uint8_t comment_clock_4byte; //Argb8888等四字节像素时推荐使用的液晶时钟频率

  uint16_t lcd_pixel_width; //液晶分辨率，宽
  uint16_t lcd_pixel_height;//液晶分辨率，高

}LCD_PARAM_TypeDef;


/** 
  * @brief  LCD液晶类型
  */   
typedef enum
{ 
  INCH_5  = 0x00, /* 野火5寸屏 */
  INCH_7,     /* 野火7寸屏 */
  INCH_4_3,  /* 野火4.3寸屏 */
	INCH_3_5,  /* 野火4.3寸屏 */
  
  LCD_TYPE_NUM /* LCD类型总数*/
}LCD_TypeDef;

/* 当前使用的LCD，默认为5寸屏 */
extern LCD_TypeDef cur_lcd;
/* 不同液晶屏的参数 */
extern const LCD_PARAM_TypeDef lcd_param[];

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
/*根据液晶数据手册的参数配置*/
#define HBP  lcd_param[cur_lcd].hbp		//HSYNC后的无效像素
#define VBP  lcd_param[cur_lcd].vbp		//VSYNC后的无效行数

#define HSW  lcd_param[cur_lcd].hsw		//HSYNC宽度
#define VSW  lcd_param[cur_lcd].vsw		//VSYNC宽度

#define HFP  lcd_param[cur_lcd].hfp		//HSYNC前的无效像素
#define VFP  lcd_param[cur_lcd].vfp		//VSYNC前的无效行数


/* LCD Size (Width and Height) */
#define  LCD_PIXEL_WIDTH        ((uint16_t)800)
#define  LCD_PIXEL_HEIGHT       ((uint16_t)480)

#define  LCD_FRAME_BUFFER       ((uint32_t)0xD0000000)






#endif