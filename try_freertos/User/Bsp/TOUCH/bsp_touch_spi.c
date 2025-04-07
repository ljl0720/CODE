#include "bsp_touch_spi.h"
#include "./Bsp/SPI_FLASH/bsp_spi_flash.h"



static __IO uint32_t  TouchTimeout = SPIT_LONG_TIMEOUT;  

SPI_InitTypeDef  SPI_InitStruct;

uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t RecvBuff[SENDBUFF_SIZE];


 /**
  * @brief  SPI_TOUCH��ʼ��
  * @param  ��
  * @retval ��
  */

void BSP_SPI_TOUCH_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(BSP_TOUCH_SPI_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(BSP_TOUCH_SPI_GPIOA_CLK|BSP_TOUCH_SPI_GPIOI_CLK,ENABLE);
	
	GPIO_PinAFConfig(BSP_TOUCH_SPI_GPIOA,BSP_TOUCH_SPI_MISO_PINSOURCE,BSP_TOUCH_SPI_AF);
	GPIO_PinAFConfig(BSP_TOUCH_SPI_GPIOA,BSP_TOUCH_SPI_MOSI_PINSOURCE,BSP_TOUCH_SPI_AF);
	GPIO_PinAFConfig(BSP_TOUCH_SPI_GPIOA,BSP_TOUCH_SPI_SCK_PINSOURCE,BSP_TOUCH_SPI_AF);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_SPI_SCK_PIN;
	GPIO_Init(BSP_TOUCH_SPI_GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_SPI_MISO_PIN;
	GPIO_Init(BSP_TOUCH_SPI_GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=BSP_TOUCH_SPI_MOSI_PIN;
	GPIO_Init(BSP_TOUCH_SPI_GPIOA,&GPIO_InitStruct);
	
	
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//8λ����֡
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//������
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB����  ��λ�ȷ�
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStruct.SPI_CRCPolynomial=7;//��ʹ��CRCУ��
	SPI_Init(BSP_TOUCH_SPI,&SPI_InitStruct);
		
  //ʹ��SPI FLASH
	SPI_Cmd(BSP_TOUCH_SPI,ENABLE);
	//��������	
	BSP_SPI_TOUCH_SendByte(0xff);
}

	/**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t BSP_SPI_TOUCH_SendByte(uint8_t byte)
{
	
	TouchTimeout=SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	

	
	while(SPI_I2S_GetFlagStatus(BSP_TOUCH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((TouchTimeout--) == 0) return BSP_TOUCH_TIMEOUT_UserCallback(20);
	}
	uint8_t Rxdata;
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	
	SPI_I2S_SendData(BSP_TOUCH_SPI,byte);
	TouchTimeout=SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ջ������ǿգ�RXNE�¼� */
	while(SPI_I2S_GetFlagStatus(BSP_TOUCH_SPI,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((TouchTimeout--) == 0) return BSP_TOUCH_TIMEOUT_UserCallback(21);
	}
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(BSP_TOUCH_SPI);
		
}

 /**
  * @brief  �����ٶ�
  * @param  ��
  * @retval ��
  */
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	  SPI1->CR1&=0xFFC7;
	  SPI1->CR1|=SPI_BaudRatePrescaler;
//    SPI_Cmd(BSP_TOUCH_SPI,DISABLE);            //�ر�SPI
//	  SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler;
//	  SPI_Init(BSP_TOUCH_SPI,&SPI_InitStruct);
    SPI_Cmd(BSP_TOUCH_SPI,ENABLE);  
   
} 

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
uint16_t BSP_TOUCH_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  BSP_TOUCH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}