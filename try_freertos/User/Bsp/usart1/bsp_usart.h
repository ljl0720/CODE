#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdlib.h"
/*  
***  
***
*/

#define BSP_USART1_BaudRate  					 115200
#define BSP_USART1_BufferSize          1000

#define BSP_USART1  									 USART1
#define BSP_USART1_CLK 								 RCC_APB2Periph_USART1
#define BSP_USART1_GPIO_PORT_CLK 			 RCC_AHB1Periph_GPIOA

#define BSP_USART1_TX_GPIO						 GPIOA
#define BSP_USART1_TX_PIN							 GPIO_Pin_9
#define BSP_USART1_TX_AF							 GPIO_AF_USART1
#define BSP_USART1_TX_SOURCE           GPIO_PinSource9

#define BSP_USART1_RX_GPIO             GPIOA
#define BSP_USART1_RX_PIN              GPIO_Pin_10
#define BSP_USART1_RX_AF               GPIO_AF_USART1
#define BSP_USART1_RX_SOURCE           GPIO_PinSource10

#define BSP_USART1_IRQHandler          USART1_IRQHandler
#define BSP_USART1_IRQChannel          USART1_IRQn



#define BSP_USART1_DMA_CLK 		    		 RCC_AHB1Periph_DMA2
#define BSP_USART1_DMA_CHANNEL     		 DMA_Channel_4
#define BSP_USART1_RX_DMA_STREAM  		 DMA2_Stream2
#define BSP_USART1_TX_DMA_STREAM  		 DMA2_Stream2

//#define BSP_USART1_DR             		 (USART1_BASE+0X04) 
#define BSP_USART1_DR                  (&BSP_USART1->DR)

#define BSP_USART1_RECV_LEN            200

void BSP_USART_GPIO_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void BSP_USART1_SendByte(USART_TypeDef *pUSARTx,uint8_t ch);
void BSP_USART1_SendString(USART_TypeDef *pUSARTx, char *str);
uint16_t BSP_USART1_RecvByte(USART_TypeDef *pUSARTx);
char* BSP_USART1_RecvString(USART_TypeDef *pUSARTx,char *str);
void BSP_USART1_DMA_Rx_Data(void);
void BSP_USART1_DMA_Config(void);
#endif