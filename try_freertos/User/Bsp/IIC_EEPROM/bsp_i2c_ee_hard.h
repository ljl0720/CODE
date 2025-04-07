#ifndef __BSP_I2C_EE_HARD_H
#define __BSP_I2C_EE_HARD_H

#include "stm32f4xx.h"


/* AT24C01/02每页有8个字节 */
#define BSP_I2C_PageSize           8

/* STM32 I2C 快速模式 */
#define BSP_I2C_Speed              400000

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define BSP_I2C_OWN_ADDRESS7      0X0A

#define BSP_HARD_I2C                          I2C2
#define BSP_HARD_I2C_CLK                      RCC_APB1Periph_I2C2
#define BSP_HARD_I2C_CLK_INIT								  RCC_APB1PeriphClockCmd
#define BSP_HARD_I2C_AF                       GPIO_AF_I2C2
#define BSP_HARD_I2C_GPIO_CLK                 RCC_AHB1Periph_GPIOH

#define BSP_HARD_I2C_SCL_PIN                  GPIO_Pin_4                 
#define BSP_HARD_I2C_SCL_GPIO                 GPIOH                       
#define BSP_HARD_I2C_SCL_SOURCE               GPIO_PinSource4


#define BSP_HARD_I2C_SDA_PIN                  GPIO_Pin_5                  
#define BSP_HARD_I2C_SDA_GPIO                 GPIOH                       
#define BSP_HARD_I2C_SDA_SOURCE               GPIO_PinSource5

/*等待超时时间*/
#define BSP_I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define BSP_I2CT_LONG_TIMEOUT         ((uint32_t)(10 * BSP_I2CT_FLAG_TIMEOUT))

/*信息输出*/
#define BSP_DEBUG_ON         0

#define BSP_EE_HARD_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define BSP_EE_HARD_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define BSP_EE_HARD_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

																					
#define DOUBLE_ADDR       0
#define LONGINT_ADDR      10
#define CHAR_ADDR         60
																			
#define EEPROM_Block0_ADDRESS 0xA0   
																					
void BSP_I2C_EE_API(uint8_t *SaveBuffer);
void BSP_I2C_EE_Init(void);																					
uint32_t BSP_I2C_EE_WritePage(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByte);																					
uint32_t BSP_I2C_EE_WriteByte(uint8_t *pBuffer, uint8_t WriteAddr);																					
void BSP_I2C_EE_WriteBuffer(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByte);
uint32_t BSP_I2C_EE_ReadBuffer(uint8_t *pBuffer,uint8_t ReadAddr,uint16_t NumByte);			
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);																					
void I2C_EE_WaitEepromStandbyState(void); 	
	
#endif