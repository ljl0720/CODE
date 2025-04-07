#include "./Bsp/SPI_FLASH/bsp_spi_flash.h"



static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

/* ���ͻ�������ʼ�� */
uint8_t Flash_Tx_Buffer[] = "SPI flash ����";
uint8_t Flash_Rx_Buffer[Flash_BufferSize];

//��ȡ��ID�洢λ��
__IO uint32_t Flash_DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO uint8_t TransferStatus1 = 0;


 /**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
	
void BSP_SPI_FLASH_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef  SPI_InitStruct;
	//ʹ��SPI5ʱ��
	BSP_FLASH_SPI_CLK_INIT(BSP_FLASH_SPI_CLK,ENABLE);
	//ʹ��SPI5��MISO��MOSI��CS��SCKʱ��
	RCC_AHB1PeriphClockCmd(BSP_FLASH_SPI_GPIO_CLK,ENABLE);
	//���Ÿ���
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_MOSI_PINSOURCE,BSP_FLASH_SPI_AF);
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_MISO_PINSOURCE,BSP_FLASH_SPI_AF);
	GPIO_PinAFConfig(BSP_FLASH_SPI_GPIO,BSP_FLASH_SPI_SCK_PINSOURCE,BSP_FLASH_SPI_AF);
	
	//��������
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
	//CS��ΪƬѡ��������Ϊ���ģʽ
	GPIO_InitStruct.GPIO_Pin=BSP_FLASH_SPI_CS_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(BSP_FLASH_SPI_GPIO,&GPIO_InitStruct);
	
	//ֹͣ�ź� CS�����ø�
	BSP_SPI_FLASH_CS_HIGH();
	//����SPI
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//������
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//8λ����֡
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;//CPOL ʱ�Ӽ����ø�
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;//CPHA ʱ����λ ż������
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;;//�������
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//2��Ƶ f_sck=f_pclk / ��Ƶֵ
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB����  ��λ�ȷ�
	SPI_InitStruct.SPI_CRCPolynomial=7;//��ʹ��CRCУ��
	SPI_Init(BSP_FLASH_SPI,&SPI_InitStruct);
	
	//ʹ��SPI FLASH
	SPI_Cmd(BSP_FLASH_SPI,ENABLE);
	//ʹSPI FLASH����4�ֽڵ�ַģʽ
	BSP_SPI_FLASH_Mode_Init();
	
}

	/**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t BSP_SPI_FLASH_SendByte(uint8_t byte)
{
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(0);
	}
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(BSP_FLASH_SPI,byte);
	SPITimeout=SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(1);
	}
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(BSP_FLASH_SPI);
		
}

/**
  * @brief  ʹ��SPI���Ͱ���ֵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */

uint16_t BSP_SPI_FLASH_SendHalfByte(uint16_t byte)
{
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	while(SPI_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(2);
	}
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(BSP_FLASH_SPI,byte);
	SPITimeout=SPIT_FLAG_TIMEOUT;
	
		/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while(SPI_I2S_GetFlagStatus(BSP_FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) return BSP_SPI_TIMEOUT_UserCallback(3);
	}	
	
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(BSP_FLASH_SPI);
	
}



 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */

void BSP_SPI_FLASH_WritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte)
{
	/* ����FLASHдʹ������ */
	BSP_SPI_FLASH_WriteEnable();
	/* ѡ��FLASH: CS�͵�ƽ */
	BSP_SPI_FLASH_CS_LOW();
	/* ҳдָ��*/
	BSP_SPI_FLASH_SendByte(W25X_PageProgram);
	/*����д��ַ�ĸ�8λ*/
	BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF000000)>>24);
	/*����д��ַ����ǰ8λ*/
  BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ���к�8λ*/
  BSP_SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�8λ*/
  BSP_SPI_FLASH_SendByte(WriteAddr & 0xFF);
	
	if(NumByte>BSP_SPI_FLASH_PerWritePageSize)
	{
		NumByte=BSP_SPI_FLASH_PerWritePageSize;
		BSP_FLASH_ERROR("SPI_FLASH_PageWrite! ����д��ռ����");
	}
	/* д������*/
	while(NumByte--)
	{
		/* ���͵�ǰҪд����ֽ����� */
		BSP_SPI_FLASH_SendByte(*pBuffer);
		/* ָ����һ�ֽ����� */
		pBuffer++;	
	}
	
	 /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  BSP_SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
   BSP_SPI_FLASH_WaitForWriteEnd();
}



 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */

