#include "./Bsp/LCD/bsp_lcd.h"

/* 不同液晶屏的参数 */
const LCD_PARAM_TypeDef lcd_param[LCD_TYPE_NUM]={

  /* 5寸屏参数 */
  {
    /*根据液晶数据手册的参数配置*/
    .hbp = 46,  //HSYNC后的无效像素
    .vbp = 23,  //VSYNC后的无效行数

    .hsw = 1,   //HSYNC宽度
    .vsw = 3,   //VSYNC宽度

    .hfp = 40,  //HSYNC前的无效像素
    .vfp = 13,  //VSYNC前的无效行数
    
    .comment_clock_2byte = 33, //rgb565/argb4444等双字节像素时推荐使用的液晶时钟频率
    .comment_clock_4byte = 21, //Argb8888等四字节像素时推荐使用的液晶时钟频率

    
    .lcd_pixel_width = LCD_PIXEL_WIDTH,//液晶分辨率，宽
    .lcd_pixel_height = LCD_PIXEL_HEIGHT,//液晶分辨率，高
  },
  
   /* 7寸屏参数（与5寸一样） */
  {
    /*根据液晶数据手册的参数配置*/
    .hbp = 46,  //HSYNC后的无效像素
    .vbp = 23,  //VSYNC后的无效行数

    .hsw = 1,  	//HSYNC宽度
    .vsw = 1,   //VSYNC宽度

    .hfp = 22,  	//HSYNC前的无效像素
    .vfp = 22,  	//VSYNC前的无效行数
    
    .comment_clock_2byte = 33, //rgb565/argb4444等双字节像素时推荐使用的液晶时钟频率
    .comment_clock_4byte = 21, //Argb8888等四字节像素时推荐使用的液晶时钟频率

    
    .lcd_pixel_width = LCD_PIXEL_WIDTH,//液晶分辨率，宽
    .lcd_pixel_height = LCD_PIXEL_HEIGHT,//液晶分辨率，高
  },

  /* 4.3寸屏参数 */
  {//4.3寸屏幕GUI暂不支持
      /*根据液晶数据手册的参数配置*/
    .hbp = 8,  //HSYNC后的无效像素
    .vbp = 2,  //VSYNC后的无效行数

    .hsw = 41,  	//HSYNC宽度
    .vsw = 10,   //VSYNC宽度

    .hfp = 4,  	//HSYNC前的无效像素
    .vfp = 4,  	//VSYNC前的无效行数
    
    .comment_clock_2byte = 15, //rgb565/argb4444等双字节像素时推荐使用的液晶时钟频率
    .comment_clock_4byte = 15, //Argb8888等四字节像素时推荐使用的液晶时钟频率
    
    .lcd_pixel_width = 480,//液晶分辨率，宽
    .lcd_pixel_height = 272,//液晶分辨率，高
  },
	
	 /* 3.5寸屏参数 */
  {
    /*根据液晶数据手册的参数配置*/
    .hbp = 46,  //HSYNC后的无效像素
    .vbp = 23,  //VSYNC后的无效行数

    .hsw = 1,   //HSYNC宽度
    .vsw = 3,   //VSYNC宽度

    .hfp = 40,  //HSYNC前的无效像素
    .vfp = 13,  //VSYNC前的无效行数
    
    .comment_clock_2byte = 33, //rgb565/argb4444等双字节像素时推荐使用的液晶时钟频率
    .comment_clock_4byte = 21, //Argb8888等四字节像素时推荐使用的液晶时钟频率

    
    .lcd_pixel_width = 320,//液晶分辨率，宽
    .lcd_pixel_height = 240,//液晶分辨率，高
  }
};

/* 当前使用的LCD，默认为5/7寸屏
  * 在触摸驱动初始化时可根据触摸芯片的型号驱分不同的LCD
*/
LCD_TypeDef cur_lcd = INCH_5;


