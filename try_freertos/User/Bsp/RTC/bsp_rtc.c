#include "./Bsp/RTC/bsp_rtc.h"
#include "./Bsp/STemWIN_TASK/Win_LoginDLG.h"
#include "GUI.h"
#include "DIALOG.h"
#include "GUITP.h"
#include "string.h"
extern struct Canlender_Value    CANLV;
/**
  * @brief  ����ʱ�������
  * @param  ��
  * @retval ��
  */
	
void BSP_RTC_Config(void)
{
	printf("config\n");
//	RTC_InitTypeDef RTC_InitStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	//��ʼ����ʼʱ��
	RTC_TimeStruct.RTC_H12=RTC_H12_AMorPM;
	RTC_TimeStruct.RTC_Hours=HOURS;
	RTC_TimeStruct.RTC_Minutes=MINUTES;
	RTC_TimeStruct.RTC_Seconds=SECONDS;
	RTC_SetTime(RTC_Format_BINorBCD,&RTC_TimeStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX,RTC_BKP_DATA);
	
	//��ʼ����ʼ����
	RTC_DateStruct.RTC_Date=DATE;
	RTC_DateStruct.RTC_Month=MONTH;
	RTC_DateStruct.RTC_WeekDay=WEEKDAY;
	RTC_DateStruct.RTC_Year=YEAR;
	RTC_SetDate(RTC_Format_BINorBCD,&RTC_DateStruct);
	RTC_WriteBackupRegister(RTC_BKP_DRX,RTC_BKP_DATA);
	
}
	
/**
  * @brief  ��ʾʱ�������
  * @param  ��
  * @retval ��
  */
void BSP_RTC_SHOW_DateTime(void)
{
	
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	
	
	//��ȡʱ������� ��Ҫȷ���Ⱥ�˳���ֹ����
	RTC_GetTime(RTC_Format_BINorBCD,&RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BINorBCD,&RTC_DateStructure);
	
	
	
	//ÿ�����һ��	
	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
	{
		//�Ȱ�Ҫ��ʾ��������sprintf����ת��Ϊ�ַ�����Ȼ�������Һ����ʾ������ʾ
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
		
		// ��ӡʱ��
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
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
void BSP_RTC_CLK_Config(void)
{
	
	RTC_InitTypeDef RTC_InitStruct;
	/*ʹ�� PWR ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	/* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
	PWR_BackupAccessCmd(ENABLE);
	
	#if defined (RTC_CLOCK_SOURCE_LSI)
	/* ʹ��LSI��ΪRTCʱ��Դ������� 
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
	
	RCC_LSICmd(ENABLE);
	/* �ȴ�LSI�ȶ� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	 /* ѡ��LSI��ΪRTC��ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
#elif defined (RTC_CLOCK_SOURCE_LSE)
	 /* ʹ��LSE */ 
	RCC_LSEConfig(RCC_LSE_ON);
	 /* �ȴ�LSE�ȶ� */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	/* ѡ��LSE��ΪRTC��ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
#endif
	/* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);
	/* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
	
	
	/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStruct.RTC_AsynchPrediv = ASYNCHPREDIV;
	RTC_InitStruct.RTC_SynchPrediv=SYNCHPREDIV;
	RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;
	
	
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStruct) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
	
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ�� ���»���ʱ��������
  * @param  ��
  * @retval ��
  */
#define LSE_STARTUP_TIMEOUT     ((uint16_t)0x05000)
void BSP_RTC_CLK_BACKUP_Config(void)
{
	
	__IO uint16_t StartUpCounter = 0; //��ʼ������ֵ
	FlagStatus LSEStatus = RESET;	    //LSE״̬��־
	RTC_InitTypeDef RTC_InitStructure;
	
	/* ʹ�� PWR ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);
	
	/*=========================ѡ��RTCʱ��Դ==============================*/
	/* ʹ��LSE */
  RCC_LSEConfig(RCC_LSE_ON);	
	
	/* �ȴ�LSE�����ȶ��������ʱ���˳� */
  do
  {
    LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
    StartUpCounter++;
  }while((LSEStatus == RESET) && (StartUpCounter != LSE_STARTUP_TIMEOUT));
	
	if(LSEStatus==SET)
	{
		printf("\n\r LSE �����ɹ� \r\n");
		/* ѡ��LSE��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	}
	else
	{
		printf("\n\r LSE ���ϣ�תΪʹ��LSI \r\n");
		
		/* ʹ��LSI */	
		RCC_LSICmd(ENABLE);
		/* �ȴ�LSI�ȶ� */ 
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		
		printf("\n\r LSI �����ɹ� \r\n");
		/* ѡ��LSI��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	}
	
	/* ʹ�� RTC ʱ�� */
  RCC_RTCCLKCmd(ENABLE);
  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
	
	
/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* �����첽Ԥ��Ƶ����ֵΪ127 */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	/* ����ͬ��Ԥ��Ƶ����ֵΪ255 */
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
	
}

