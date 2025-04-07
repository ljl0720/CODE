#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

#include "stm32f4xx.h"

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 	  



#define BSP_TOUCH_GPIOI_CLK                 RCC_AHB1Periph_GPIOI
#define BSP_TOUCH_GPIOI                     GPIOI
#define BSP_TOUCH_GPIOH_CLK                 RCC_AHB1Periph_GPIOH
#define BSP_TOUCH_GPIOH                     GPIOH
#define BSP_TOUCH_GPIOG_CLK                 RCC_AHB1Periph_GPIOG
#define BSP_TOUCH_GPIOG                     GPIOG

#define BSP_TOUCH_PEN_PIN                       GPIO_Pin_11 
#define BSP_TOUCH_PEN_PINSOURCE                 GPIO_PinSource11

#define BSP_TOUCH_MISO_PIN                      GPIO_Pin_3
#define BSP_TOUCH_MISO_PINSOURCE                GPIO_PinSource3

#define BSP_TOUCH_MOSI_PIN                      GPIO_Pin_3
#define BSP_TOUCH_MOSI_PINSOURCE                GPIO_PinSource3

#define BSP_TOUCH_SCK_PIN                       GPIO_Pin_10
#define BSP_TOUCH_SCK_PINSOURCE                 GPIO_PinSource10

#define BSP_TOUCH_CS_PIN                        GPIO_Pin_8
#define BSP_TOUCH_CS_PINSOURCE                  GPIO_PinSource8

										    
//触摸屏控制器
typedef struct
{
	u8 (*init)(void);			//初始化触摸屏控制器
	u8 (*scan)(u8);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);		//触摸屏校准
	u16 x0;						//原始坐标(第一次按下时的坐标)
	u16 y0;
	u16 x; 						//当前坐标(此次扫描时,触屏的坐标)
	u16 y;						   	    
	u8  sta;					//笔的状态 
								//b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下.         			  
////////////////////////触摸屏校准参数/////////////////////////			
//	uint8_t xfac;					
//	uint8_t yfac;
//	uint8_t xoff;
//	uint8_t yoff;	
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
//touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

//与触摸屏芯片连接引脚	   
//与触摸屏芯片连接引脚	  


#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40020C14 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021810 
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40020C14 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40021810 
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40020C14 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入


#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

#define PEN     PHin(11)    //T_PEN
#define DOUT    PGin(3)    //T_MISO
#define TDIN    PIout(3)   //T_MOSI
#define TCLK    PHout(10)   //T_SCK
#define TCS     PIout(8)   //T_CS 
   
void TP_Write_Byte(u8 num);						//向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);							//读取AD转换值
u16 TP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//双方向读取(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标校准点
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//画一个大点
u8 TP_Scan(u8 tp);								//扫描
void TP_Save_Adjdata(void);						//保存校准参数
u8 TP_Get_Adjdata(void);						//读取校准参数
void TP_Adjust(void);							//触摸屏校准
u8 TP_Init(void);								//初始化
																 
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//显示校准信息
#endif