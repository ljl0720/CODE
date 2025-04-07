#include "./Bsp/RTC/bsp_rtc.h"
#include "./Bsp/STemWIN_TASK/Win_LoginDLG.h"
#include "GUI.h"
#include "DIALOG.h"
#include "GUITP.h"
#include "string.h"
extern struct Canlender_Value    CANLV;
/**
  * @brief  设置时间和日期
  * @param  无
  * @retval 无
  */
	
void BSP_RTC_Config(void)
{
	printf("config\n");
//	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//初始化起始时间
	RTC_TimeStruct.RTC_H12=RTC_H12_AMorPM;
	RTC_TimeStruct.RTC_Hours=HOURS;
	RTC_TimeStruct.RTC_Minutes=MINUTES;
	RTC_TimeStruct.RTC_Seconds=SECONDS;
	RTC_SetTime(RTC_Format_BINorBCD,&RTC_TimeStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX,RTC_BKP_DATA);
	
	//初始化起始日期
	RTC_DateStruct.RTC_Date=DATE;
	RTC_DateStruct.RTC_Month=MONTH;
	RTC_DateStruct.RTC_WeekDay=WEEKDAY;
	RTC_DateStruct.RTC_Year=YEAR;
	RTC_SetDate(RTC_Format_BINorBCD,&RTC_DateStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX,RTC_BKP_DATA);
	
}
	
/**
  * @brief  显示时间和日期
  * @param  无
  * @retval 无
  */
void BSP_RTC_SHOW_DateTime(void)
{
	
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	
	
	//获取时间和日期 需要确定先后顺序防止错误
	RTC_GetTime(RTC_Format_BINorBCD,&RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BINorBCD,&RTC_DateStructure);
	
	
	
	//每秒输出一次	
	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
	{
		//先把要显示的数据用sprintf函数转换为字符串，然后才能用液晶显示函数显示
			sprintf(LCDTemp,"The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);
		
		printf("The Date :  Y:20 %0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);
		
		// 打印时间
      printf("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.RTC_Hours, 
			RTC_TimeStructure.RTC_Minutes, 
			RTC_TimeStructure.RTC_Seconds);		
		
		sprintf(CANLV.Day,sizeof(CANLV.Day),"%0.2d",RTC_DateStructure.RTC_Date);
	sprintf(CANLV.Month,sizeof(CANLV.Month),"%0.2d",RTC_DateStructure.RTC_Month);
	sprintf(CANLV.Year,sizeof(CANLV.Year),"%20%0.2d",RTC_DateStructure.RTC_Year);
	
	sprintf(CANLV.Hour,sizeof(CANLV.Hour),"%0.2d",RTC_TimeStructure.RTC_Hours);
	sprintf(CANLV.Minute,sizeof(CANLV.Minute),"%0.2d",RTC_TimeStructure.RTC_Minutes);
	sprintf(CANLV.Second,sizeof(CANLV.Second),"%20%0.2d",RTC_TimeStructure.RTC_Seconds);
		(void)RTC->DR;
	
	}	
	Rtctmp=RTC_TimeStructure.RTC_Seconds;
}

/**
  * @brief  RTC配置：选择RTC时钟源，设置RTC_CLK的分频系数
  * @param  无
  * @retval 无
  */
void BSP_RTC_CLK_Config(void)
{
	
	RTC_InitTypeDef RTC_InitStruct;
	/*使能 PWR 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	/* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
	PWR_BackupAccessCmd(ENABLE);
	
	#if defined (RTC_CLOCK_SOURCE_LSI)
	/* 使用LSI作为RTC时钟源会有误差 
	 * 默认选择LSE作为RTC的时钟源
	 */
	
	RCC_LSICmd(ENABLE);
	/* 等待LSI稳定 */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	 /* 选择LSI做为RTC的时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
#elif defined (RTC_CLOCK_SOURCE_LSE)
	 /* 使能LSE */ 
	RCC_LSEConfig(RCC_LSE_ON);
	 /* 等待LSE稳定 */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	/* 选择LSE做为RTC的时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
#endif
	/* 使能RTC时钟 */
  RCC_RTCCLKCmd(ENABLE);
	/* 等待 RTC APB 寄存器同步 */
  RTC_WaitForSynchro();
	
	
	/*=====================初始化同步/异步预分频器的值======================*/
	/* 驱动日历的时钟ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	/* 设置异步预分频器的值 */
	RTC_InitStruct.RTC_AsynchPrediv = ASYNCHPREDIV;
	RTC_InitStruct.RTC_SynchPrediv=SYNCHPREDIV;
	RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;
	
	
	/* 用RTC_InitStructure的内容初始化RTC寄存器 */
	if (RTC_Init(&RTC_InitStruct) == ERROR)
	{
		printf("\n\r RTC 时钟初始化失败 \r\n");
	}	
	
}

/**
  * @brief  RTC配置：选择RTC时钟源，设置RTC_CLK的分频系数 重新唤醒时进行配置
  * @param  无
  * @retval 无
  */
#define LSE_STARTUP_TIMEOUT     ((uint16_t)0x05000)
void BSP_RTC_CLK_BACKUP_Config(void)
{
	
	__IO uint16_t StartUpCounter = 0; //初始计数器值
	FlagStatus LSEStatus = RESET;	    //LSE状态标志
	RTC_InitTypeDef RTC_InitStructure;
	
	/* 使能 PWR 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
  PWR_BackupAccessCmd(ENABLE);
	
	/*=========================选择RTC时钟源==============================*/
	/* 使能LSE */
  RCC_LSEConfig(RCC_LSE_ON);	
	
	/* 等待LSE启动稳定，如果超时则退出 */
  do
  {
    LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
    StartUpCounter++;
  }while((LSEStatus == RESET) && (StartUpCounter != LSE_STARTUP_TIMEOUT));
	
	if(LSEStatus==SET)
	{
		printf("\n\r LSE 启动成功 \r\n");
		/* 选择LSE作为RTC的时钟源 */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	}
	else
	{
		printf("\n\r LSE 故障，转为使用LSI \r\n");
		
		/* 使能LSI */	
		RCC_LSICmd(ENABLE);
		/* 等待LSI稳定 */ 
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		
		printf("\n\r LSI 启动成功 \r\n");
		/* 选择LSI作为RTC的时钟源 */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	}
	
	/* 使能 RTC 时钟 */
  RCC_RTCCLKCmd(ENABLE);
  /* 等待 RTC APB 寄存器同步 */
  RTC_WaitForSynchro();
	
	
/*=====================初始化同步/异步预分频器的值======================*/
	/* 驱动日历的时钟ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* 设置异步预分频器的值为127 */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	/* 设置同步预分频器的值为255 */
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* 用RTC_InitStructure的内容初始化RTC寄存器 */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC 时钟初始化失败 \r\n");
	}	
	
}

