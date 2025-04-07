#include "bsp_i2c_ee_hard.h"
#include "./Bsp/usart1/bsp_usart.h"

__IO uint32_t  I2CTimeout = BSP_I2CT_LONG_TIMEOUT;   

uint16_t BSP_EEPROM_ADDRESS;//EEPROM�豸��ַ

long double Double_Buffer[7] = {0};//�洢С������

int Int_Bufffer[7] = {0};		//�洢��������

uint8_t Char_Buffer[7]={'\0'};//�洢�ַ�

uint8_t Ctl_Flag = 0;		

extern uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];
/**
  * @brief  I2C1 I/O����
  * @param  ��
  * @retval ��
  */
void BSP_EE_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(BSP_HARD_I2C_GPIO_CLK,ENABLE);
	BSP_HARD_I2C_CLK_INIT(BSP_HARD_I2C_CLK,ENABLE);
	
	GPIO_PinAFConfig(BSP_HARD_I2C_SCL_GPIO,BSP_HARD_I2C_SCL_SOURCE,BSP_HARD_I2C_AF);
	GPIO_PinAFConfig(BSP_HARD_I2C_SDA_GPIO,BSP_HARD_I2C_SDA_SOURCE,BSP_HARD_I2C_AF);
	//����ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	//���ģʽ
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	//������Ҳ������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=BSP_HARD_I2C_SCL_PIN;
	GPIO_Init(BSP_HARD_I2C_SCL_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_HARD_I2C_SDA_PIN;
	GPIO_Init(BSP_HARD_I2C_SDA_GPIO,&GPIO_InitStruct);
	
}

/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
void BSP_EE_I2C_Config(void)
{
	
	I2C_InitTypeDef  I2C_InitStruct;
	
	I2C_InitStruct.I2C_ClockSpeed=BSP_I2C_Speed;
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;//����ʱ��ռ�ձ� 2��1 LOW:HIGH
	I2C_InitStruct.I2C_OwnAddress1=BSP_I2C_OWN_ADDRESS7;//���������ַ Ψһ
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;//ʹ����Ӧ
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//Ѱַģʽ  ��ַ����Ϊ7bit
	I2C_Init(BSP_HARD_I2C,&I2C_InitStruct);
	I2C_Cmd(BSP_HARD_I2C,ENABLE); //ʹ��I2C
	
	I2C_AcknowledgeConfig(BSP_HARD_I2C,ENABLE);	//�Զ�Ӧ��ACK
		
}



/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾIIC��ȡʧ��.
  */
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  BSP_EE_HARD_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}

/**
  * @brief  �豸��æ���ȴ����� 
  * @param  ��
  * @retval ��
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    //������ʼ�ź�
    I2C_GenerateSTART(BSP_HARD_I2C, ENABLE);
    //��ȡI2C2��״̬�Ĵ���SR1����ֵ
    SR1_Tmp = I2C_ReadRegister(BSP_HARD_I2C, I2C_Register_SR1);
    //����EEPROM�豸��ַ
    I2C_Send7bitAddress(BSP_HARD_I2C, BSP_EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(BSP_HARD_I2C, I2C_Register_SR1) & 0x0002));
  
  //�����־λ
  I2C_ClearFlag(BSP_HARD_I2C, I2C_FLAG_AF);
 //����ֹͣ�ź�    
  I2C_GenerateSTOP(BSP_HARD_I2C, ENABLE); 
}



/**
  * @brief  I2C ����(EEPROM)��ʼ��
  * @param  ��
  * @retval ��
  */
void BSP_I2C_EE_Init(void)
{
	BSP_EE_GPIO_Config();
	BSP_EE_I2C_Config();
	
  BSP_EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
}

/**
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t BSP_I2C_EE_WritePage(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByte)
{
	
	I2CTimeout=BSP_I2CT_LONG_TIMEOUT;
	//�ж������Ƿ��ڷ�æ
	while(I2C_GetFlagStatus(BSP_HARD_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(4);
		
	}
	//��ʼ�ź�
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(5);
	}
	//�����豸��ַ
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(6);
	}
	//���������ַ
	I2C_SendData(BSP_HARD_I2C,WriteAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV8�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(7);
	}
	while(NumByte--)
	{
		I2C_SendData(BSP_HARD_I2C,*pBuffer);
		pBuffer++;
		I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
		//���EV8�¼�
		while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(8);
	}
	}
	//ֹͣ�ź�
	I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
	return 1;
}

/**
  * @brief   дһ���ֽڵ�I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ 
  * @retval  ��
  */
uint32_t BSP_I2C_EE_WriteByte(uint8_t *pBuffer, uint8_t WriteAddr)
{
	
	//��ʼ�ź�
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//���EV5�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(0);
	}
	//�����豸��ַ
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(1);
	}
	
	//���������ַ
	I2C_SendData(BSP_HARD_I2C,WriteAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//���EV8�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(2);
	}
	//����һ���ֽ�����
	I2C_SendData(BSP_HARD_I2C,*pBuffer);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	
	//���EV8�¼�
		while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
	}
	//ֹͣ�ź�
	I2C_GenerateSTOP(BSP_HARD_I2C,ENABLE);
	return 1;
	
}


