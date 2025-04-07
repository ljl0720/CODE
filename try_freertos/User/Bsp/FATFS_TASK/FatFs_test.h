#ifndef __FATFS_TEST_H__
#define __FATFS_TEST_H__

/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/


/* STM32 固件库头文件 */
#include "stm32f4xx.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"

/* 开发板硬件bsp头文件 */
#include "./Bsp/usart1/bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "ff.h"
#include "./Bsp/FATFS_TASK/FatFs_test.h"


/*
*************************************************************************
*                             变量声明
*************************************************************************
*/


/*
*************************************************************************
*                               函数声明
*************************************************************************
*/

void FileSystem_Init(void);

void FileSystem_Write(TCHAR * path,BYTE*  Write);

void FileSystem_Read(TCHAR * path);



#endif /* __FATFS_TEST_H__ */





