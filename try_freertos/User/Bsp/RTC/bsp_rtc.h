#ifndef __BSP_RTC_H	
#define __BSP_RTC_H

#include "stm32f4xx.h"

// ʱ��Դ�궨��
//#define RTC_CLOCK_SOURCE_LSE      
#define RTC_CLOCK_SOURCE_LSI

// �첽��Ƶ����
#define ASYNCHPREDIV         0X7F
// ͬ����Ƶ����
#define SYNCHPREDIV          0XFF



// ʱ��궨��
#define RTC_H12_AMorPM			 RTC_H12_AM  
#define HOURS                1          // 0~23
#define MINUTES              1          // 0~59
#define SECONDS              1          // 0~59

// ���ں궨��
#define WEEKDAY              5          // 1~7
#define DATE                 12         // 1~31
#define MONTH                03         // 1~12
#define YEAR                 25         // 0~99

// ʱ���ʽ�궨��
#define RTC_Format_BINorBCD  RTC_Format_BIN

// ������Ĵ����궨��
#define RTC_BKP_DRX          RTC_BKP_DR0
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0X32F2
 
void BSP_RTC_Config(void);
void BSP_RTC_SHOW_DateTime(void);
void BSP_RTC_CLK_Config(void);
void BSP_RTC_CLK_BACKUP_Config(void);

#endif