//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F429IGT6,正点原子Apollo STM32F4/F7开发板,主频180MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PF9          //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PF8          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PD6          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PF7          //液晶屏SPI总线时钟信号
//      DC/RS        接          PD5          //液晶屏数据/命令控制信号
//       RST         接          PD12         //液晶屏复位控制信号
//       CS          接          PD11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PH11         //触摸屏触摸中断信号
//      T_DO         接          PG3          //触摸屏SPI总线读信号
//      T_DIN        接          PI3          //触摸屏SPI总线写信号
//      T_CS         接          PI8          //触摸屏片选控制信号
//      T_CLK        接          PH6          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "bsp_touch_at24cxx.h" 
#include "bsp_touch_lcd.h"
#include "./Bsp/IIC_EEPROM/bsp_i2c_ee_hard.h"

extern __IO uint32_t  I2CTimeout;
/*****************************************************************************
 * @name       :void AT24CXX_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialize IIC interface
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void AT24CXX_Init(void)
{
	IIC_Init();//IIC初始化
}

/*****************************************************************************
 * @name       :u8 AT24CXX_ReadOneByte(u16 ReadAddr)
 * @date       :2018-08-09 
 * @function   :Read out a data at a specified address in the AT24CXX
 * @parameters :ReadAddr:the address of start reading
 * @retvalue   :Read data
******************************************************************************/ 
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;
	I2CTimeout=BSP_I2CT_LONG_TIMEOUT;
	
	//判断总线是否处于繁忙  //*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(BSP_HARD_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(49);
		
	}
	//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(40);
	}
	//发送EEPROM地址
	I2C_Send7bitAddress(BSP_HARD_I2C,0xA0,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(41);
	}
	//使能I2C总线
	I2C_Cmd(BSP_HARD_I2C,ENABLE);
	//发送接收地址
	I2C_SendData(BSP_HARD_I2C,ReadAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV8事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(42);
	}
	//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(43);
	}
	
		//发送EEPROM地址
	I2C_Send7bitAddress(BSP_HARD_I2C,0xA0,I2C_Direction_Receiver);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(44);
	}
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	
//			I2C_AcknowledgeConfig(BSP_HARD_I2C,DISABLE);
//			
//			I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
//			
//		I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
		
		while(I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_SLAVE_BYTE_RECEIVED)==0)
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(43);
	}
	{
		temp=I2C_ReceiveData(BSP_HARD_I2C);
  }
	I2C_AcknowledgeConfig(BSP_HARD_I2C,ENABLE);
	return 1;
//	u8 temp=0;		  	    																 
//    IIC_Start();  
//	if(EE_TYPE>AT24C16)
//	{
//		IIC_Send_Byte(0XA0);	   //发送写命令
//		IIC_Wait_Ack();
//		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
//	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
//	IIC_Wait_Ack(); 
//    IIC_Send_Byte(ReadAddr%256);   //发送低地址
//	IIC_Wait_Ack();	    
//	IIC_Start();  	 	   
//	IIC_Send_Byte(0XA1);           //进入接收模式			   
//	IIC_Wait_Ack();	 
//    temp=IIC_Read_Byte(0);		   
//    IIC_Stop();//产生一个停止条件	    
//	return temp;
}

/*****************************************************************************
 * @name       :void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
 * @date       :2018-08-09 
 * @function   :Write a data at a specified address in AT24CXX
 * @parameters :WriteAddr:the destination address for writing data
								DataToWrite:Data to be written
 * @retvalue   :None
******************************************************************************/ 
uint32_t AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    		

//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(30);
	}
	//发送设备地址
	I2C_Send7bitAddress(BSP_HARD_I2C,0xA0,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(31);
	}
	
	//发送外设地址
	I2C_SendData(BSP_HARD_I2C,WriteAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//检测EV8事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(2);
	}
	//发送一个字节数据
	I2C_SendData(BSP_HARD_I2C,DataToWrite);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//检测EV8事件
		while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
	}
	//停止信号
	I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
	return 1;	
//    IIC_Start();  
//	if(EE_TYPE>AT24C16)
//	{
//		IIC_Send_Byte(0XA0);	    //发送写命令
//		IIC_Wait_Ack();
//		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
//	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
//	IIC_Wait_Ack();	   
//    IIC_Send_Byte(WriteAddr%256);   //发送低地址
//	IIC_Wait_Ack(); 	 										  		   
//	IIC_Send_Byte(DataToWrite);     //发送字节							   
//	IIC_Wait_Ack();  		    	   
//    IIC_Stop();//产生一个停止条件 
//	delay_ms(10);	 
}

/*****************************************************************************
 * @name       :void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
 * @date       :2018-08-09 
 * @function   :begins to write data with a length of Len 
								at the specified address in the AT24CXX
 * @parameters :WriteAddr:the address to start writing
								DataToWrite:the header address of the data array 
								Len:Length of data to be written(2 or 4)
 * @retvalue   :None
******************************************************************************/ 
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/*****************************************************************************
 * @name       :u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
 * @date       :2018-08-09 
 * @function   :begins to read data with a length of Len 
								at the specified address in the AT24CXX,
								used to read 16bits or 32bits data
 * @parameters :ReadAddr:the address of start reading
								len:Length of data to be read(2 or 4)
 * @retvalue   :Read data
******************************************************************************/ 
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

/*****************************************************************************
 * @name       :u8 AT24CXX_Check(void)
 * @date       :2018-08-09 
 * @function   :Check that AT24CXX is normal or not,
								Use AT24CXX's last address (255) to store the token,
								if use the other 24C series, this address needs to be modified,
 * @parameters :None
 * @retvalue   :0-check successfully
								1-check failed
******************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

/*****************************************************************************
 * @name       :void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
 * @date       :2018-08-09 
 * @function   :begins to read out the specified number of data at 
								the specified address in the AT24CXX
 * @parameters :ReadAddr:the address of start reading,it is 0~255 for 24c02
								pBuffer:the header address of the data array 
								NumToRead:Number of data to be read
 * @retvalue   :None
******************************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 

/*****************************************************************************
 * @name       :void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
 * @date       :2018-08-09 
 * @function   :begins to write the specified number of data at 
								the specified address in the AT24CXX
 * @parameters :WriteAddr:the address of start writing,it is 0~255 for 24c02
								pBuffer:the header address of the data array 
								NumToRead:Number of data to be writen
 * @retvalue   :None
******************************************************************************/ 
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 











