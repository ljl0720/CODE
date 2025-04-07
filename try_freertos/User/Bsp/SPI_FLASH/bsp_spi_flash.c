#include "./Bsp/SPI_FLASH/bsp_spi_flash.h"



static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

/* 发送缓冲区初始化 */
uint8_t Flash_Tx_Buffer[] = "SPI flash 操作";
uint8_t Flash_Rx_Buffer[Flash_BufferSize];

//读取的ID存储位置
__IO uint32_t Flash_DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO uint8_t TransferStatus1 = 0;


 /**
  * @brief  SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
	
void BSP_SPI_FLASH_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef  SPI_InitStruct;
	//使能SPI5时钟
	BSP_FLASH_SPI_CLK_INIT(BSP_FLASH_SPI_CLK,ENABLE);
	//使能SPI5的MISO、MOSI、CS、SCK时钟
	RCC_AHB1PeriphClockCmd(BSP_FLASH_SPI_GPIO_CLK,ENABLE);
	//引脚复用
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_MOSI_PINSOURCE,BSP_FLASH_SPI_AF);
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_MISO_PINSOURCE,BSP_FLASH_SPI_AF);
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_SCK_PINSOURCE,BSP_FLASH_SPI_AF);
	
	//复用推挽
	GPIO_InitStruct.GPIO_Pin=BSP_FLASH_SPI_SCK_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BSP_FLASH_SPI_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_FLASH_SPI_MISO_PIN;
	GPIO_Init(BSP_FLASH_SPI_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_FLASH_SPI_MOSI_PIN;
	GPIO_Init(BSP_FLASH_SPI_GPIO,&GPIO_InitStruct);
	//CS作为片选线则设置为输出模式
	GPIO_InitStruct.GPIO_Pin=BSP_FLASH_SPI_CS_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(BSP_FLASH_SPI_GPIO,&GPIO_InitStruct);
	
	//停止信号 CS引脚置高
	BSP_SPI_FLASH_CS_HIGH();
	//配置SPI
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//主机端
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//8位数据帧
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;//CPOL 时钟极性置高
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;//CPHA 时钟相位 偶数边沿
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;;//软件控制
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//2分频 f_sck=f_pclk / 分频值
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB先行  高位先发
	SPI_InitStruct.SPI_CRCPolynomial=7;//不使用CRC校验
	SPI_Init(BSP_FLASH_SPI,&SPI_InitStruct);
	
	//使能SPI FLASH
	SPI_Cmd(BSP_FLASH_SPI,ENABLE);
	//使SPI FLASH进入4字节地址模式
	BSP_SPI_FLASH_Mode_Init();
	
}

	/**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t BSP_SPI_FLASH_SendByte(uint8_t byte)
{
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(0);
	}
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(BSP_FLASH_SPI,byte);
	SPITimeout=SPIT_FLAG_TIMEOUT;
	
	/* 等待接收缓冲区非空，RXNE事件 */
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(1);
	}
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(BSP_FLASH_SPI);
		
}

/**
  * @brief  使用SPI发送半个字的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */

uint16_t BSP_SPI_FLASH_SendHalfByte(uint16_t byte)
{
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(2);
	}
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(BSP_FLASH_SPI,byte);
	SPITimeout=SPIT_FLAG_TIMEOUT;
	
		/* 等待接收缓冲区非空，RXNE事件 */
	while(SPI_I2S_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(3);
	}	
	
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(BSP_FLASH_SPI);
	
}



 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */

void BSP_SPI_FLASH_WritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte)
{
	/* 发送FLASH写使能命令 */
	BSP_SPI_FLASH_WriteEnable();
	/* 选择FLASH: CS低电平 */
	BSP_SPI_FLASH_CS_LOW();
	/* 页写指令*/
	BSP_SPI_FLASH_SendByte(W25X_PageProgram);
	/*发送写地址的高8位*/
	BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF000000)>>24);
	/*发送写地址的中前8位*/
  BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中后8位*/
  BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低8位*/
  BSP_SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	if(NumByte>BSP_SPI_FLASH_PerWritePageSize)
	{
		NumByte=BSP_SPI_FLASH_PerWritePageSize;
		BSP_FLASH_ERROR("SPI_FLASH_PageWrite! 所需写入空间过大");
	}
	/* 写入数据*/
	while(NumByte--)
	{
		/* 发送当前要写入的字节数据 */
		BSP_SPI_FLASH_SendByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;	
	}
	
	 /* 停止信号 FLASH: CS 高电平 */
  BSP_SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
   BSP_SPI_FLASH_WaitForWriteEnd();
}



 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */

void BSP_SPI_FLASH_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte)
{
	
	uint8_t Num_Page=0,Num_Single=0,Addr=0,count=0,Temp=0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
	Addr=WriteAddr % BSP_SPI_FLASH_PageSize;
	/*差count个数据值未写入*/
	count=BSP_SPI_FLASH_PageSize-Addr;
	/*计算出要写多少整数页*/
	Num_Page=NumByte / BSP_SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
	Num_Single=NumByte % BSP_SPI_FLASH_PageSize;
	
	if(Addr==0)
	{
		if(Num_Page==0)
		{
			//开始页写入  只需一页
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,NumByte);
		}
		else
		{
			while(NumByte--)//先写入整数页	
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,BSP_SPI_FLASH_PageSize);
				WriteAddr+=BSP_SPI_FLASH_PageSize;
				pBuffer+=BSP_SPI_FLASH_PageSize;
			}
			/*若有多余的不满一页的数据，把它写完*/
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,Num_Single);
		}
	}
	else
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if(Num_Page==0)
		{
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
			if(Num_Single>count)
			{
				Temp=Num_Single-count;
				/*先写满当前页*/
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,count);
				WriteAddr+=count;
				pBuffer+=count;
				/*再写剩余的数据*/
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,Temp);
			}
			else/*当前页剩余的count个位置能写完NumOfSingle个数据*/
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,NumByte);
			}			
		}
		else/* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*地址不对齐多出的count分开处理，不加入这个运算*/
			NumByte-=count;
			Num_Page=NumByte / BSP_SPI_FLASH_PageSize;
			Num_Single=NumByte % BSP_SPI_FLASH_PageSize;
			
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,count);
			WriteAddr+=count;
			pBuffer+=count;
			
			/*把整数页都写了*/
			while(Num_Page--)
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,BSP_SPI_FLASH_PageSize);
				WriteAddr+=BSP_SPI_FLASH_PageSize;
				pBuffer+=BSP_SPI_FLASH_PageSize;
			}
			if(Num_Single!=0)
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,Num_Single);
			}
		}
	}
}

