#include "bsp_touch_lcd.h"
#include "./Bsp/TOUCH/bsp_touch_spi.h"

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	


/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :��Ĵ���д������
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(uint8_t data)
{ 
   LCD_CS_CLR;     
	 LCD_RS_CLR;	  
   BSP_SPI_TOUCH_SendByte(data);
   LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :��ȡ����
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR;
	 LCD_RS_SET;
   BSP_SPI_TOUCH_SendByte(data);
   LCD_CS_SET;
}
/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :��Ĵ���д������
 * @parameters :LCD_Reg:Register address
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :��ȡRAM����
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :д��16λ����
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
   LCD_CS_CLR;
   LCD_RS_SET;  
   BSP_SPI_TOUCH_SendByte(Data>>8);
	 BSP_SPI_TOUCH_SendByte(Data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :������ʾ����
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	
	u16 twidth, theight;
    twidth = xStar + xEnd - 1;
    theight = yStar + yEnd - 1;
	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   


/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :������ʾ��С
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :���㺯��
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}
/*****************************************************************************
 * @name       :void LCD_RD_DATA(u16 Color)
 * @date       :2018-08-09 
 * @function   :��ȡ����
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
u8 LCD_RD_DATA(void)
{
	 u8 data;
	 LCD_CS_CLR;
	 LCD_RS_SET;
	 SPI_SetSpeed(SPI_BaudRatePrescaler_16);
	 data = BSP_SPI_TOUCH_SendByte(0xFF);
	 SPI_SetSpeed(SPI_BaudRatePrescaler_2);
	 LCD_CS_SET;
	 return data;
}

/*****************************************************************************
 * @name       :void LCD_ReadReg(u8 LCD_Reg)
 * @date       :2018-08-09 
 * @function   :�ӼĴ�����ȡ����
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
u8 LCD_ReadReg(u8 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);
  return LCD_RD_DATA();
}

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :��ȡRAM����
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}	
/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-08-09 
 * @function   :��ȡ16λ����
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
u16 Lcd_ReadData_16Bit(void)
{
	u16 r,g;
	LCD_CS_CLR;
	LCD_RS_CLR;	  
	BSP_SPI_TOUCH_SendByte(lcddev.rramcmd);
	SPI_SetSpeed(SPI_BaudRatePrescaler_16);
	LCD_RS_SET;
	BSP_SPI_TOUCH_SendByte(0xFF);
	r = BSP_SPI_TOUCH_SendByte(0xFF);
	g = BSP_SPI_TOUCH_SendByte(0xFF);
	SPI_SetSpeed(SPI_BaudRatePrescaler_2);
	LCD_CS_SET;
	r<<=8;
	r|=g;
	return r;
}

/*****************************************************************************
 * @name       :void opt_delay(u8 i)
 * @date       :2018-08-09 
 * @function   :��mdk -O1ʱ���Ż�ʱ��Ҫ����
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void opt_delay(u8 i)
{
    while (i--);
}

/*****************************************************************************
 * @name       :u16  LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :���㺯��
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
u16  LCD_ReadPoint(u16 x,u16 y) //����
{
	
	
	u16 r=0,g=0,b=0;
	u16 color;
	LCD_SetCursor(x,y);//���ù��λ�� 
	r = Lcd_ReadData_16Bit();
	opt_delay(2);
	b = Lcd_ReadData_16Bit();
	g= r & 0XFF;
	g <<= 8;
	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
//  u16 r, g, b;

//    if (x >= lcddev.width || y >= lcddev.height)return 0;   //�����˷�Χ,ֱ�ӷ���

//    LCD_SetCursor(x, y);


//        LCD_WR_REG(0X2E);
//    

//    r = LCD_RD_DATA();          //�ٶ�


//    r = LCD_RD_DATA();          //ʵ��������ɫ

//    //9341/5310/5510/7789 Ҫ��2�ζ���
//    b = LCD_RD_DATA();
//    g = r & 0XFF;               //���� 9341/5310/5510/7789, ��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
//    g <<= 8;
//    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  // 9341/5310/5510/7789 ��Ҫ��ʽת��һ��
	
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color
 * @date       :2018-08-09 
 * @function   :���
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void LCD_Clear(u16 Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<lcddev.height;i++)
	{
    for(m=0;m<lcddev.width;m++)
    {	
			Lcd_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Һ����ģ���ʼ��
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
		  GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd(BSP_TOUCH_SPI_GPIOD_CLK,ENABLE);           //ʹ��GPIODʱ��
    
    //PD6
    GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_LCD_DC_PIN;            //PD5
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;  //�������
	  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;          //����
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;     //����         
    GPIO_Init(BSP_TOUCH_SPI_GPIOD,&GPIO_InitStruct);     //��ʼ��
	
	  GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_LCD_LED_PIN;            //PD6
	  GPIO_Init(BSP_TOUCH_SPI_GPIOD,&GPIO_InitStruct);     //��ʼ��
	
	  GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_LCD_CS_PIN;            //PD11
	  GPIO_Init(BSP_TOUCH_SPI_GPIOD,&GPIO_InitStruct);     //��ʼ��
	
	  GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_LCD_RESET_PIN;            //PD13
	  GPIO_Init(BSP_TOUCH_SPI_GPIOD,&GPIO_InitStruct);     //��ʼ��
}


/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :����LCD
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}





/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :�趨LCD��ʾ����
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
	    lcddev.rramcmd=0x2E;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_G;		
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width=LCD_W;
			lcddev.height=LCD_G;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1   
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_G;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_W;
			lcddev.height=LCD_G;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
		
		LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(0);
        LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.width - 1) >> 8);
        LCD_WR_DATA((lcddev.width - 1) & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(0);
        LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.height - 1) >> 8);
        LCD_WR_DATA((lcddev.height - 1) & 0XFF);
	}		
}	 



/*****************************************************************************
 * @name       :void LCDD_Init(void)
 * @date       :2018-08-09 
 * @function   :��Ĵ���д��ָ������ʼ������оƬ
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCDD_Init(void)
{  
	BSP_SPI_TOUCH_Init();		   			        //��ʼ��SPI
//	SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2); //����Ϊ45Mʱ��,����ģʽ
	LCD_GPIOInit();//LCD GPIO��ʼ��										 
 	LCD_RESET(); //LCD ��λ
//*************3.2inch ILI9341��ʼ��**********//	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xD9); //C1 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] //0x01
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x26); 	 //3F
	LCD_WR_DATA(0x26); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB0); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x52);
	LCD_WR_DATA(0xB7);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x48);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x31);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x1F);

	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on		

  LCD_direction(USE_HORIZONTAL);//����LCD��ʾ����
	LCD_LED=1;//��������	 
	LCD_Clear(WHITE);//��ȫ����ɫ
}


 //��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
    u16  r, g, b, rgb;
    b = (c >> 0) & 0x1f;
    g = (c >> 5) & 0x3f;
    r = (c >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);
    return (rgb);
}




