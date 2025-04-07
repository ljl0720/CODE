#ifndef __BSP_TOUCH_LCD_H
#define __BSP_TOUCH_LCD_H

#include "stm32f4xx.h"
#include "stdlib.h"



#define BSP_TOUCH_SPI_GPIOD_CLK                 RCC_AHB1Periph_GPIOD
#define BSP_TOUCH_SPI_GPIOD                     GPIOD

#define BSP_TOUCH_LCD_CS_PIN                    GPIO_Pin_11 
#define BSP_TOUCH_LCD_CS_PINSOURCE              GPIO_PinSource11

#define BSP_TOUCH_LCD_RESET_PIN                 GPIO_Pin_13
#define BSP_TOUCH_LCD_RESET_PINSOURCE           GPIO_PinSource13

#define BSP_TOUCH_LCD_DC_PIN                    GPIO_Pin_5 
#define BSP_TOUCH_LCD_DC_PINSOURCE              GPIO_PinSource5

#define BSP_TOUCH_LCD_LED_PIN                   GPIO_Pin_6
#define BSP_TOUCH_LCD_LED_PINSOURCE             GPIO_PinSource6


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  rramcmd;
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 1//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_G 240

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

//typedef struct
//{
//    vu16 LCD_REG;
//    vu16 LCD_RAM;
//} LCD_TypeDef;

//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其!
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 

#define LED   6        //背光控制引脚
#define CS    11       //片选引脚
#define RS    5       //寄存器/数据选择引脚  
#define RST   13       //复位引脚

//hal库的封装移植
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define	LCD_LED PDout(LED) //LCD背光    		 

#define LCD_CS  PDout(CS) 
#define LCD_RS  PDout(RS) 
#define LCD_RST PDout(RST)
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

#define	LCD_CS_SET  LCD_CS=1//GPIO_TYPE->BSRR=1<<LCD_CS    //片选端口  	PB11
#define	LCD_RS_SET	LCD_RS=1//GPIO_TYPE->BSRR=1<<LCD_RS    //数据/命令  PB10	  
#define	LCD_RST_SET	LCD_RST=1//GPIO_TYPE->BSRR=1<<LCD_RST    //复位			PB12

 							    
#define	LCD_CS_CLR  LCD_CS=0//GPIO_TYPE->BRR=1<<LCD_CS     //片选端口  	PB11
#define	LCD_RS_CLR	LCD_RS=0//GPIO_TYPE->BRR=1<<LCD_RS     //数据/命令  PB10	 
#define	LCD_RST_CLR	LCD_RST=0//GPIO_TYPE->BRR=1<<LCD_RST    //复位			  PB12

				


//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)




void LCD_WR_REG(u8 data);
void LCD_WR_DATA(u8 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);
u8 LCD_RD_DATA(void);
u8 LCD_ReadReg(u8 LCD_Reg);
void LCD_ReadRAM_Prepare(void);
u16 Lcd_ReadData_16Bit(void);
void opt_delay(u8 i);
u16  LCD_ReadPoint(u16 x,u16 y);
void LCD_Clear(u16 Color);
void LCD_GPIOInit(void);
void LCD_RESET(void);
void LCD_direction(u8 direction);
void LCDD_Init(void);
u16 LCD_BGR2RGB(u16 c);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_ms_rtos(uint32_t count);
#endif