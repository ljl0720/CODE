#include "bsp_i2c_ee_hard.h"
#include "./Bsp/usart1/bsp_usart.h"

__IO uint32_t  I2CTimeout = BSP_I2CT_LONG_TIMEOUT;   

uint16_t BSP_EEPROM_ADDRESS;//EEPROM设备地址

long double Double_Buffer[7] = {0};//存储小数部分

int Int_Bufffer[7] = {0};		//存储整数部分

uint8_t Char_Buffer[7]={'\0'};//存储字符

uint8_t Ctl_Flag = 0;		

extern uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];
/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
void BSP_EE_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(BSP_HARD_I2C_GPIO_CLK,ENABLE);
	BSP_HARD_I2C_CLK_INIT(BSP_HARD_I2C_CLK,ENABLE);
	
	GPIO_PinAFConfig(BSP_HARD_I2C_SCL_GPIO,BSP_HARD_I2C_SCL_SOURCE,BSP_HARD_I2C_AF);
	GPIO_PinAFConfig(BSP_HARD_I2C_SDA_GPIO,BSP_HARD_I2C_SDA_SOURCE,BSP_HARD_I2C_AF);
	//复用模式
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	//输出模式
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	//不上拉也不下拉
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=BSP_HARD_I2C_SCL_PIN;
	GPIO_Init(BSP_HARD_I2C_SCL_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_HARD_I2C_SDA_PIN;
	GPIO_Init(BSP_HARD_I2C_SDA_GPIO,&GPIO_InitStruct);
	
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
void BSP_EE_I2C_Config(void)
{
	
	I2C_InitTypeDef  I2C_InitStruct;
	
	I2C_InitStruct.I2C_ClockSpeed=BSP_I2C_Speed;
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;//设置时钟占空比 2：1 LOW:HIGH
	I2C_InitStruct.I2C_OwnAddress1=BSP_I2C_OWN_ADDRESS7;//设置自身地址 唯一
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;//使能响应
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//寻址模式  地址长度为7bit
	I2C_Init(BSP_HARD_I2C,&I2C_InitStruct);
	I2C_Cmd(BSP_HARD_I2C,ENABLE); //使能I2C
	
	I2C_AcknowledgeConfig(BSP_HARD_I2C,ENABLE);	//自动应答ACK
		
}



/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  BSP_EE_HARD_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0;
}

/**
  * @brief  设备繁忙，等待操作 
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    //发送起始信号
    I2C_GenerateSTART(BSP_HARD_I2C, ENABLE);
    //读取I2C2的状态寄存器SR1的数值
    SR1_Tmp = I2C_ReadRegister(BSP_HARD_I2C, I2C_Register_SR1);
    //发送EEPROM设备地址
    I2C_Send7bitAddress(BSP_HARD_I2C, BSP_EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(BSP_HARD_I2C, I2C_Register_SR1) & 0x0002));
  
  //清除标志位
  I2C_ClearFlag(BSP_HARD_I2C, I2C_FLAG_AF);
 //发送停止信号    
  I2C_GenerateSTOP(BSP_HARD_I2C, ENABLE); 
}



/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void BSP_I2C_EE_Init(void)
{
	BSP_EE_GPIO_Config();
	BSP_EE_I2C_Config();
	
  BSP_EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t BSP_I2C_EE_WritePage(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByte)
{
	
	I2CTimeout=BSP_I2CT_LONG_TIMEOUT;
	//判断总线是否处于繁忙
	while(I2C_GetFlagStatus(BSP_HARD_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(4);
		
	}
	//开始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(5);
	}
	//发送设备地址
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(6);
	}
	//发送外设地址
	I2C_SendData(BSP_HARD_I2C,WriteAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV8事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(7);
	}
	while(NumByte--)
	{
		I2C_SendData(BSP_HARD_I2C,*pBuffer);
		pBuffer++;
		I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
		//检测EV8事件
		while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(8);
	}
	}
	//停止信号
	I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
	return 1;
}

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t BSP_I2C_EE_WriteByte(uint8_t *pBuffer, uint8_t WriteAddr)
{
	
	//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(0);
	}
	//发送设备地址
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(1);
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
	I2C_SendData(BSP_HARD_I2C,*pBuffer);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//检测EV8事件
		while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
	}
	//停止信号
	I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
	return 1;
	
}


/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void BSP_I2C_EE_WriteBuffer(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByte)
{
	//设置页数量 单个数量 地址 字节数
	uint8_t Num_Page=0,Num_Signel=0,Addr=0,count=0;
	//计算写入地址
	Addr=WriteAddr % BSP_I2C_PageSize;
	//计算多余所需的页数
	count=BSP_I2C_PageSize-Addr;
	//计算需要多少页   //需使用页数量=待写入数量/单个页能存储数据大小               取整
	Num_Page=NumByte / BSP_I2C_PageSize;
	//计算剩余不足一页的数据    //需使用单个字节数量=带写入数量%单个页能存储数据大小         取余
	Num_Signel=NumByte % BSP_I2C_PageSize;
	
	if(Addr==0)//写入地址设定为0
	{
		if(Num_Page==0)//数据量不足一页
		{
			BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//将数据写入一页
			I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
		}
		else//写入地址不为0
		{
			while(Num_Page--)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,BSP_I2C_PageSize);//先在偏移后的写入地址写入一页数据
				I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
				WriteAddr+=BSP_I2C_PageSize;//偏移写入后的地址
				pBuffer+=BSP_I2C_PageSize;//数据偏移，准备下次写入地址
			}
			if(Num_Signel!=0)//有不足一页数据待写入
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//在偏移后的地址写入剩余数据
				I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
			}
		}
	}
	else//写入地址设定不为0
	{
		if(Num_Page==0)//未满一页数据
		{
			BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//将数据写入一页
			I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
		}
		else//数据量大于一页
		{
			NumByte-=count;//获取剩余
			Num_Page=NumByte / BSP_I2C_PageSize;//计算写入数据页数
			Num_Signel=NumByte % BSP_I2C_PageSize;//计算剩余不满一页的数据
			
			if(count!=0)//需要多余的页数
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,count);//写入
				I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
				WriteAddr+=count;//计算总需要写入页数
				pBuffer+=count;//				
			}
			while(Num_Page--)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,BSP_I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
				WriteAddr+=BSP_I2C_PageSize;
				pBuffer+=BSP_I2C_PageSize;
			}
			if(Num_Signel!=0)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//写入剩余不满一页的数据
				I2C_EE_WaitEepromStandbyState();//等待EEPROM繁忙结束
			}
		}
	}
	
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */

uint32_t BSP_I2C_EE_ReadBuffer(uint8_t *pBuffer,uint8_t ReadAddr,uint16_t NumByte)
{
	
	I2CTimeout=BSP_I2CT_LONG_TIMEOUT;
	
	//判断总线是否处于繁忙  //*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(BSP_HARD_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(9);
		
	}
	//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(10);
	}
	//发送EEPROM地址
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(11);
	}
	//使能I2C总线
	I2C_Cmd(BSP_HARD_I2C,ENABLE);
	//发送接收地址
	I2C_SendData(BSP_HARD_I2C,ReadAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV8事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(12);
	}
	//起始信号
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(13);
	}
	
		//发送EEPROM地址
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Receiver);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(14);
	}
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	while(NumByte)
	{
		if(NumByte==1)
		{
			I2C_AcknowledgeConfig(BSP_HARD_I2C,DISABLE);
			
			I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
		}		
		I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
		
		while(I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_SLAVE_BYTE_RECEIVED)==0)
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
	}
	{
		*pBuffer=I2C_ReceiveData(BSP_HARD_I2C);
		pBuffer++;
		NumByte--;
	}
  }
	I2C_AcknowledgeConfig(BSP_HARD_I2C,ENABLE);
	return 1;
}

/*
***  外置EEPROM读取写入接口
***
*/
void BSP_I2C_EE_API(uint8_t *SaveBuffer)
{
	
	/*读取数据标志位*/
  BSP_I2C_EE_ReadBuffer(&Ctl_Flag, 0, 1);
	
	if(Ctl_Flag!=0xAB)/*若标志等于0xcd，表示之前已有写入数据*/
	{
		printf("\r\n没有检测到数据标志，FLASH没有存储数据\r\n");
        Ctl_Flag =0xAB;
		
		/*写入标志到0地址*/
        BSP_I2C_EE_WriteBuffer(&Ctl_Flag, 0, 1); 
		
	for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
        {
           Char_Buffer[I2C_EE_WriteBuffer_Num] = SaveBuffer[I2C_EE_WriteBuffer_Num];
        }
				
				
				/*写入数据*/
				BSP_I2C_EE_WriteBuffer((void*)SaveBuffer,CHAR_ADDR,sizeof(SaveBuffer));
				
				printf("向芯片写入数据：");
				
				/*打印到串口*/
  for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
        {
          printf("小数tx = %d\r\n",Char_Buffer[I2C_EE_WriteBuffer_Num]);
        }
				
	}else{
		      Ctl_Flag =0xA1;
		      printf("\r\n检测到数据标志\r\n");
		    /*读取整数数据*/
        BSP_I2C_EE_ReadBuffer((void*)Char_Buffer, CHAR_ADDR, sizeof(Char_Buffer));
		
				printf("\r\n从芯片读到数据：\r\n");	
		
		     for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
				{
					printf("整数 rx = %d \r\n",Char_Buffer[I2C_EE_WriteBuffer_Num]);				
				}
		
	}
				
}