static u32 fac_us=0;							//us��ʱ������
//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	 
void delay_us(u32 nus)
{ 
       u32 ticks;
       u32 told,tnow,reload,tcnt=0;
       if((0x0001&(SysTick->CTRL)) ==0)    //��ʱ��δ����
              vPortSetupTimerInterrupt();  //��ʼ����ʱ��
 
       reload = SysTick->LOAD;                     //��ȡ��װ�ؼĴ���ֵ
       ticks = nus * (SystemCoreClock / 1000000);  //����ʱ��ֵ
       
       vTaskSuspendAll();//��ֹOS���ȣ���ֹ���us��ʱ
       told=SysTick->VAL;  //��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ��
       while(1)
       {
              tnow=SysTick->VAL; //��ȡ��ǰ��ֵ�Ĵ���ֵ
              if(tnow!=told)  //��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
              {         
                     if(tnow<told)  //��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0
                          tcnt+=told-tnow; //����ֵ=��ʼֵ-��ǰֵ
 
                     else     //��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
                            tcnt+=reload-tnow+told;   //����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ  ��
                                                      //�Ѵӿ�ʼֵ�Ƶ�0�� 
 
                     told=tnow;   //���¿�ʼֵ
                     if(tcnt>=ticks)break;  //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
              } 
       }  
       xTaskResumeAll();	//�ָ�OS����		   
} 
//SystemCoreClockΪϵͳʱ��(system_stmf4xx.c��)��ͨ��ѡ���ʱ����Ϊ
//systick��ʱ��ʱ�ӣ����ݾ����������


//����������Ե���ʱ����
void delay_ms_rtos(uint32_t count)
{
	uint8_t i;
	uint32_t told=SysTick->VAL;
	uint32_t ticks;
	uint32_t tnow;
	uint32_t tcnt=0;
	uint32_t reload=SysTick->LOAD;
	ticks=(SystemCoreClock/1000)*count;//���������ʱʱ�������systick����ֵ
	told=SysTick->VAL;//��ʼ��ʱǰ��¼��ǰ�ļ�����ֵ
	while(1)//ѭ���ȴ���ʱ����
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
				tcnt+=told-tnow;//��¼systick�ļ���ֵ
			else
				tcnt+=reload+told-tnow;//���systick��װ���ˣ���¼systick�ļ���ֵ
			told=tnow;
			if(tcnt>=ticks)//systick�ļ���ֵ�ﵽ������ʱʱ��
				break;//������ʱ
		}
	}
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
	{delay_us(1000);}
}