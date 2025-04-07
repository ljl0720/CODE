/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "GUITP.h"
/* 开发板硬件bsp头文件 */
#include "bsp_led.h"
#include "./Bsp/usart1/bsp_usart.h"
#include "./Bsp/IIC_EEPROM/bsp_i2c_ee_hard.h"
#include "./Bsp/SPI_FLASH/bsp_spi_flash.h"
#include "./Bsp/ADC_TEMP/bsp_adc_temp.h"
#include "./Bsp/RTC/bsp_rtc.h"
#include "./Bsp/PWR_STOP/bsp_pwr_stop.h"
#include "./Bsp/TOUCH/bsp_touch.h"
#include "./Bsp/TOUCH/bsp_touch_lcd.h"
#include "./Bsp/FATFS_TASK/FatFs_test.h"
#include "./Bsp/STemWIN_TASK/Win_DispositionDLG.h"
#include "./Bsp/STemWIN_TOOL/bsp_tool.h"
#include "./Bsp/STemWIN_TASK/Win_LoginDLG.h"
#include "./Bsp/STemWIN_TASK/Win_Manage_SelectDLG.h"
#include "./Bsp/STemWIN_TASK/Win_System_SetDLG.h"
#include "./Bsp/STemWIN_TASK/Menu_Child_WindowDLG.h"
#include "./Bsp/STemWIN_TASK/Win_MonitorDLG.h"
#include "./Bsp/STemWIN_TASK/Connect_WinDLG.h"
#include "./Bsp/STemWIN_TASK/Canlender_WindowDLG.h"
#include "./Bsp/STemWIN_TASK/Log_WinDLG.h"
#include "./Bsp/STemWIN_TASK/Log_TimeSel_WinDLG.h"
#include "./Bsp/STemWIN_TASK/Calibrate_WindowDLG.h"
#include "./Bsp/STemWIN_TASK/Dispositon_ChildDLG.h"

#include "Win_MenuDLG.h"
#include "bsp_keyboard.h"
#include "string.h"
//#include "malloc.h"

/*任务标志变量*/
int FLASH_WriteFlag=0;

/*任务参数*/
extern uint8_t USART1_SendBuffer[BSP_USART1_BufferSize];//串口发送缓冲数组
extern uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];//串口接收缓冲数组
extern u16 USART_RX_STA;       //接收状态标记
extern uint8_t Ctl_Flag;

/* IIC EEPROM 发送接收缓冲区初始化 */
uint8_t I2c_Buf_Write[256];//EEPROM的写入缓冲数组
uint8_t I2c_Buf_Read[256];//EEPROM的读取缓冲数组

/* ADC1 芯片温度检测数值初始化 */
extern __IO u16 ADC_ConvertedValue_temp;//ADC1的通道16内部检测芯片温度

/* SPI FLASH 发送接收缓冲区初始化 */
extern uint8_t Flash_Tx_Buffer[];//FLASH的发送缓冲数组
extern uint8_t Flash_Rx_Buffer[];//FLASH的接收缓冲数组

//读取的ID存储位置
extern __IO uint32_t Flash_DeviceID ;
extern __IO uint32_t FlashID;
extern __IO uint8_t TransferStatus1;

/* 集成初始化配置函数 */
static void BSP_Init(void);

/* 任务句柄创建 */
static TaskHandle_t AppTaskCreate_Handle=NULL;/* 创建任务句柄 */
static TaskHandle_t BSP_USART1_DMA_Handle=NULL;/* USART1串口+DMA任务句柄 */
static TaskHandle_t BSP_I2C_EE_Handle=NULL;/* I2C2总线+EEPROM任务句柄 */
static TaskHandle_t BSP_SPI_FLASH_Handle=NULL;/*SPI通信+FLASH任务句柄 */
static TaskHandle_t BSP_ADC_DMA_TEMP_Handle=NULL;/* ADC1 芯片温度检测任务句柄 */
static TaskHandle_t BSP_RTC_DATETIME_Handle=NULL;/* RTC  日期时间输出任务句柄 */
static TaskHandle_t BSP_LCD_Init_Handle=NULL;/* LCD  触摸屏初始化任务句柄 */
static TaskHandle_t BSP_LCD_Return_Handle=NULL;/* LCD  触摸屏初始化任务句柄 */
static TaskHandle_t BSP_LCD_CHILD_Handle=NULL;/* LCD  子界面切换初始化任务句柄 */
/* 任务栈创建 */
StackType_t BSP_USART1_DMA_Stack[100];

