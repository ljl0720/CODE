#include "./Bsp/PWR_STOP/bsp_pwr_stop.h"

RCC_ClocksTypeDef clock_status_wakeup,clock_status_config;//定义唤醒和是中华状态的结构体
uint8_t clock_source_wakeup,clock_source_config;//保存时钟唤醒和时钟资源的变量
	
	
void BSP_PWR_STOP_TEST(void)
{	
	Delay_PWR(0x3FFFFFF);	
	
  /*设置停止模式时，FLASH进入掉电状态*/
  PWR_FlashPowerDownCmd (ENABLE);
	
	/* 进入停止模式，设置电压调节器为低功耗模式，等待中断唤醒 */
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
	
	/*********************被唤醒***********************/
	//时钟源
	clock_source_wakeup = RCC_GetSYSCLKSource ();
	
	//时钟频率
	RCC_GetClocksFreq(&clock_status_wakeup);
	
	//从停止模式下被唤醒后使用的是HSI时钟，此处重启HSE时钟,使用PLLCLK
	BSP_SYSCLKConfig_STOP();
	
	//获取重新配置后的时钟状态	
	//时钟源
	clock_source_config = RCC_GetSYSCLKSource ();
	//时钟频率
	RCC_GetClocksFreq(&clock_status_config);
	
	//因为刚唤醒的时候使用的是HSI时钟，会影响串口波特率，输出不对，所以在重新配置时钟源后才使用串口输出。
		printf("\r\n重新配置后的时钟状态：\r\n");
		printf(" SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d (0表示HSI，8表示PLLCLK)\n", 
			clock_status_config.SYSCLK_Frequency, 
			clock_status_config.HCLK_Frequency, 
			clock_status_config.PCLK1_Frequency, 
			clock_status_config.PCLK2_Frequency, 
			clock_source_config);
			
		printf("\r\n刚唤醒的时钟状态：\r\n");	
		printf(" SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d (0表示HSI，8表示PLLCLK)\n", 
			clock_status_wakeup.SYSCLK_Frequency, 
			clock_status_wakeup.HCLK_Frequency, 
			clock_status_wakeup.PCLK1_Frequency, 
			clock_status_wakeup.PCLK2_Frequency, 
			clock_source_wakeup);
			
			Delay_PWR(0x1FFFFFF);		
}	


/**
  * @brief  停机唤醒后配置系统时钟: 使能 HSE, PLL
  *         并且选择PLL作为系统时钟.
  * @param  None
  * @retval None
  */

void BSP_SYSCLKConfig_STOP(void)
{
	
	/* After wake-up from STOP reconfigure the system clock */
  /* 使能 HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* 等待 HSE 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	/* 使能 PLL */ 
  RCC_PLLCmd(ENABLE);
	 /* 等待 PLL 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

  /* 选择PLL作为系统时钟源 */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* 等待PLL被选择为系统时钟源 */
  while (RCC_GetSYSCLKSource() != 0x08);
}

	/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
void BSP_PWR_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级：2 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级：0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置 KEY1和KEY2为中断，并设置中断优先级
  * @param  无
  * @retval 无
  */
void BSP_PWR_EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
	/*开启按键GPIO口的时钟*/
	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK ,ENABLE);
  
  /* 使能 SYSCFG 时钟 ，使用GPIO外部中断时必须使能SYSCFG时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* 配置 NVIC */
  BSP_PWR_NVIC_Configuration();
  
	/* 选择按键1的引脚 */ 
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 设置引脚为输入模式 */ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
  /* 设置引脚不上拉也不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* 使用上面的结构体初始化按键 */
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure); 

	/* 连接 EXTI 中断源 到key1引脚 */
  SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);

  /* 选择 EXTI 中断源 */
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
  /* 中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* 下降沿触发 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  /* 使能中断/事件线 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* 选择按键2的引脚 */ 
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;  
  /* 其他配置与上面相同 */
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      

	/* 连接 EXTI 中断源 到key2 引脚 */
  SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);

  /* 选择 EXTI 中断源 */
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* 上升沿触发 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void Delay_PWR(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}