/**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t BSP_SPI_FLASH_ReadByte(void)
{
	
	return (BSP_SPI_FLASH_SendByte(Dummy_Byte));
	
}

void BSP_SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* 开始通信 置低 */
  BSP_SPI_FLASH_CS_LOW();

  /* 读数据指令 */
  BSP_SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  BSP_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  BSP_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  BSP_SPI_FLASH_SendByte(ReadAddr & 0xFF);
}



/**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void BSP_SPI_FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByte)
{
	/* 选择FLASH: CS低电平 */
	BSP_SPI_FLASH_CS_LOW();
	/* 发送 读 指令 */
	BSP_SPI_FLASH_SendByte(W25X_ReadData);
	/* 发送 读 地址高8位 */
  BSP_SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* 发送 读 地址中前8位 */
  BSP_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中后8位 */
  BSP_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* 发送 读 地址低8位 */
  BSP_SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* 读取数据 */
  while (NumByte--)
  {
    /* 读取一个字节*/
    *pBuffer = BSP_SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t BSP_SPI_FLASH_ReadID(void)
{
	
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	/* 开始通讯：CS低电平 */
  BSP_SPI_FLASH_CS_LOW();
	
	/* 发送JEDEC指令，读取ID */
  BSP_SPI_FLASH_SendByte(W25X_JedecDeviceID);
	
	/* 读取一个字节数据 */
  Temp0 = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
	/* 读取一个字节数据 */
  Temp1 = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
  /* 读取一个字节数据 */
  Temp2 = BSP_SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  BSP_SPI_FLASH_CS_HIGH();

	/*把数据组合起来，作为函数的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
	
}

/**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
uint32_t BSP_SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;

  /* 开始通讯：CS低电平 */
  BSP_SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  BSP_SPI_FLASH_SendByte(W25X_DeviceID);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  
  /* 从FLASH中读取一个字节数据 */
  Temp = BSP_SPI_FLASH_SendByte(Dummy_Byte);
  printf("ReadDeviceID data: %d\n", Temp);
  /* 停止通讯：CS高电平 */
  BSP_SPI_FLASH_CS_HIGH();

  return Temp;
	
}

 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void BSP_SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	//发送写使能命令
	BSP_SPI_FLASH_WriteEnable();
	//等待写入结束
	BSP_SPI_FLASH_WaitForWriteEnd();
	//开始通讯 CS置低
	BSP_SPI_FLASH_CS_LOW();
	//发送扇区擦除指令
	BSP_SPI_FLASH_SendByte(W25X_SectorErase);
	//发送擦除扇区地址的高八位
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF000000)>>24);
	//发送擦除扇区地址的中前八位
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF0000)>>16);
	//发送擦除扇区地址的中后八位
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF00)>>8);
	//发送擦除扇区地址的低八位
	BSP_SPI_FLASH_SendByte(SectorAddr&0xFF);
	//停止信号
	BSP_SPI_FLASH_CS_HIGH();
	//等待写入结束
	BSP_SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void BSP_SPI_FLASH_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  BSP_SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  BSP_SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
   BSP_SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  BSP_SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  BSP_SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
  BSP_SPI_FLASH_CS_LOW();

  /* 发送 读状态寄存器 命令 */
  BSP_SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = BSP_SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        BSP_SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
  BSP_SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_WriteEnable(void)
{
	
	//通信开始
	BSP_SPI_FLASH_CS_LOW();
	//发送写使能命令
	BSP_SPI_FLASH_SendByte(W25X_WriteEnable);
	//通信结束
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  向FLASH发送 掉电 命令
  * @param  none
  * @retval none
  */

void BSP_SPI_FLASH_PowerDown(void)
{
	
	//通信开始
	BSP_SPI_FLASH_CS_LOW();
	//发送掉电命令
	BSP_SPI_FLASH_SendByte(W25X_PowerDown);
	//通信结束
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  向FLASH发送 唤醒 命令
  * @param  none
  * @retval none
  */

void BSP_SPI_FLASH_WakeUP(void)
{
	
	//通信开始
	BSP_SPI_FLASH_CS_LOW();
	//发送唤醒命令
	BSP_SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	//通信结束
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  使 SPI_FLASH 进入 4 字节地址模式
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*选择 FLASH: CS 低 */
	BSP_SPI_FLASH_CS_LOW();
	
	/* 发送状态寄存器 3 命令 */
	BSP_SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
	/* 停止信号 FLASH: CS 高 */
	BSP_SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*选择 FLASH: CS 低 */
		BSP_SPI_FLASH_CS_LOW();
		
		/* 进入4字节模式 */
		BSP_SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* 停止信号 FLASH: CS 高 */
		BSP_SPI_FLASH_CS_HIGH();
	}
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
uint16_t BSP_SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  BSP_FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}