/* 任务函数创建 */
static void AppTaskCreate(void);/* 用于创建任务 */
static void BSP_USART1_DMA_Task(void);/* USART1串口+DMA任务 */
static void BSP_I2C_EE_Task(void);/* I2C2总线+EEPROM任务 */
static void BSP_SPI_FLASH_Task(void);/*SPI通信+FLASH任务 */
static void BSP_ADC_DMA_TEMP_Task(void);/* ADC1 芯片温度检测任务 */
static void BSP_RTC_DATETIME_Task(void);/* RTC  日期时间输出任务 */
static void BSP_LCD_Init_Task(void);/* LCD  触摸屏初始化任务 */
static void BSP_LCD_RETURN_Task(void);/* LCD  触摸屏初始化任务 */
static void BSP_LCD_CHILD_Task(void);/* LCD  子界面切换初始化任务 */
/* 信号量句柄创建 */
SemaphoreHandle_t BinarySem_Handle =NULL;/* 用于控制USART1串口+DMA任务中断 修改为串口数据导入EEPROM */
SemaphoreHandle_t BinarySem_FLASH_Handle =NULL;/* 用于控制USART1串口数据导入SPI FLASH */
SemaphoreHandle_t BinarySem_ADC_TEMP_Handle =NULL;/* 用于控制USART1串口数据导入SPI FLASH */
SemaphoreHandle_t BinarySem_RTC_DATETIME_Handle=NULL;/* 用于控制RTC计时输出 */
SemaphoreHandle_t BinarySem_LCD_Handle=NULL;/* 用于控制LCD显示 */
SemaphoreHandle_t BinarySem_LCD_DEL_Handle=NULL;/* 用于控制LCD删除 */
/* 事件句柄创建 */
EventGroupHandle_t EventGroup_Peripheral_Handle=NULL;//外设启动
EventGroupHandle_t EventGroup_next_Handle=NULL;//向上切换
EventGroupHandle_t EventGroup_prev_Handle=NULL;//向下切换
EventGroupHandle_t EventGroup_branch_Handle=NULL;//分支子窗口切换
/* 事件掩码创建 */
//#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
//#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

/* 任务执行时间变量存储数组 */
char InfoBuffer[20];

/*数据对象句柄*/
extern GRAPH_DATA_Handle GraphdataV,GraphdataH;
extern GUI_POINT point;
extern WM_HWIN Show_prev,Show_next,login,number;

/*页面控件数据结构体*/
struct Login_Value 				LV; 
struct Keyboard_Value		  KV;
struct Menu_Value 				MV;
struct Manage_Sel_Value	  MSV;
struct System_Set_Value		SSV;
struct Monitor_Value 			MONV;
struct Disposition_Value  DISPV; 
struct Connect_Value 			CONTV;
struct Canlender_Value    CANLV;
struct Calibrate_Value    CALIV;
struct Log_Value 					LOGV;

extern GUI_HMEM hQR;//二维码句柄
extern GUI_BITMAP ac_buff[BITMAP_NUM];
extern int bmp_i;
int flag_login=0;
int flag_keyboard=0;
int FLAG_Window = 3;//标记触发Keyboard的界面
extern DataPoint *head;//释放曲线链表资源

Task_List HList;

int main(void)
{	
	
	BaseType_t Return_Status=pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	/* 开发板硬件初始化 */
  BSP_Init();
//	HList=Task_ListInit();
	
  printf("串口发送数据触发中断,任务处理数据!\n");	
	
	Return_Status=xTaskCreate( (TaskFunction_t) AppTaskCreate,
														 (const char *  ) "AppTaskCreate",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 1,
														 (TaskHandle_t *) &AppTaskCreate_Handle);
														 
	if(Return_Status==pdPASS)
	{			
		vTaskStartScheduler();   /* 启动任务，开启调度 */
	}else
    {
			return -1;
		} 
		
		
	while(1);/* 正常不会执行到这里 */  
}

static void AppTaskCreate(void)
{
	
	BaseType_t Return_Status=pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	taskENTER_CRITICAL();//进入临界区
	
	BinarySem_Handle = xSemaphoreCreateBinary();/* 创建二值信号量 */
	BinarySem_FLASH_Handle= xSemaphoreCreateBinary();/* 创建二值信号量 */
	BinarySem_ADC_TEMP_Handle= xSemaphoreCreateBinary();/* 创建二值信号量 */
	BinarySem_RTC_DATETIME_Handle= xSemaphoreCreateBinary();/* 创建二值信号量 */
	BinarySem_LCD_Handle= xSemaphoreCreateBinary();/* 创建二值信号量 */
 	BinarySem_LCD_DEL_Handle= xSemaphoreCreateBinary();/* 创建二值信号量 */
	
	EventGroup_Peripheral_Handle=xEventGroupCreate();
	EventGroup_next_Handle=xEventGroupCreate();
	EventGroup_prev_Handle=xEventGroupCreate();
	EventGroup_branch_Handle=xEventGroupCreate();
	if(NULL != BinarySem_Handle)
    printf("BinarySem_Handle二值信号量创建成功!\n");
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_USART1_DMA_Task,
														 (const char *  ) "usart1_dma",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 2,
														 (TaskHandle_t *) &BSP_USART1_DMA_Handle);
														 
	if(Return_Status==pdPASS)
	{	
		printf("创建Uart_Task任务成功!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_I2C_EE_Task,
														 (const char *  ) "i2c_ee",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 4,
														 (TaskHandle_t *) &BSP_I2C_EE_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建I2C_EE_Task任务成功!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_SPI_FLASH_Task,														
														 (const char *  ) "spi_flash",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 5,
														 (TaskHandle_t *) &BSP_SPI_FLASH_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建SPI_FLASH_Task任务成功!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_ADC_DMA_TEMP_Task,														
														 (const char *  ) "adc_temp",
														 (uint32_t      ) 128,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 3,
														 (TaskHandle_t *) &BSP_ADC_DMA_TEMP_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建BSP_ADC_DMA_TEMP_Task任务成功!\n");		
	}
	
	
		Return_Status=xTaskCreate( (TaskFunction_t) BSP_RTC_DATETIME_Task,														
														 (const char *  ) "rtc_date",
														 (uint32_t      ) 128,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_RTC_DATETIME_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建BSP_RTC_DATETIME_Task任务成功!\n");		
	}
	
			Return_Status=xTaskCreate( (TaskFunction_t) BSP_LCD_Init_Task,														
														 (const char *  ) "lcd_show",
														 (uint32_t      ) 1024*8,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_LCD_Init_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建BSP_LCD_Init_Task任务成功!\n");		
	}
				Return_Status=xTaskCreate( (TaskFunction_t) BSP_LCD_RETURN_Task,														
														 (const char *  ) "lcd_show",
														 (uint32_t      ) 1024*8,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_LCD_Return_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("创建BSP_LCD_DEL_Task任务成功!\n");		
	}
	
//		Return_Status=xTaskCreate( (TaskFunction_t) BSP_LCD_CHILD_Task,														
//														 (const char *  ) "lcd_show",
//														 (uint32_t      ) 512,
//														 (void *		    ) NULL,
//														 (UBaseType_t   ) 8,
//														 (TaskHandle_t *) &BSP_LCD_CHILD_Handle);
//	
//		if(Return_Status==pdPASS)
//	{	
//		printf("创建BSP_LCD_DEL_Task任务成功!\n");		
//	}

	
	vTaskDelete(AppTaskCreate_Handle);//删除AppTaskCreate任务
	taskEXIT_CRITICAL();//退出临界区
}