void BSP_SPI_FLASH_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte)
{
	
	uint8_t Num_Page=0,Num_Single=0,Addr=0,count=0,Temp=0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
	Addr=WriteAddr % BSP_SPI_FLASH_PageSize;
	/*��count������ֵδд��*/
	count=BSP_SPI_FLASH_PageSize-Addr;
	/*�����Ҫд��������ҳ*/
	Num_Page=NumByte / BSP_SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
	Num_Single=NumByte % BSP_SPI_FLASH_PageSize;
	
	if(Addr==0)
	{
		if(Num_Page==0)
		{
			//��ʼҳд��  ֻ��һҳ
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,NumByte);
		}
		else
		{
			while(NumByte--)//��д������ҳ	
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,BSP_SPI_FLASH_PageSize);
				WriteAddr+=BSP_SPI_FLASH_PageSize;
				pBuffer+=BSP_SPI_FLASH_PageSize;
			}
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,Num_Single);
		}
	}
	else
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if(Num_Page==0)
		{
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
			if(Num_Single>count)
			{
				Temp=Num_Single-count;
				/*��д����ǰҳ*/
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,count);
				WriteAddr+=count;
				pBuffer+=count;
				/*��дʣ�������*/
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,Temp);
			}
			else/*��ǰҳʣ���count��λ����д��NumOfSingle������*/
			{
				BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,NumByte);
			}			
		}
		else/* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*��ַ����������count�ֿ������������������*/
			NumByte-=count;
			Num_Page=NumByte / BSP_SPI_FLASH_PageSize;
			Num_Single=NumByte % BSP_SPI_FLASH_PageSize;
			
			BSP_SPI_FLASH_WritePage(pBuffer,WriteAddr,count);
			WriteAddr+=count;
			pBuffer+=count;
			
			/*������ҳ��д��*/
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
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
uint8_t BSP_SPI_FLASH_ReadByte(void)
{
	
	return (BSP_SPI_FLASH_SendByte(Dummy_Byte));
	
}

