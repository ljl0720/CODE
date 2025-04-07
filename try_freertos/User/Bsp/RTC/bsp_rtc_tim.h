#ifndef __BSP_RTC_TIM_H
#define __BSP_RTC_TIM_H

#include "stm32f4xx.h"

#define   BSP_RTC_TIM_IRQHandle           TIM2_IRQHandler   

void ConfigureTimerForRunTimeStats(void);
#endif