/***********************************************************************
  * @ 函数名  ： BSP_USART1_DMA_Task
  * @ 功能说明： USART1串口+DMA接收任务
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_USART1_DMA_Task(void)
{
	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  while (1)
  {
		taskENTER_CRITICAL();
    //获取二值信号量 xSemaphore,没获取到则一直等待``
//		xReturn = xSemaphoreTake(BinarySem_Handle,/* 二值信号量句柄 */
//                              portMAX_DELAY); /* 等待时间 */
//    if(pdPASS == xReturn)
		
//    {
		if(*USART1_RecvBuffer){
      printf("收到数据:%s\n",USART1_RecvBuffer);
      memset(USART1_RecvBuffer,0,BSP_USART1_BufferSize);/* 清零 */
		  Ctl_Flag = 0;
			
//			vTaskGetRunTimeStats(InfoBuffer);
		  printf("%s\r\n",InfoBuffer);
			 BSP_PWR_STOP_TEST();
		}
		taskEXIT_CRITICAL();
		
		xSemaphoreGive(BinarySem_LCD_Handle);
		
		GUI_Exec();
		GUI_TOUCH_Exec();

		portYIELD();
		vTaskDelay(100);
//    }
			
  }
	
}

/***********************************************************************
  * @ 函数名  ： BSP_USART1_DMA_Task
  * @ 功能说明： USART1串口+DMA接收任务
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_I2C_EE_Task(void)
{
	while(1){
//		taskENTER_CRITICAL();
	BaseType_t	pxHigherPriorityTaskWoken;
//	vTaskDelay(100);
//	BSP_I2C_EE_API(USART1_RecvBuffer);
	//获取二值信号量 xSemaphore,没获取到则一直等待``
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
		xReturn = xSemaphoreTake(BinarySem_Handle,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
		
	  if(pdPASS == xReturn)
    {
	for (int i=0; i<=10; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = USART1_RecvBuffer[i];
    printf("0x%02X  ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
    printf("\n\r");    
   }

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	BSP_I2C_EE_WriteBuffer(I2c_Buf_Write, CHAR_ADDR, 10);
	FileSystem_Write("0:IIC_EEPROM写入数据.txt",I2c_Buf_Write);
	 
	 //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	BSP_I2C_EE_ReadBuffer(I2c_Buf_Read, CHAR_ADDR, 10); 
	 
	 //将I2c_Buf_Read中的数据通过串口打印
	for (int i=0; i<10; i++)
	{	
    printf("0x%02X  ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
    printf("\n\r");
	}
   }
		xSemaphoreGive(BinarySem_FLASH_Handle);	//释放FLASH任务的二值信号量
//	 taskEXIT_CRITICAL();
 }
}

static void BSP_SPI_FLASH_Task(void)
{
	while(1)
	{
//		taskENTER_CRITICAL();
		/* 获取 Flash Device ID */
  BaseType_t 	xReturn = xSemaphoreTake(BinarySem_FLASH_Handle,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
	
		
	if(xReturn==pdPASS){
	Flash_DeviceID=BSP_SPI_FLASH_ReadDeviceID();
	vTaskDelay(20);
		
	/* 获取 SPI Flash ID */
	FlashID=BSP_SPI_FLASH_ReadID();
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, Flash_DeviceID);
	
	if(FlashID==BSP_FLASH_ID256 && FLASH_WriteFlag==0)
	{
		printf("\r\n检测到SPI FLASH W25Q256 !\r\n");
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		BSP_SPI_FLASH_SectorErase(Flash_SectorToErase);
		/* 将发送缓冲区的数据写到flash中 */
//		BSP_SPI_FLASH_WriteBuffer(Flash_Tx_Buffer,Flash_WriteAddress,10);
//		printf("\r\n写入的数据为：\r\n%s", Flash_Tx_Buffer);
		
		BSP_SPI_FLASH_WriteBuffer(USART1_RecvBuffer,Flash_WriteAddress,10);
		printf("\r\n写入的USART1数据为：\r\n%s", USART1_RecvBuffer);
		FileSystem_Write("0:SPI_FLASH写入数据.txt",USART1_RecvBuffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		BSP_SPI_FLASH_ReadBuffer(Flash_Rx_Buffer,Flash_WriteAddress,10);
		printf("\r\n读出的数据为：\r\n%s", Flash_Rx_Buffer);		
		FLASH_WriteFlag=1;
	}
	else
	{
		printf("\r\n检测到SPI FLASH W25Q256 !\r\n");
		memset(Flash_Rx_Buffer,0,10);/* 清零 */
		BSP_SPI_FLASH_ReadBuffer(Flash_Rx_Buffer,(Flash_WriteAddress+5),5);
		printf("\r\n读出的数据为：\r\n%s", Flash_Rx_Buffer);		
		FLASH_WriteFlag=0;
		
		xSemaphoreGive(BinarySem_RTC_DATETIME_Handle);//释放RTC计时输出的二值信号量

		
	}
	xSemaphoreGive(BinarySem_ADC_TEMP_Handle);	//释放ADC1芯片温度检测的二值信号量
//	taskEXIT_CRITICAL();
//	portYIELD();
//	vTaskDelay(10);
//	/*FLASH断电*/
//	BSP_SPI_FLASH_PowerDown();
}
	}
	
}
static void BSP_ADC_DMA_TEMP_Task(void)
{
	while(1)
		{
			

	    BaseType_t 	xReturn = xSemaphoreTake(BinarySem_ADC_TEMP_Handle,/* 二值信号量句柄 */
                              portMAX_DELAY); /* 等待时间 */
				uint8_t tem[10];
	if(xReturn==pdPASS)
	{
	  uint16_t	Current_Temp=(ADC_ConvertedValue_temp*3.3/4096-0.76)*1000/2.5+25;
		printf("\r\n The IC current tem= %3d ℃\r\n", Current_Temp/10);
	}

 }
}

