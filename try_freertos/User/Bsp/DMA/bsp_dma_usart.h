#ifndef __BSP_DMA_USART_H
#define __BSP_DMA_USART_H

#include "stm32f4xx.h"

#define BSP_USART1_DMA_CLK 		     RCC_AHB1Periph_DMA2
#define BSP_USART1_DMA_CHANNEL     DMA_Channel_4
#define BSP_USART1_RX_DMA_STREAM   DMA2_Stream2
#define BSP_USART1_TX_DMA_STREAM   DMA2_Stream7

#define BSP_USART1_DR              (USART1_BASE+0X04)
#define BSP_USART1_BufferSize      5000                                                                                                                                                                                                                                                                                                                      _DR          (USART1_BASE+

void BSP_USART1_DMA_Config(void);

#endif