/**
  * @brief   ���������е�����д��I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
void BSP_I2C_EE_WriteBuffer(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByte)
{
	//����ҳ���� �������� ��ַ �ֽ���
	uint8_t Num_Page=0,Num_Signel=0,Addr=0,count=0;
	//����д���ַ
	Addr=WriteAddr % BSP_I2C_PageSize;
	//������������ҳ��
	count=BSP_I2C_PageSize-Addr;
	//������Ҫ����ҳ   //��ʹ��ҳ����=��д������/����ҳ�ܴ洢���ݴ�С               ȡ��
	Num_Page=NumByte / BSP_I2C_PageSize;
	//����ʣ�಻��һҳ������    //��ʹ�õ����ֽ�����=��д������%����ҳ�ܴ洢���ݴ�С         ȡ��
	Num_Signel=NumByte % BSP_I2C_PageSize;
	
	if(Addr==0)//д���ַ�趨Ϊ0
	{
		if(Num_Page==0)//����������һҳ
		{
			BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//������д��һҳ
			I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
		}
		else//д���ַ��Ϊ0
		{
			while(Num_Page--)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,BSP_I2C_PageSize);//����ƫ�ƺ��д���ַд��һҳ����
				I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
				WriteAddr+=BSP_I2C_PageSize;//ƫ��д���ĵ�ַ
				pBuffer+=BSP_I2C_PageSize;//����ƫ�ƣ�׼���´�д���ַ
			}
			if(Num_Signel!=0)//�в���һҳ���ݴ�д��
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//��ƫ�ƺ�ĵ�ַд��ʣ������
				I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
			}
		}
	}
	else//д���ַ�趨��Ϊ0
	{
		if(Num_Page==0)//δ��һҳ����
		{
			BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//������д��һҳ
			I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
		}
		else//����������һҳ
		{
			NumByte-=count;//��ȡʣ��
			Num_Page=NumByte / BSP_I2C_PageSize;//����д������ҳ��
			Num_Signel=NumByte % BSP_I2C_PageSize;//����ʣ�಻��һҳ������
			
			if(count!=0)//��Ҫ�����ҳ��
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,count);//д��
				I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
				WriteAddr+=count;//��������Ҫд��ҳ��
				pBuffer+=count;//				
			}
			while(Num_Page--)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,BSP_I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
				WriteAddr+=BSP_I2C_PageSize;
				pBuffer+=BSP_I2C_PageSize;
			}
			if(Num_Signel!=0)
			{
				BSP_I2C_EE_WritePage(pBuffer,WriteAddr,Num_Signel);//д��ʣ�಻��һҳ������
				I2C_EE_WaitEepromStandbyState();//�ȴ�EEPROM��æ����
			}
		}
	}
	
}

/**
  * @brief   ��EEPROM�����ȡһ������ 
  * @param   
  *		@arg pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *		@arg WriteAddr:�������ݵ�EEPROM�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */

uint32_t BSP_I2C_EE_ReadBuffer(uint8_t *pBuffer,uint8_t ReadAddr,uint16_t NumByte)
{
	
	I2CTimeout=BSP_I2CT_LONG_TIMEOUT;
	
	//�ж������Ƿ��ڷ�æ  //*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(BSP_HARD_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(9);
		
	}
	//��ʼ�ź�
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(10);
	}
	//����EEPROM��ַ
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Transmitter);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(11);
	}
	//ʹ��I2C����
	I2C_Cmd(BSP_HARD_I2C,ENABLE);
	//���ͽ��յ�ַ
	I2C_SendData(BSP_HARD_I2C,ReadAddr);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV8�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(12);
	}
	//��ʼ�ź�
	I2C_GenerateSTART(BSP_HARD_I2C,ENABLE);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(BSP_HARD_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(13);
	}
	
		//����EEPROM��ַ
	I2C_Send7bitAddress(BSP_HARD_I2C,BSP_EEPROM_ADDRESS,I2C_Direction_Receiver);
	
	I2CTimeout=BSP_I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
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
***  ����EEPROM��ȡд��ӿ�
***
*/
void BSP_I2C_EE_API(uint8_t *SaveBuffer)
{
	
	/*��ȡ���ݱ�־λ*/
  BSP_I2C_EE_ReadBuffer(&Ctl_Flag, 0, 1);
	
	if(Ctl_Flag!=0xAB)/*����־����0xcd����ʾ֮ǰ����д������*/
	{
		printf("\r\nû�м�⵽���ݱ�־��FLASHû�д洢����\r\n");
        Ctl_Flag =0xAB;
		
		/*д���־��0��ַ*/
        BSP_I2C_EE_WriteBuffer(&Ctl_Flag, 0, 1); 
		
	for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
        {
           Char_Buffer[I2C_EE_WriteBuffer_Num] = SaveBuffer[I2C_EE_WriteBuffer_Num];
        }
				
				
				/*д������*/
				BSP_I2C_EE_WriteBuffer((void*)SaveBuffer,CHAR_ADDR,sizeof(SaveBuffer));
				
				printf("��оƬд�����ݣ�");
				
				/*��ӡ������*/
  for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
        {
          printf("С��tx = %d\r\n",Char_Buffer[I2C_EE_WriteBuffer_Num]);
        }
				
	}else{
		      Ctl_Flag =0xA1;
		      printf("\r\n��⵽���ݱ�־\r\n");
		    /*��ȡ��������*/
        BSP_I2C_EE_ReadBuffer((void*)Char_Buffer, CHAR_ADDR, sizeof(Char_Buffer));
		
				printf("\r\n��оƬ�������ݣ�\r\n");	
		
		     for(int I2C_EE_WriteBuffer_Num=0; I2C_EE_WriteBuffer_Num<7; I2C_EE_WriteBuffer_Num++ )
				{
					printf("���� rx = %d \r\n",Char_Buffer[I2C_EE_WriteBuffer_Num]);				
				}
		
	}
				
}