static void BSP_RTC_DATETIME_Task(void)
{
	BaseType_t 	xReturn=pdPASS;
	while(1)
	{
		
		
	/*
	 * 当我们配置过RTC时间之后就往备份寄存器0写入一个数据做标记
	 * 所以每次程序重新运行的时候就通过检测备份寄存器0的值来判断
	 * RTC 是否已经配置过，如果配置过那就继续运行，如果没有配置过
	 * 就初始化RTC，配置RTC的时间。
	 */
//				 	xReturn = xSemaphoreTake(BinarySem_RTC_DATETIME_Handle,/* 二值信号量句柄 */
//                              portMAX_DELAY); /* 等待时间 */
		
		EventBits_t bit=xEventGroupWaitBits(EventGroup_Peripheral_Handle, /* 事件对象句柄 */
		                                    DATETIME_EVENT,/* 接收线程感兴趣的事件 */
	                                     	pdTRUE,/* 退出时清除事件位 */
		                                    pdTRUE,   /* 满足感兴趣的所有事件 */   
		                                    portMAX_DELAY);/* 指定超时事件,一直等 */
		
	if((xReturn==pdPASS)&&(bit & DATETIME_EVENT)){
	if(RTC_ReadBackupRegister(RTC_BKP_DRX)!=RTC_BKP_DATA)
	{
		BSP_RTC_Config();
	}
	else 
	{
		/* 检查是否电源复位 */
		if(RCC_GetFlagStatus(RCC_FLAG_PORRST)!=RESET)
		{
			 printf("\r\n 发生电源复位....\r\n");
		}
		/* 检查是否外部复位 */
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST)!=RESET)
		{
			BSP_RTC_Config();
			printf("\r\n 发生外部复位....\r\n");
		}
		printf("\r\n 不需要重新配置RTC....\r\n");
		/* 使能 PWR 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
    PWR_BackupAccessCmd(ENABLE);
    /* 等待 RTC APB 寄存器同步 */
    RTC_WaitForSynchro(); 
	}
	/* 显示时间和日期 */
	BSP_RTC_SHOW_DateTime();
	
//	portYIELD();
 }
}
}
int i=0;

void Init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	LCDD_Init();	   //液晶屏初始化
	TP_Init();
	
//		/*FATFS+SDIO系统初始化	*/
//	FileSystem_Init();
	
		GUI_Init();/* 初始化GUI */
	GUI_SetBkColor(GUI_WHITE);
  GUI_CURSOR_Show();/* 开启光标 */
	GUI_UC_SetEncodeUTF8();/* 显示自定义汉字 */
  GUI_Clear();
	WM_EnableMemdev(1);
  WM_MULTIBUF_Enable(1);/* 开启三缓冲 */
	GUI_Exec();
}

