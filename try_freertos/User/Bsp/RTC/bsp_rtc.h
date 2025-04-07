#ifndef __BSP_RTC_H	
#define __BSP_RTC_H

#include "stm32f4xx.h"

// 时钟源宏定义
//#define RTC_CLOCK_SOURCE_LSE      
#define RTC_CLOCK_SOURCE_LSI

// 异步分频因子
#define ASYNCHPREDIV         0X7F
// 同步分频因子
#define SYNCHPREDIV          0XFF



// 时间宏定义
#define RTC_H12_AMorPM			 RTC_H12_AM  
#define HOURS                1          // 0~23
#define MINUTES              1          // 0~59
#define SECONDS              1          // 0~59

// 日期宏定义
#define WEEKDAY              5          // 1~7
#define DATE                 12         // 1~31
#define MONTH                03         // 1~12
#define YEAR                 25         // 0~99

// 时间格式宏定义
#define RTC_Format_BINorBCD  RTC_Format_BIN

// 备份域寄存器宏定义
#define RTC_BKP_DRX          RTC_BKP_DR0
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0X32F2
 
void BSP_RTC_Config(void);
void BSP_RTC_SHOW_DateTime(void);
void BSP_RTC_CLK_Config(void);
void BSP_RTC_CLK_BACKUP_Config(void);

#endif