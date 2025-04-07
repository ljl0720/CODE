#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

#include "stm32f4xx.h"

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 	  



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

										    
//������������
typedef struct
{
	u8 (*init)(void);			//��ʼ��������������
	u8 (*scan)(u8);				//ɨ�败����.0,��Ļɨ��;1,��������;	 
	void (*adjust)(void);		//������У׼
	u16 x0;						//ԭʼ����(��һ�ΰ���ʱ������)
	u16 y0;
	u16 x; 						//��ǰ����(�˴�ɨ��ʱ,����������)
	u16 y;						   	    
	u8  sta;					//�ʵ�״̬ 
								//b7:����1/�ɿ�0; 
	                            //b6:0,û�а�������;1,�а�������.         			  
////////////////////////������У׼����/////////////////////////			
//	uint8_t xfac;					
//	uint8_t yfac;
//	uint8_t xoff;
//	uint8_t yoff;	
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//touchtype=0��ʱ��,�ʺ�����ΪX����,����ΪY�����TP.
//touchtype=1��ʱ��,�ʺ�����ΪY����,����ΪX�����TP.
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

//�봥����оƬ��������	   
//�봥����оƬ��������	  


#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40020C14 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021810 
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40020C14 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40021810 
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40020C14 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����


#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����

#define PEN     PHin(11)    //T_PEN
#define DOUT    PGin(3)    //T_MISO
#define TDIN    PIout(3)   //T_MOSI
#define TCLK    PHout(10)   //T_SCK
#define TCS     PIout(8)   //T_CS 
   
void TP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//��һ������У׼��
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//��һ�����
u8 TP_Scan(u8 tp);								//ɨ��
void TP_Save_Adjdata(void);						//����У׼����
u8 TP_Get_Adjdata(void);						//��ȡУ׼����
void TP_Adjust(void);							//������У׼
u8 TP_Init(void);								//��ʼ��
																 
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//��ʾУ׼��Ϣ
#endif