void BSP_SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* ��ʼͨ�� �õ� */
  BSP_SPI_FLASH_CS_LOW();

  /* ������ָ�� */
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
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void BSP_SPI_FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByte)
{
	/* ѡ��FLASH: CS�͵�ƽ */
	BSP_SPI_FLASH_CS_LOW();
	/* ���� �� ָ�� */
	BSP_SPI_FLASH_SendByte(W25X_ReadData);
	/* ���� �� ��ַ��8λ */
  BSP_SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* ���� �� ��ַ��ǰ8λ */
  BSP_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ�к�8λ */
  BSP_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��8λ */
  BSP_SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* ��ȡ���� */
  while (NumByte--)
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = BSP_SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t BSP_SPI_FLASH_ReadID(void)
{
	
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	/* ��ʼͨѶ��CS�͵�ƽ */
  BSP_SPI_FLASH_CS_LOW();
	
	/* ����JEDECָ���ȡID */
  BSP_SPI_FLASH_SendByte(W25X_JedecDeviceID);
	
	/* ��ȡһ���ֽ����� */
  Temp0 = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
	/* ��ȡһ���ֽ����� */
  Temp1 = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
  /* ��ȡһ���ֽ����� */
  Temp2 = BSP_SPI_FLASH_SendByte(Dummy_Byte);

  /* ֹͣͨѶ��CS�ߵ�ƽ */
  BSP_SPI_FLASH_CS_HIGH();

	/*�����������������Ϊ�����ķ���ֵ*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
	
}

/**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
uint32_t BSP_SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  BSP_SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  BSP_SPI_FLASH_SendByte(W25X_DeviceID);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  BSP_SPI_FLASH_SendByte(Dummy_Byte);
  
  /* ��FLASH�ж�ȡһ���ֽ����� */
  Temp = BSP_SPI_FLASH_SendByte(Dummy_Byte);
  printf("ReadDeviceID data: %d\n", Temp);
  /* ֹͣͨѶ��CS�ߵ�ƽ */
  BSP_SPI_FLASH_CS_HIGH();

  return Temp;
	
}

 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void BSP_SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	//����дʹ������
	BSP_SPI_FLASH_WriteEnable();
	//�ȴ�д�����
	BSP_SPI_FLASH_WaitForWriteEnd();
	//��ʼͨѶ CS�õ�
	BSP_SPI_FLASH_CS_LOW();
	//������������ָ��
	BSP_SPI_FLASH_SendByte(W25X_SectorErase);
	//���Ͳ���������ַ�ĸ߰�λ
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF000000)>>24);
	//���Ͳ���������ַ����ǰ��λ
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF0000)>>16);
	//���Ͳ���������ַ���к��λ
	BSP_SPI_FLASH_SendByte((SectorAddr&0xFF00)>>8);
	//���Ͳ���������ַ�ĵͰ�λ
	BSP_SPI_FLASH_SendByte(SectorAddr&0xFF);
	//ֹͣ�ź�
	BSP_SPI_FLASH_CS_HIGH();
	//�ȴ�д�����
	BSP_SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void BSP_SPI_FLASH_BulkErase(void)
{
  /* ����FLASHдʹ������ */
  BSP_SPI_FLASH_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  BSP_SPI_FLASH_CS_LOW();
  /* �����������ָ��*/
   BSP_SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  BSP_SPI_FLASH_CS_HIGH();

  /* �ȴ��������*/
  BSP_SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  BSP_SPI_FLASH_CS_LOW();

  /* ���� ��״̬�Ĵ��� ���� */
  BSP_SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = BSP_SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        BSP_SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  BSP_SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_WriteEnable(void)
{
	
	//ͨ�ſ�ʼ
	BSP_SPI_FLASH_CS_LOW();
	//����дʹ������
	BSP_SPI_FLASH_SendByte(W25X_WriteEnable);
	//ͨ�Ž���
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  ��FLASH���� ���� ����
  * @param  none
  * @retval none
  */

void BSP_SPI_FLASH_PowerDown(void)
{
	
	//ͨ�ſ�ʼ
	BSP_SPI_FLASH_CS_LOW();
	//���͵�������
	BSP_SPI_FLASH_SendByte(W25X_PowerDown);
	//ͨ�Ž���
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  ��FLASH���� ���� ����
  * @param  none
  * @retval none
  */

void BSP_SPI_FLASH_WakeUP(void)
{
	
	//ͨ�ſ�ʼ
	BSP_SPI_FLASH_CS_LOW();
	//���ͻ�������
	BSP_SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	//ͨ�Ž���
	BSP_SPI_FLASH_CS_HIGH();
	
}

/**
  * @brief  ʹ SPI_FLASH ���� 4 �ֽڵ�ַģʽ
  * @param  none
  * @retval none
  */
void BSP_SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*ѡ�� FLASH: CS �� */
	BSP_SPI_FLASH_CS_LOW();
	
	/* ����״̬�Ĵ��� 3 ���� */
	BSP_SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = BSP_SPI_FLASH_SendByte(Dummy_Byte);
	
	/* ֹͣ�ź� FLASH: CS �� */
	BSP_SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*ѡ�� FLASH: CS �� */
		BSP_SPI_FLASH_CS_LOW();
		
		/* ����4�ֽ�ģʽ */
		BSP_SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* ֹͣ�ź� FLASH: CS �� */
		BSP_SPI_FLASH_CS_HIGH();
	}
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
uint16_t BSP_SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  BSP_FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}