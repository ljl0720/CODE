#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "stm32f4xx.h"
#include "stdio.h"

/* 获取缓冲区的长度 */
#define TxBufferSize1    (countof(TxBuffer1) - 1)
#define RxBufferSize1    (countof(TxBuffer1) - 1)
#define countof(a)       (sizeof(a) / sizeof(*(a)))
#define Flash_BufferSize (countof(Flash_Tx_Buffer)-1)

#define  Flash_WriteAddress     0x00000
#define  Flash_ReadAddress      Flash_WriteAddress
#define  Flash_SectorToErase    Flash_WriteAddress

#define  BSP_FLASH_ID                        0XEF4018     //W25Q128
#define  BSP_FLASH_ID256                     0XEF4019     //W25Q256

#define BSP_SPI_FLASH_PageSize              256
#define BSP_SPI_FLASH_PerWritePageSize      256

#define W25X_WriteEnable		          0x06    //写使能
#define W25X_WriteDisable		          0x04    //写失能
#define W25X_ReadStatusReg		        0x05    //
#define W25X_WriteStatusReg		        0x01 
#define W25X_ReadData			            0x03
#define W25X_FastReadData		          0x0B 
#define W25X_FastReadDual		          0x3B 
#define W25X_PageProgram		          0x02 
#define W25X_BlockErase			          0xD8 
#define W25X_SectorErase		          0x20 
#define W25X_ChipErase			          0xC7 
#define W25X_PowerDown			          0xB9 
#define W25X_ReleasePowerDown	        0xAB 
#define W25X_DeviceID			            0xAB 
#define W25X_ManufactDeviceID    	  	0x90 
#define W25X_JedecDeviceID		        0x9F 
#define W25X_Enter4ByteMode		        0xB7
#define W25X_ReadStatusRegister3      0x15

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF

#define BSP_FLASH_SPI                           SPI5
#define BSP_FLASH_SPI_CLK                       RCC_APB2Periph_SPI5
#define BSP_FLASH_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
#define BSP_FLASH_SPI_AF                        GPIO_AF_SPI5
#define BSP_FLASH_SPI_GPIO                      GPIOF
#define BSP_FLASH_SPI_GPIO_CLK                  RCC_AHB1Periph_GPIOF

#define BSP_FLASH_SPI_SCK_PIN                   GPIO_Pin_7 
#define BSP_FLASH_SPI_SCK_PINSOURCE             GPIO_PinSource7

#define BSP_FLASH_SPI_MISO_PIN                  GPIO_Pin_8
#define BSP_FLASH_SPI_MISO_PINSOURCE            GPIO_PinSource8

#define BSP_FLASH_SPI_MOSI_PIN                  GPIO_Pin_9 
#define BSP_FLASH_SPI_MOSI_PINSOURCE            GPIO_PinSource9

#define BSP_FLASH_SPI_CS_PIN                    GPIO_Pin_6
#define BSP_FLASH_SPI_CS_PINSOURCE              GPIO_PinSource6


#define BSP_SPI_FLASH_CS_LOW()      {BSP_FLASH_SPI_GPIO->BSRRH=BSP_FLASH_SPI_CS_PIN;}
#define BSP_SPI_FLASH_CS_HIGH()     {BSP_FLASH_SPI_GPIO->BSRRL=BSP_FLASH_SPI_CS_PIN;}

/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define BSP_FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define BSP_FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define BSP_FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


																					
void BSP_SPI_FLASH_Config(void);																					
uint8_t BSP_SPI_FLASH_SendByte(uint8_t byte);
uint16_t BSP_SPI_FLASH_SendHalfByte(uint16_t byte);																					
void BSP_SPI_FLASH_WritePage(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte);																					
void BSP_SPI_FLASH_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByte);
																					
uint8_t BSP_SPI_FLASH_ReadByte(void);
void BSP_SPI_FLASH_StartReadSequence(uint32_t ReadAddr);																					
void BSP_SPI_FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByte);																					
uint32_t BSP_SPI_FLASH_ReadID(void);																					
uint32_t BSP_SPI_FLASH_ReadDeviceID(void);
																					
void BSP_SPI_FLASH_SectorErase(uint32_t SectorAddr);																					
void BSP_SPI_FLASH_WaitForWriteEnd(void);																					
void BSP_SPI_FLASH_WriteEnable(void);																					
void BSP_SPI_FLASH_PowerDown(void);																					
void BSP_SPI_FLASH_WakeUP(void);																			
void BSP_SPI_FLASH_Mode_Init(void);

uint16_t BSP_SPI_TIMEOUT_UserCallback(uint8_t errorCode);																					
#endif