#include "./Bsp/RTC/bsp_rtc_tim.h"

volatile unsigned long long FreeRTOSRunTimeTicks = 0;


void ConfigureTimerForRunTimeStats(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;		
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM_TimeBaseInitStruct.TIM_Period = 36-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 100-1;

	TIM_TimeBaseInit (TIM2,&TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init (&NVIC_InitStruct);
	
	TIM_ITConfig (TIM2,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit (TIM2,TIM_IT_Update);
	TIM_Cmd (TIM2,ENABLE);
}




