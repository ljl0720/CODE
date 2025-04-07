#ifndef __BSP_PWR_STOP_H
#define __BSP_PWR_STOP_H

#include "stm32f4xx.h"
#include "./Bsp/usart1/bsp_usart.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOH
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOH
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_2
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOH
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource2
#define KEY1_INT_EXTI_LINE                EXTI_Line2
#define KEY1_INT_EXTI_IRQ                 EXTI2_IRQn

#define KEY1_IRQHandler                   EXTI2_IRQHandler

#define KEY2_INT_GPIO_PORT                GPIOC
#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define KEY2_INT_GPIO_PIN                 GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource13
#define KEY2_INT_EXTI_LINE                EXTI_Line13
#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define KEY2_IRQHandler                   EXTI15_10_IRQHandler


void BSP_PWR_STOP_TEST(void);
void BSP_PWR_NVIC_Configuration(void);
void BSP_PWR_EXTI_Key_Config(void);
void BSP_SYSCLKConfig_STOP(void);
void Delay_PWR(__IO uint32_t nCount);
#endif