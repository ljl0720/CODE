#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f4xx.h"


#define BSP_I2C_SPEED        400000  //I2Cͨ������  ����ģʽ
#define BSP_I2C_PageSize     8       //��д���ҳ


#define BSP_I2C              I2C2

#define BSP_I2C_WR           0       /* д����bit */
#define BSP_I2C_RD           1       /* ������bit */

#define BSP_I2C_GPIO_PORT    GPIOH   /* GPIO�˿� H*/
#define BSP_I2C_GPIO_CLK     RCC_AHB1Periph_GPIOH  /* GPIO�˿�ʱ�� */
#define BSP_I2C_SDA_PIN      GPIO_Pin_5  /* ���ӵ�SDA�����ߵ�GPIO 5*/
#define BSP_I2C_SCL_PIN      GPIO_Pin_4  /* ���ӵ�SCLʱ���ߵ�GPIO 4 */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1
		
		#define BSP_I2C_SCL_1()  		GPIO_SetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SCL_PIN)            /* SCL = 1 */
		#define BSP_I2C_SCL_0()  		GPIO_ResetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SCL_PIN)          /* SCL = 0 */
		
		#define BSP_I2C_SDA_1()  		GPIO_SetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SDA_PIN)            /* SDA = 1 */
		#define BSP_I2C_SDA_0()  		GPIO_ResetBits(BSP_I2C_GPIO_PORT,BSP_I2C_SDA_PIN)          /* SDA = 0 */
		
		#define BSP_I2C_SDA_READ()  GPIO_ReadInputDataBit(BSP_I2C_GPIO_PORT, BSP_I2C_SDA_PIN)	     /* ��SDA����״̬ */
		
#else

		/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
		#define EEPROM_I2C_SCL_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
		#define EEPROM_I2C_SCL_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
		#define EEPROM_I2C_SDA_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
		#define EEPROM_I2C_SDA_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
		#define EEPROM_I2C_SDA_READ()  ((EEPROM_I2C_GPIO_PORT->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */

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