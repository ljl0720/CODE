#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f4xx.h"


#define BSP_I2C_SPEED        400000  //I2C通信速率  快速模式
#define BSP_I2C_PageSize     8       //可写入的页


#define BSP_I2C              I2C2

#define BSP_I2C_WR           0       /* 写控制bit */
#define BSP_I2C_RD           1       /* 读控制bit */

#define BSP_I2C_GPIO_PORT    GPIOH   /* GPIO端口 H*/
#define BSP_I2C_GPIO_CLK     RCC_AHB1Periph_GPIOH  /* GPIO端口时钟 */
#define BSP_I2C_SDA_PIN      GPIO_Pin_5  /* 连接到SDA数据线的GPIO 5*/
#define BSP_I2C_SCL_PIN      GPIO_Pin_4  /* 连接到SCL时钟线的GPIO 4 */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1
		
		#define BSP_I2C_SCL_1()  		GPIO_SetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SCL_PIN)            /* SCL = 1 */
		#define BSP_I2C_SCL_0()  		GPIO_ResetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SCL_PIN)          /* SCL = 0 */
		
		#define BSP_I2C_SDA_1()  		GPIO_SetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SDA_PIN)            /* SDA = 1 */
		#define BSP_I2C_SDA_0()  		GPIO_ResetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SDA_PIN)          /* SDA = 0 */
		
		#define BSP_I2C_SDA_READ()  GPIO_ReadInputDataBit(BSP_I2C_GPIO_PORT, BSP_I2C_SDA_PIN)	     /* 读SDA口线状态 */
		
#else

		/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
		#define EEPROM_I2C_SCL_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
		#define EEPROM_I2C_SCL_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
		#define EEPROM_I2C_SDA_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
		#define EEPROM_I2C_SDA_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
		#define EEPROM_I2C_SDA_READ()  ((EEPROM_I2C_GPIO_PORT->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */

#endif

void BSP_I2C_Start(void);
void BSP_I2C_Stop(void);
void BSP_I2C_SendByte(uint8_t _uByte);
uint8_t BSP_I2C_ReadByte(void);
uint8_t BSP_I2C_WaitACK(void);
void BSP_I2C_Ack(void);
uint8_t BSP_I2C_Nack(void);
void BSP_I2C_GPIO_Config(void);
uint8_t BSP_I2C_CheckDevice(uint8_t _Address);
#endif