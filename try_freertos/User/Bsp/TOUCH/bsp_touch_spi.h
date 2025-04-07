#ifndef __BSP_TOUCH_SPI_H
#define __BSP_TOUCH_SPI_H

#include "stm32f4xx.h"



#define BSP_TOUCH_SPI                           SPI1
#define BSP_TOUCH_SPI_CLK                       RCC_APB2Periph_SPI1
#define BSP_TOUCH_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
#define BSP_TOUCH_SPI_AF                        GPIO_AF_SPI1
#define BSP_TOUCH_SPI_GPIOA                     GPIOA
#define BSP_TOUCH_SPI_GPIOI                     GPIOI
#define BSP_TOUCH_SPI_GPIOA_CLK                 RCC_AHB1Periph_GPIOA
#define BSP_TOUCH_SPI_GPIOI_CLK                 RCC_AHB1Periph_GPIOI

#define BSP_TOUCH_SPI_SCK_PIN                   GPIO_Pin_5 
#define BSP_TOUCH_SPI_SCK_PINSOURCE             GPIO_PinSource5

#define BSP_TOUCH_SPI_MISO_PIN                  GPIO_Pin_6
#define BSP_TOUCH_SPI_MISO_PINSOURCE            GPIO_PinSource6

#define BSP_TOUCH_SPI_MOSI_PIN                  GPIO_Pin_7 
#define BSP_TOUCH_SPI_MOSI_PINSOURCE            GPIO_PinSource7

#define BSP_TOUCH_SPI_CS_PIN                    GPIO_Pin_8
#define BSP_TOUCH_SPI_CS_PINSOURCE              GPIO_PinSource8


#define BSP_SPI_TOUCH_CS_LOW()      {BSP_FLASH_SPI_GPIO->BSRRH=BSP_FLASH_SPI_CS_PIN;}
#define BSP_SPI_TOUCH_CS_HIGH()     {BSP_FLASH_SPI_GPIO->BSRRL=BSP_FLASH_SPI_CS_PIN;}

/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define TOUCH_DEBUG_ON         1

#define BSP_TOUCH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define BSP_TOUCH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define BSP_TOUCH_DEBUG(fmt,arg...)          do{\
                                          if(TOUCH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define SENDBUFF_SIZE                     		10//发送的数据量
																					
void BSP_SPI_TOUCH_Init(void);																		
uint8_t BSP_SPI_TOUCH_SendByte(uint8_t byte);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);
uint16_t BSP_TOUCH_TIMEOUT_UserCallback(uint8_t errorCode);
#endif