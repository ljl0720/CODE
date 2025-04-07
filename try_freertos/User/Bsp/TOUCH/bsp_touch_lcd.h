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


//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  rramcmd;
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
/////////////////////////////////////�û�������///////////////////////////////////	 
#define USE_HORIZONTAL  	 1//����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 320
#define LCD_G 240

//TFTLCD������Ҫ���õĺ���		   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//typedef struct
//{
//    vu16 LCD_REG;
//    vu16 LCD_RAM;
//} LCD_TypeDef;

//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����!
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 

#define LED   6        //�����������
#define CS    11       //Ƭѡ����
#define RS    5       //�Ĵ���/����ѡ������  
#define RST   13       //��λ����

//hal��ķ�װ��ֲ
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define	LCD_LED PDout(LED) //LCD����    		 

#define LCD_CS  PDout(CS) 
#define LCD_RS  PDout(RS) 
#define LCD_RST PDout(RST)
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 

#define	LCD_CS_SET  LCD_CS=1//GPIO_TYPE->BSRR=1<<LCD_CS    //Ƭѡ�˿�  	PB11
#define	LCD_RS_SET	LCD_RS=1//GPIO_TYPE->BSRR=1<<LCD_RS    //����/����  PB10	  
#define	LCD_RST_SET	LCD_RST=1//GPIO_TYPE->BSRR=1<<LCD_RST    //��λ			PB12

 							    
#define	LCD_CS_CLR  LCD_CS=0//GPIO_TYPE->BRR=1<<LCD_CS     //Ƭѡ�˿�  	PB11
#define	LCD_RS_CLR	LCD_RS=0//GPIO_TYPE->BRR=1<<LCD_RS     //����/����  PB10	 
#define	LCD_RST_CLR	LCD_RST=0//GPIO_TYPE->BRR=1<<LCD_RST    //��λ			  PB12

				


//������ɫ
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
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)




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