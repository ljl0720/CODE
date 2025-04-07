#include "bsp_touch_spi.h"
#include "./Bsp/SPI_FLASH/bsp_spi_flash.h"



static __IO uint32_t  TouchTimeout = SPIT_LONG_TIMEOUT;  

SPI_InitTypeDef  SPI_InitStruct;

uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t RecvBuff[SENDBUFF_SIZE];


 /**
  * @brief  SPI_TOUCH初始化
  * @param  无
  * @retval 无
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
	
	
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//8位数据帧
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//主机端
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//MSB先行  高位先发
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;//串行同步时钟的空闲状态为高电平
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStruct.SPI_CRCPolynomial=7;//不使用CRC校验
	SPI_Init(BSP_TOUCH_SPI,&SPI_InitStruct);
		
  //使能SPI FLASH
	SPI_Cmd(BSP_TOUCH_SPI,ENABLE);
	//启动传输	
	BSP_SPI_TOUCH_SendByte(0xff);
}

	/**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t BSP_SPI_TOUCH_SendByte(uint8_t byte)
{
	
	TouchTimeout=SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE事件 */
	

	
	while(SPI_I2S_GetFlagStatus(BSP_TOUCH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((TouchTimeout--) == 0) return BSP_TOUCH_TIMEOUT_UserCallback(20);
	}
	uint8_t Rxdata;
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	
	SPI_I2S_SendData(BSP_TOUCH_SPI,byte);
	TouchTimeout=SPIT_FLAG_TIMEOUT;
	
	/* 等待接收缓冲区非空，RXNE事件 */
	while(SPI_I2S_GetFlagStatus(BSP_TOUCH_SPI,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((TouchTimeout--) == 0) return BSP_TOUCH_TIMEOUT_UserCallback(21);
	}
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(BSP_TOUCH_SPI);
		
}

 /**
  * @brief  调整速度
  * @param  无
  * @retval 无
  */
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	  SPI1->CR1&=0xFFC7;
	  SPI1->CR1|=SPI_BaudRatePrescaler;
//    SPI_Cmd(BSP_TOUCH_SPI,DISABLE);            //关闭SPI
//	  SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler;
//	  SPI_Init(BSP_TOUCH_SPI,&SPI_InitStruct);
    SPI_Cmd(BSP_TOUCH_SPI,ENABLE);  
   
} 

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
uint16_t BSP_TOUCH_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  BSP_TOUCH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}