static void BSP_LCD_Init_Task(void)
{
//	BaseType_t xReturn=pdPASS;
//BaseType_t 	xReturn=xSemaphoreTake(BinarySem_LCD_Handle,/* 二值信号量句柄 */
//                              portMAX_DELAY); /* 等待时间 */
//	if(xReturn==pdPASS){
		
		
//	vTaskDelay(100);
		
		if(flag_login==0)
	  {
//			LV.Login=CreateLogin();
			/*调试界面用*/
			/*3.6修改调整所有字体和图标显示，必须与PPT一致   */
//			KV.Keyboard=Createnumber();//Y  
			
//		MV.Menu=CreateMenu_Win();//Y
			
//			MSV.Manage_Sel=CreateManage_Select_Win();//Y
			
//			SSV.System_Set=CreateSystem_Set_Win();//Y
			
			MONV.Monitor=CreateMonitor_Win();
			
//			DISPV.Disposition=CreateWin_Disposition();
			
//			CONTV.Connect=CreateConnect_Win();
			
//			CANLV.Canlender=CreateCanlender_Window();
					
//			LOGV.Log=CreateLog_Win();
			
//			CALIV.Calibrate=CreateCalibrate_Window();

//			DISPV.Disposition_Child=CreateDispositon_Child();
//				WM_HideWindow(KV.Keyboard);
			
//			WM_HideWindow(LV.Login);
//			WM_HideWindow(MV.Menu);
			
		}
	
//		WM_ShowWindow(LV.Login);
		long sssss=GUI_ALLOC_GetNumFreeBytes ();
			  printf("Init Task FreeBytes %ld\n",sssss);
	while(1)
	{
		EventBits_t bit=xEventGroupWaitBits(EventGroup_next_Handle, /* 事件对象句柄 */
		                                    LOGIN_EVENT|KEYBOARD_EVENT|MENU_EVENT|MANAGE_SELECT_EVENT|
																				SYSTEM_SET_EVENT|MENU_CHILD_EVENT|MONITOR_EVENT|DISPOSITION_EVENT|
																				CONNECT_EVENT|CANLENDER_EVENT|LOG_EVENT|LOG_CHILD_EVENT|
																				CALIBRATE_EVENT|DISPOSITION_CHILD_EVENT,/* 接收线程感兴趣的事件 */
	                                     	pdTRUE,/* 退出时清除事件位 */
		                                    pdFALSE,   /* 满足感兴趣的所有事件 */   
		                                    portMAX_DELAY);/* 指定超时事件,一直等 */
		
		
//		if(bit&KEYBOARD_EVENT)
//		{
//			printf("切换到 Keyboard 页面...\n");
//			WM_DeleteWindow(LV.Login);//替换为删除
////			WM_HideWindow(LV.Login);
//			KV.Keyboard=Createnumber();//替换为创建
////			WM_ShowWindow(KV.Keyboard);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			
//			 
//		}
//		if(bit&LOGIN_EVENT)
//		{
//			printf("切换到 Login 页面...\n");
//			WM_DeleteWindow(KV.Keyboard);//替换为删除
////			WM_HideWindow(KV.Keyboard);
//			 LV.Login=CreateLogin();//替换为创建
////			WM_ShowWindow(LV.Login);
//			EDIT_SetText(LV.User_Wm,LV.User_Text);
//			EDIT_SetText(LV.Pwd_Wm,LV.Pwd_Text);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Login Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		
//		if(bit&MENU_EVENT)
//		{
//			printf("切换到 Menu 页面...\n");
//			WM_DeleteWindow(LV.Login);
////			WM_HideWindow(LV.Login);//替换为删除
//			MV.Menu=CreateMenu_Win();
////			WM_ShowWindow(MV.Menu);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Menu Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		if(bit&MANAGE_SELECT_EVENT)
//		{
//			printf("切换到 Manage Select 页面...\n");
//			WM_DeleteWindow(MV.Menu);
//			MSV.Manage_Sel=CreateManage_Select_Win();
////			WM_ShowWindow(MV.Menu);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Manage Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		
//		if(bit&SYSTEM_SET_EVENT)
//		{
//			printf("切换到 System Set 页面...\n");
//			WM_DeleteWindow(MSV.Manage_Sel);
//			SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(MV.Menu);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Manage Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		
//		if(bit & MENU_CHILD_EVENT) {
//				printf("MENU 切换到 MENU_CHILD 页面...\n");
//			WM_DeleteWindow(MV.Menu);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MV.Menu_Child=CreateMenu_Chlid_Window();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Manage Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		
//				if(bit&MONITOR_EVENT)
//		{
//			printf("切换到 Monitor 页面...\n");
//			WM_DeleteWindow(MV.Menu);//替换为删除
////			WM_HideWindow(LV.Login);
//			MONV.Monitor=CreateMonitor_Win();//替换为创建
////			WM_ShowWindow(KV.Keyboard);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Monitor Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			
//			 
//		}
	switch(bit)
			{
		case KEYBOARD_EVENT:
		{
			if(LV.Login)
			{
			printf("Login 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(LV.Login);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(DISPV.Disposition)
			{
				printf("Disposition 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(DISPV.Disposition);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(CONTV.Connect)
			{
				printf("Connect 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(CONTV.Connect);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(CANLV.Canlender)
			{
				printf("Canlender 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(CANLV.Canlender);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(LOGV.Log)
			{
				printf("Log 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(LOGV.Log);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(LOGV.Log_Child)
			{
				printf("Log Child 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(LOGV.Log_Child);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(CALIV.Calibrate)
			{
				printf("Calibrate 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(CALIV.Calibrate);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(DISPV.Disposition_Child)
			{
					printf("Disposition Child 切换到 Keyboard 页面...\n");
			WM_DeleteWindow(DISPV.Disposition_Child);//替换为删除
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
		}
		case LOGIN_EVENT:
		{
			printf("切换到 Login 页面...\n");
			WM_DeleteWindow(KV.Keyboard);//替换为删除
//			WM_HideWindow(KV.Keyboard);
			 LV.Login=CreateLogin();//替换为创建
//			WM_ShowWindow(LV.Login);
			EDIT_SetText(LV.User_Wm,LV.User_Text);
			EDIT_SetText(LV.Pwd_Wm,LV.Pwd_Text);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Login Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		
		case MENU_EVENT:
		{
			printf("切换到 Menu 页面...\n");
			WM_DeleteWindow(LV.Login);
//			WM_HideWindow(LV.Login);//替换为删除
			MV.Menu=CreateMenu_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Menu Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		case MANAGE_SELECT_EVENT:
		{
			printf("切换到 Manage Select 页面...\n");
			WM_DeleteWindow(MV.Menu);
			MSV.Manage_Sel=CreateManage_Select_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Manage Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		
		case SYSTEM_SET_EVENT:
		{
			printf("切换到 System Set 页面...\n");
			WM_DeleteWindow(MSV.Manage_Sel);
			SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Manage Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		
		case MENU_CHILD_EVENT: 
			{
				printf("MENU 切换到 MENU_CHILD 页面...\n");
			WM_DeleteWindow(MV.Menu);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu_Child=CreateMenu_Chlid_Window();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Manage Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		
		case MONITOR_EVENT:
		{
			printf("切换到 Monitor 页面...\n");
			WM_DeleteWindow(MV.Menu);//替换为删除
//			WM_HideWindow(LV.Login);
			MONV.Monitor=CreateMonitor_Win();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Monitor Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		case DISPOSITION_EVENT:
		{
			if(MV.Menu)
			{
			printf("切换到 Disposition 页面...\n");
			WM_DeleteWindow(MV.Menu);//替换为删除
//			WM_HideWindow(LV.Login);
			DISPV.Disposition=CreateWin_Disposition();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Disposition Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
			}
			
//			if(KV.Keyboard)
//			{
//							printf("切换到 Disposition 页面...\n");
//			WM_DeleteWindow(MV.Menu);//替换为删除
////			WM_HideWindow(LV.Login);
//			DISPV.Disposition=CreateWin_Disposition();//替换为创建
////			WM_ShowWindow(KV.Keyboard);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Disposition Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			break;
//			}
		}
		case CONNECT_EVENT:
		{
			printf("切换到 Connect 页面...\n");
			WM_DeleteWindow(SSV.System_Set);//替换为删除
//			WM_HideWindow(LV.Login);
			CONTV.Connect=CreateConnect_Win();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case CANLENDER_EVENT:
		{
				printf("切换到 Canlender 页面...\n");
			WM_DeleteWindow(SSV.System_Set);//替换为删除
//			WM_HideWindow(LV.Login);
			CANLV.Canlender=CreateCanlender_Window();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case CALIBRATE_EVENT:
		{
				printf("切换到 Canlender 页面...\n");
			WM_DeleteWindow(SSV.System_Set);//替换为删除
//			WM_HideWindow(LV.Login);
//			CALIV.Calibrate=crea();//替换为创建
			CALIV.Calibrate=CreateCalibrate_Window();
			
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case LOG_EVENT:
		{
			printf("切换到 Log 页面...\n");
			WM_DeleteWindow(MSV.Manage_Sel);
			LOGV.Log=CreateLog_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		case LOG_CHILD_EVENT:
		{
			printf("切换到 Log Child 页面...\n");
			WM_DeleteWindow(LOGV.Log);
			LOGV.Log_Child=CreateLog_TimeSel_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		case DISPOSITION_CHILD_EVENT:
		{
			if(DISPV.Disposition)
			{
				printf("切换到 Disposition Child 页面...\n");
			WM_DeleteWindow(DISPV.Disposition);
			DISPV.Disposition_Child=CreateDispositon_Child();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Disposition Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
			}
		}
			}

//		if((flag_login==1) && (flag_keyboard ==2) ){xEventGroupSetBits(EventGroup_Handle,KEY4_EVENT);}
		
//	TP_Init();//开启校准检测
//		ICONVIEW_DeleteItem(LV.Logo,0);
//		WM_InvalidateWindow(LV.Logo);
		GUI_Exec();
		GUI_TOUCH_Exec();
		vTaskDelay(5);
	}

}

static void BSP_LCD_RETURN_Task(void)
{
	long sssss=GUI_ALLOC_GetNumFreeBytes ();
			  printf("Return Task FreeBytes %ld\n",sssss);
	
            // 强制创建登录界面

while(1){
		EventBits_t bit2=xEventGroupWaitBits(EventGroup_prev_Handle, /* 事件对象句柄 */
		                                    LOGIN_EVENT|MENU_EVENT|MANAGE_SELECT_EVENT|SYSTEM_SET_EVENT
																				|MENU_CHILD_EVENT|MONITOR_EVENT|DISPOSITION_EVENT|
																				CONNECT_EVENT|CANLENDER_EVENT|LOG_EVENT|LOG_CHILD_EVENT|
																				CALIBRATE_EVENT|DISPOSITION_CHILD_EVENT	,/* 接收线程感兴趣的事件 */
	                                     	pdTRUE,/* 退出时清除事件位 */
		                                    pdFALSE,   /* 满足感兴趣的所有事件 */   
		                                    portMAX_DELAY);/* 指定超时事件,一直等 */

//	   if(bit2 & LOGIN_EVENT) {
//				printf("切换回 Login 页面...\n");
//			WM_DeleteWindow(MV.Menu);
////		SSV.System_Set=CreateSystem_Set_Win();
//			 LV.Login=CreateLogin();//替换为创建
//			 EDIT_SetText(LV.User_Wm,LV.User_Text);
//			 EDIT_SetText(LV.Pwd_Wm,LV.Pwd_Text);
////			WM_ShowWindow(LV.Login);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Login Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		 
//		if(bit2 & MENU_EVENT) {
//			if(MSV.Manage_Sel)
//			{
//				printf("Manage Select切换回 Menu 页面...\n");
//			WM_DeleteWindow(MSV.Manage_Sel);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MV.Menu=CreateMenu_Win();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Menu Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			}
//			if(SSV.System_Set)
//			{
//				printf("System Set切换回 Menu 页面...\n");
//			WM_DeleteWindow(SSV.System_Set);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MV.Menu=CreateMenu_Win();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Menu Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			}	
//			if(MONV.Monitor) {
//				printf("Monitor界面切换回 MENU 页面...\n");
//			WM_DeleteWindow(MONV.Monitor);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MV.Menu=CreateMenu_Win();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Menu Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		}
//		
//		if(bit2 & MANAGE_SELECT_EVENT) {
//				printf("切换回 Manage Select 页面...\n");
//			WM_DeleteWindow(SSV.System_Set);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MSV.Manage_Sel=CreateManage_Select_Win();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Manage Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
		switch(bit2)
		{
			case  LOGIN_EVENT: 
				{
				printf("切换回 Login 页面...\n");
			WM_DeleteWindow(MV.Menu);
//		SSV.System_Set=CreateSystem_Set_Win();
			 LV.Login=CreateLogin();//替换为创建
			 EDIT_SetText(LV.User_Wm,LV.User_Text);
			 EDIT_SetText(LV.Pwd_Wm,LV.Pwd_Text);
//			WM_ShowWindow(LV.Login);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Login Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		 
			case MENU_EVENT: 
				{
			if(MSV.Manage_Sel)
			{
				printf("Manage Select切换回 Menu 页面...\n");
			WM_DeleteWindow(MSV.Manage_Sel);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Menu Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
			}
			if(SSV.System_Set)
			{
				printf("System Set切换回 Menu 页面...\n");
			WM_DeleteWindow(SSV.System_Set);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Menu Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
			}	
			if(MONV.Monitor) 
			{
				printf("Monitor界面切换回 MENU 页面...\n");
			WM_DeleteWindow(MONV.Monitor);
//			Destory_Point(&head);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Menu Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(MV.Menu_Child) 
			{
				printf("Menu_Child 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(MV.Menu_Child);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Menu Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(CONTV.Connect)
			{
				printf("Connect 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(CONTV.Connect);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Connect Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(CANLV.Canlender)
			{
				printf("Canlender 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(CANLV.Canlender);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(LOGV.Log)
			{
				printf("Log 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(LOGV.Log);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(CALIV.Calibrate)
			{
				printf("Calibrate 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(CALIV.Calibrate);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
			if(DISPV.Disposition)
			{
				printf("Disposition 界面切换回 MENU 页面...\n");
			WM_DeleteWindow(DISPV.Disposition);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;	
			}
		}
		  case  MANAGE_SELECT_EVENT:
		  {
				if(SSV.System_Set)
				{
				printf("System Set 切换回 Manage Select 页面...\n");
			WM_DeleteWindow(SSV.System_Set);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MSV.Manage_Sel=CreateManage_Select_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Manage Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}
				if(LOGV.Log)
				{
					printf("Log 切换回 Manage Select 页面...\n");
			WM_DeleteWindow(LOGV.Log);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MSV.Manage_Sel=CreateManage_Select_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
				}
		  }
				
			case SYSTEM_SET_EVENT:
			{
				if(CONTV.Connect)
				{
					printf("Canlender 切换回 System Set 页面...\n");
			WM_DeleteWindow(CONTV.Connect);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			SSV.System_Set=CreateSystem_Set_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return System Set Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
				}
				if(CANLV.Canlender)
				{
						printf("Canlender 切换回 System Set 页面...\n");
			WM_DeleteWindow(CANLV.Canlender);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			SSV.System_Set=CreateSystem_Set_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return System Set Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
				}
				if(CALIV.Calibrate)
				{
						printf("Calibrate 切换回 System Set 页面...\n");
			WM_DeleteWindow(CALIV.Calibrate);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			SSV.System_Set=CreateSystem_Set_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return System Set Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
				}
			}
			case DISPOSITION_EVENT:
			{
				if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Dispositon 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			DISPV.Disposition=CreateWin_Disposition();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Dispositon Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
				  }
				if(DISPV.Disposition_Child)
			{
			printf("切换到 Disposition 页面...\n");
			WM_DeleteWindow(DISPV.Disposition_Child);//替换为删除
//			WM_HideWindow(LV.Login);
			DISPV.Disposition=CreateWin_Disposition();//替换为创建
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Disposition Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
			}
			}
			case CONNECT_EVENT:
				{
				if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Connect 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			CONTV.Connect=CreateConnect_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Connect Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}
		}
		case CANLENDER_EVENT:
		{
			if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Canlender 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			CANLV.Canlender=CreateCanlender_Window();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Connect Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
		}				
	}
		case LOG_EVENT:
		{
			if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Log 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			LOGV.Log=CreateLog_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}			
			if(LOGV.Log_Child)
					{
				printf("Log Child 切换回 Log 页面...\n");
			WM_DeleteWindow(LOGV.Log_Child);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			LOGV.Log=CreateLog_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}			
				}		
		case LOG_CHILD_EVENT:
		{
			if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Log Child 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			LOGV.Log_Child=CreateLog_TimeSel_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Log Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}
		}
		case CALIBRATE_EVENT:
		{
				if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Calibrate 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			CALIV.Calibrate=CreateCalibrate_Window();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Calibrate Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}
		}
		case DISPOSITION_CHILD_EVENT:
		{
			if(KV.Keyboard)
					{
				printf("Keyboard 切换回 Dispositon Child 页面...\n");
			WM_DeleteWindow(KV.Keyboard);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			DISPV.Disposition_Child=CreateDispositon_Child();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Dispositon Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		  break;
					}
		}
		
	}
		
//		if(bit2 & SYSTEM_SET_EVENT) {
//				printf("切换回 System Set 页面...\n");
//			WM_DeleteWindow(MV.Menu);
////		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return System Set Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//	
	
//	ICONVIEW_DeleteItem(LV.Logo,0); 
				GUI_Exec();
		GUI_TOUCH_Exec();
//		GUI_Delay(100);
		vTaskDelay(5);			
		
	}
		}
		
static void BSP_LCD_CHILD_Task(void)
	{
		
		long sssss=GUI_ALLOC_GetNumFreeBytes ();
			  printf("Return Task FreeBytes %ld\n",sssss);
	  
            // 强制创建登录界面
while(1){	
		EventBits_t bit3=xEventGroupWaitBits(EventGroup_branch_Handle, /* 事件对象句柄 */
		                                    MENU_CHILD_EVENT,/* 接收线程感兴趣的事件 */
	                                     	pdTRUE,/* 退出时清除事件位 */
		                                    pdFALSE,   /* 满足感兴趣的所有事件 */   
		                                    portMAX_DELAY);/* 指定超时事件,一直等 */
	
	if(bit3 & MENU_CHILD_EVENT) {
				printf("子界面切换回 MENU 页面...\n");
			WM_DeleteWindow(MV.Menu_Child);
//		SSV.System_Set=CreateSystem_Set_Win();
//			WM_ShowWindow(LV.Login);
			MV.Menu=CreateMenu_Win();
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Return Manage Select Task FreeBytes %ld\n",sssss);
			GUI_Exec();
		}
	
	
			GUI_Exec();
		GUI_TOUCH_Exec();
		vTaskDelay(5);
	
}
	}		
	

/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	/* 串口初始化	*/
	BSP_USART_GPIO_Config();
	
	/* UASRT1的DMA初始化	*/
	BSP_USART1_DMA_Config();
	
//	/* I2C2的EEPROM初始化	*/
  BSP_I2C_EE_Init();
	
	/* SPI的FLASH初始化	*/
	BSP_SPI_FLASH_Config();
	
	/* ADC1+DMA的芯片温度检测初始化	*/
	ADC_Cmd(ADC1, DISABLE);
	BSP_ADC1_MODE_Config();
	
	/*RTC日期计时时钟初始化	*/
	BSP_RTC_CLK_Config();
	
	/*PWR停止模式按键唤醒初始化	*/
	BSP_PWR_EXTI_Key_Config();
	
	/*SPI+LCD显示屏校准初始化	*/
	LCDD_Init();	   //液晶屏初始化
	TP_Init();
	
	/*FATFS+SDIO系统初始化	*/
	FileSystem_Init();
	
	
//	NVIC_SetPriority(PendSV_IRQn, 15);  // 最低优先级
//  NVIC_SetPriority(SysTick_IRQn, 15);
//	my_mem_init(SRAMIN);		    //初始化内部内存池
//	my_mem_init(SRAMEX);		    //初始化外部内存池
//	my_mem_init(SRAMCCM);		    //初始化CCM内存池
    
	
  /*STemWIN显示界面头文件初始化	*/
	GUI_Init();/* 初始化GUI */
	GUI_SetBkColor(GUI_WHITE);
  GUI_CURSOR_Show();/* 开启光标 */
	GUI_UC_SetEncodeUTF8();/* 显示自定义汉字 */
  GUI_Clear();
	WM_EnableMemdev(1);
//	GUI_EnableAlpha(1);
  WM_MULTIBUF_Enable(1);/* 开启三缓冲 */
	GUI_Exec();
	
	XYBlock_Init();
	
}

/********************************END OF FILE****************************/
