#ifndef __FATFS_TEST_H__
#define __FATFS_TEST_H__

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/


/* STM32 �̼���ͷ�ļ� */
#include "stm32f4xx.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/* ������Ӳ��bspͷ�ļ� */
#include "./Bsp/usart1/bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "ff.h"
#include "./Bsp/FATFS_TASK/FatFs_test.h"


/*
*************************************************************************
*                             ��������
*************************************************************************
*/


/*
*************************************************************************
*                               ��������
*************************************************************************
*/

void FileSystem_Init(void);

void FileSystem_Write(TCHAR * path,BYTE*  Write);

void FileSystem_Read(TCHAR * path);



#endif /* __FATFS_TEST_H__ */





