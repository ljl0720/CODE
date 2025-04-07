/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

#include "./Bsp/usart1/bsp_usart.h"
#include "./Bsp/RTC/bsp_rtc_tim.h"
#include "./Bsp/PWR_STOP/bsp_pwr_stop.h"
#include "./Bsp/STemWIN_TASK/Win_LoginDLG.h"

#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#include "semphr.h"
#include "GUI.h"
#include "DIALOG.h"
#include "GUITP.h"

extern volatile unsigned long long FreeRTOSRunTimeTicks;
extern int flag_login,flag_keyboard;
extern struct Login_Value LV;
extern struct Keyboard_Value KV;
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/


extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
			
}
/*********************************************************************************
  * @ 函数名  ： DEBUG_USART_IRQHandler
  * @ 功能说明： 串口中断服务函数
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  ********************************************************************************/
void BSP_USART1_IRQHandler(void)
{
  uint32_t ulReturn;
  /* 进入临界段，临界段可以嵌套 */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(USART_GetITStatus(BSP_USART1,USART_IT_IDLE)!=RESET)
	{		
		BSP_USART1_DMA_Rx_Data();       /* 释放一个信号量，表示数据已接收 */
		
		uint16_t USART1_RecvData=USART_ReceiveData(BSP_USART1);
		
	/* 等待接收数据寄存器非空 */
	while(USART_GetFlagStatus(BSP_USART1,USART_FLAG_RXNE)!=RESET){}
		
		printf("收到数据ISR:%s\n",USART1_RecvData);
		
		
		USART_ReceiveData(BSP_USART1); /* 清除标志位 */
		printf("清除标志位成功\n");
	}	 
  printf("退出\n");
  /* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*********************************************************************************
  * @ 函数名  ： BSP_RTC_TIM_IRQHandle
  * @ 功能说明： RTC计时中断服务函数
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  ********************************************************************************/
void BSP_RTC_TIM_IRQHandle(void)
{
	if (TIM_GetITStatus (TIM2,TIM_IT_Update) == SET)
	{
		FreeRTOSRunTimeTicks++;
	}
		TIM_ClearITPendingBit (TIM2,TIM_IT_Update);
}

/*********************************************************************************
  * @ 函数名  ： EXTI2_IRQHandler
  * @ 功能说明： PWR停止模式中断服务函数
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  ********************************************************************************/
void	KEY1_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE)!=RESET)
   {
		 EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	 }
	
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
