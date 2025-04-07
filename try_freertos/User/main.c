/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"
#include "GUITP.h"
/* ������Ӳ��bspͷ�ļ� */
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

/*�����־����*/
int FLASH_WriteFlag=0;

/*�������*/
extern uint8_t USART1_SendBuffer[BSP_USART1_BufferSize];//���ڷ��ͻ�������
extern uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];//���ڽ��ջ�������
extern u16 USART_RX_STA;       //����״̬���
extern uint8_t Ctl_Flag;

/* IIC EEPROM ���ͽ��ջ�������ʼ�� */
uint8_t I2c_Buf_Write[256];//EEPROM��д�뻺������
uint8_t I2c_Buf_Read[256];//EEPROM�Ķ�ȡ��������

/* ADC1 оƬ�¶ȼ����ֵ��ʼ�� */
extern __IO u16 ADC_ConvertedValue_temp;//ADC1��ͨ��16�ڲ����оƬ�¶�

/* SPI FLASH ���ͽ��ջ�������ʼ�� */
extern uint8_t Flash_Tx_Buffer[];//FLASH�ķ��ͻ�������
extern uint8_t Flash_Rx_Buffer[];//FLASH�Ľ��ջ�������

//��ȡ��ID�洢λ��
extern __IO uint32_t Flash_DeviceID ;
extern __IO uint32_t FlashID;
extern __IO uint8_t TransferStatus1;

/* ���ɳ�ʼ�����ú��� */
static void BSP_Init(void);

/* ���������� */
static TaskHandle_t AppTaskCreate_Handle=NULL;/* ���������� */
static TaskHandle_t BSP_USART1_DMA_Handle=NULL;/* USART1����+DMA������ */
static TaskHandle_t BSP_I2C_EE_Handle=NULL;/* I2C2����+EEPROM������ */
static TaskHandle_t BSP_SPI_FLASH_Handle=NULL;/*SPIͨ��+FLASH������ */
static TaskHandle_t BSP_ADC_DMA_TEMP_Handle=NULL;/* ADC1 оƬ�¶ȼ�������� */
static TaskHandle_t BSP_RTC_DATETIME_Handle=NULL;/* RTC  ����ʱ����������� */
static TaskHandle_t BSP_LCD_Init_Handle=NULL;/* LCD  ��������ʼ�������� */
static TaskHandle_t BSP_LCD_Return_Handle=NULL;/* LCD  ��������ʼ�������� */
static TaskHandle_t BSP_LCD_CHILD_Handle=NULL;/* LCD  �ӽ����л���ʼ�������� */
/* ����ջ���� */
StackType_t BSP_USART1_DMA_Stack[100];

/* ���������� */
static void AppTaskCreate(void);/* ���ڴ������� */
static void BSP_USART1_DMA_Task(void);/* USART1����+DMA���� */
static void BSP_I2C_EE_Task(void);/* I2C2����+EEPROM���� */
static void BSP_SPI_FLASH_Task(void);/*SPIͨ��+FLASH���� */
static void BSP_ADC_DMA_TEMP_Task(void);/* ADC1 оƬ�¶ȼ������ */
static void BSP_RTC_DATETIME_Task(void);/* RTC  ����ʱ��������� */
static void BSP_LCD_Init_Task(void);/* LCD  ��������ʼ������ */
static void BSP_LCD_RETURN_Task(void);/* LCD  ��������ʼ������ */
static void BSP_LCD_CHILD_Task(void);/* LCD  �ӽ����л���ʼ������ */
/* �ź���������� */
SemaphoreHandle_t BinarySem_Handle =NULL;/* ���ڿ���USART1����+DMA�����ж� �޸�Ϊ�������ݵ���EEPROM */
SemaphoreHandle_t BinarySem_FLASH_Handle =NULL;/* ���ڿ���USART1�������ݵ���SPI FLASH */
SemaphoreHandle_t BinarySem_ADC_TEMP_Handle =NULL;/* ���ڿ���USART1�������ݵ���SPI FLASH */
SemaphoreHandle_t BinarySem_RTC_DATETIME_Handle=NULL;/* ���ڿ���RTC��ʱ��� */
SemaphoreHandle_t BinarySem_LCD_Handle=NULL;/* ���ڿ���LCD��ʾ */
SemaphoreHandle_t BinarySem_LCD_DEL_Handle=NULL;/* ���ڿ���LCDɾ�� */
/* �¼�������� */
EventGroupHandle_t EventGroup_Peripheral_Handle=NULL;//��������
EventGroupHandle_t EventGroup_next_Handle=NULL;//�����л�
EventGroupHandle_t EventGroup_prev_Handle=NULL;//�����л�
EventGroupHandle_t EventGroup_branch_Handle=NULL;//��֧�Ӵ����л�
/* �¼����봴�� */
//#define KEY1_EVENT  (0x01 << 0)//�����¼������λ0
//#define KEY2_EVENT  (0x01 << 1)//�����¼������λ1

/* ����ִ��ʱ������洢���� */
char InfoBuffer[20];

/*���ݶ�����*/
extern GRAPH_DATA_Handle GraphdataV,GraphdataH;
extern GUI_POINT point;
extern WM_HWIN Show_prev,Show_next,login,number;

/*ҳ��ؼ����ݽṹ��*/
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

extern GUI_HMEM hQR;//��ά����
extern GUI_BITMAP ac_buff[BITMAP_NUM];
extern int bmp_i;
int flag_login=0;
int flag_keyboard=0;
int FLAG_Window = 3;//��Ǵ���Keyboard�Ľ���
extern DataPoint *head;//�ͷ�����������Դ

Task_List HList;

int main(void)
{	
	
	BaseType_t Return_Status=pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	/* ������Ӳ����ʼ�� */
  BSP_Init();
//	HList=Task_ListInit();
	
  printf("���ڷ������ݴ����ж�,����������!\n");	
	
	Return_Status=xTaskCreate( (TaskFunction_t) AppTaskCreate,
														 (const char *  ) "AppTaskCreate",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 1,
														 (TaskHandle_t *) &AppTaskCreate_Handle);
														 
	if(Return_Status==pdPASS)
	{			
		vTaskStartScheduler();   /* �������񣬿������� */
	}else
    {
			return -1;
		} 
		
		
	while(1);/* ��������ִ�е����� */  
}

static void AppTaskCreate(void)
{
	
	BaseType_t Return_Status=pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	taskENTER_CRITICAL();//�����ٽ���
	
	BinarySem_Handle = xSemaphoreCreateBinary();/* ������ֵ�ź��� */
	BinarySem_FLASH_Handle= xSemaphoreCreateBinary();/* ������ֵ�ź��� */
	BinarySem_ADC_TEMP_Handle= xSemaphoreCreateBinary();/* ������ֵ�ź��� */
	BinarySem_RTC_DATETIME_Handle= xSemaphoreCreateBinary();/* ������ֵ�ź��� */
	BinarySem_LCD_Handle= xSemaphoreCreateBinary();/* ������ֵ�ź��� */
 	BinarySem_LCD_DEL_Handle= xSemaphoreCreateBinary();/* ������ֵ�ź��� */
	
	EventGroup_Peripheral_Handle=xEventGroupCreate();
	EventGroup_next_Handle=xEventGroupCreate();
	EventGroup_prev_Handle=xEventGroupCreate();
	EventGroup_branch_Handle=xEventGroupCreate();
	if(NULL != BinarySem_Handle)
    printf("BinarySem_Handle��ֵ�ź��������ɹ�!\n");
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_USART1_DMA_Task,
														 (const char *  ) "usart1_dma",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 2,
														 (TaskHandle_t *) &BSP_USART1_DMA_Handle);
														 
	if(Return_Status==pdPASS)
	{	
		printf("����Uart_Task����ɹ�!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_I2C_EE_Task,
														 (const char *  ) "i2c_ee",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 4,
														 (TaskHandle_t *) &BSP_I2C_EE_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����I2C_EE_Task����ɹ�!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_SPI_FLASH_Task,														
														 (const char *  ) "spi_flash",
														 (uint32_t      ) 512,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 5,
														 (TaskHandle_t *) &BSP_SPI_FLASH_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����SPI_FLASH_Task����ɹ�!\n");		
	}
	
	Return_Status=xTaskCreate( (TaskFunction_t) BSP_ADC_DMA_TEMP_Task,														
														 (const char *  ) "adc_temp",
														 (uint32_t      ) 128,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 3,
														 (TaskHandle_t *) &BSP_ADC_DMA_TEMP_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����BSP_ADC_DMA_TEMP_Task����ɹ�!\n");		
	}
	
	
		Return_Status=xTaskCreate( (TaskFunction_t) BSP_RTC_DATETIME_Task,														
														 (const char *  ) "rtc_date",
														 (uint32_t      ) 128,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_RTC_DATETIME_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����BSP_RTC_DATETIME_Task����ɹ�!\n");		
	}
	
			Return_Status=xTaskCreate( (TaskFunction_t) BSP_LCD_Init_Task,														
														 (const char *  ) "lcd_show",
														 (uint32_t      ) 1024*8,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_LCD_Init_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����BSP_LCD_Init_Task����ɹ�!\n");		
	}
				Return_Status=xTaskCreate( (TaskFunction_t) BSP_LCD_RETURN_Task,														
														 (const char *  ) "lcd_show",
														 (uint32_t      ) 1024*8,
														 (void *		    ) NULL,
														 (UBaseType_t   ) 8,
														 (TaskHandle_t *) &BSP_LCD_Return_Handle);
	
		if(Return_Status==pdPASS)
	{	
		printf("����BSP_LCD_DEL_Task����ɹ�!\n");		
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
//		printf("����BSP_LCD_DEL_Task����ɹ�!\n");		
//	}

	
	vTaskDelete(AppTaskCreate_Handle);//ɾ��AppTaskCreate����
	taskEXIT_CRITICAL();//�˳��ٽ���
}

/***********************************************************************
  * @ ������  �� BSP_USART1_DMA_Task
  * @ ����˵���� USART1����+DMA��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_USART1_DMA_Task(void)
{
	
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  while (1)
  {
		taskENTER_CRITICAL();
    //��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�``
//		xReturn = xSemaphoreTake(BinarySem_Handle,/* ��ֵ�ź������ */
//                              portMAX_DELAY); /* �ȴ�ʱ�� */
//    if(pdPASS == xReturn)
		
//    {
		if(*USART1_RecvBuffer){
      printf("�յ�����:%s\n",USART1_RecvBuffer);
      memset(USART1_RecvBuffer,0,BSP_USART1_BufferSize);/* ���� */
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
  * @ ������  �� BSP_USART1_DMA_Task
  * @ ����˵���� USART1����+DMA��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_I2C_EE_Task(void)
{
	while(1){
//		taskENTER_CRITICAL();
	BaseType_t	pxHigherPriorityTaskWoken;
//	vTaskDelay(100);
//	BSP_I2C_EE_API(USART1_RecvBuffer);
	//��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�``
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
		xReturn = xSemaphoreTake(BinarySem_Handle,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
		
	  if(pdPASS == xReturn)
    {
	for (int i=0; i<=10; i++ ) //��仺��
  {   
    I2c_Buf_Write[i] = USART1_RecvBuffer[i];
    printf("0x%02X  ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
    printf("\n\r");    
   }

  //��I2c_Buf_Write��˳�����������д��EERPOM�� 
	BSP_I2C_EE_WriteBuffer(I2c_Buf_Write, CHAR_ADDR, 10);
	FileSystem_Write("0:IIC_EEPROMд������.txt",I2c_Buf_Write);
	 
	 //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
	BSP_I2C_EE_ReadBuffer(I2c_Buf_Read, CHAR_ADDR, 10); 
	 
	 //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (int i=0; i<10; i++)
	{	
    printf("0x%02X  ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
    printf("\n\r");
	}
   }
		xSemaphoreGive(BinarySem_FLASH_Handle);	//�ͷ�FLASH����Ķ�ֵ�ź���
//	 taskEXIT_CRITICAL();
 }
}

static void BSP_SPI_FLASH_Task(void)
{
	while(1)
	{
//		taskENTER_CRITICAL();
		/* ��ȡ Flash Device ID */
  BaseType_t 	xReturn = xSemaphoreTake(BinarySem_FLASH_Handle,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
	
		
	if(xReturn==pdPASS){
	Flash_DeviceID=BSP_SPI_FLASH_ReadDeviceID();
	vTaskDelay(20);
		
	/* ��ȡ SPI Flash ID */
	FlashID=BSP_SPI_FLASH_ReadID();
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, Flash_DeviceID);
	
	if(FlashID==BSP_FLASH_ID256 && FLASH_WriteFlag==0)
	{
		printf("\r\n��⵽SPI FLASH W25Q256 !\r\n");
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		BSP_SPI_FLASH_SectorErase(Flash_SectorToErase);
		/* �����ͻ�����������д��flash�� */
//		BSP_SPI_FLASH_WriteBuffer(Flash_Tx_Buffer,Flash_WriteAddress,10);
//		printf("\r\nд�������Ϊ��\r\n%s", Flash_Tx_Buffer);
		
		BSP_SPI_FLASH_WriteBuffer(USART1_RecvBuffer,Flash_WriteAddress,10);
		printf("\r\nд���USART1����Ϊ��\r\n%s", USART1_RecvBuffer);
		FileSystem_Write("0:SPI_FLASHд������.txt",USART1_RecvBuffer);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		BSP_SPI_FLASH_ReadBuffer(Flash_Rx_Buffer,Flash_WriteAddress,10);
		printf("\r\n����������Ϊ��\r\n%s", Flash_Rx_Buffer);		
		FLASH_WriteFlag=1;
	}
	else
	{
		printf("\r\n��⵽SPI FLASH W25Q256 !\r\n");
		memset(Flash_Rx_Buffer,0,10);/* ���� */
		BSP_SPI_FLASH_ReadBuffer(Flash_Rx_Buffer,(Flash_WriteAddress+5),5);
		printf("\r\n����������Ϊ��\r\n%s", Flash_Rx_Buffer);		
		FLASH_WriteFlag=0;
		
		xSemaphoreGive(BinarySem_RTC_DATETIME_Handle);//�ͷ�RTC��ʱ����Ķ�ֵ�ź���

		
	}
	xSemaphoreGive(BinarySem_ADC_TEMP_Handle);	//�ͷ�ADC1оƬ�¶ȼ��Ķ�ֵ�ź���
//	taskEXIT_CRITICAL();
//	portYIELD();
//	vTaskDelay(10);
//	/*FLASH�ϵ�*/
//	BSP_SPI_FLASH_PowerDown();
}
	}
	
}
static void BSP_ADC_DMA_TEMP_Task(void)
{
	while(1)
		{
			

	    BaseType_t 	xReturn = xSemaphoreTake(BinarySem_ADC_TEMP_Handle,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */
				uint8_t tem[10];
	if(xReturn==pdPASS)
	{
	  uint16_t	Current_Temp=(ADC_ConvertedValue_temp*3.3/4096-0.76)*1000/2.5+25;
		printf("\r\n The IC current tem= %3d ��\r\n", Current_Temp/10);
	}

 }
}

static void BSP_RTC_DATETIME_Task(void)
{
	BaseType_t 	xReturn=pdPASS;
	while(1)
	{
		
		
	/*
	 * ���������ù�RTCʱ��֮��������ݼĴ���0д��һ�����������
	 * ����ÿ�γ����������е�ʱ���ͨ����ⱸ�ݼĴ���0��ֵ���ж�
	 * RTC �Ƿ��Ѿ����ù���������ù��Ǿͼ������У����û�����ù�
	 * �ͳ�ʼ��RTC������RTC��ʱ�䡣
	 */
//				 	xReturn = xSemaphoreTake(BinarySem_RTC_DATETIME_Handle,/* ��ֵ�ź������ */
//                              portMAX_DELAY); /* �ȴ�ʱ�� */
		
		EventBits_t bit=xEventGroupWaitBits(EventGroup_Peripheral_Handle, /* �¼������� */
		                                    DATETIME_EVENT,/* �����̸߳���Ȥ���¼� */
	                                     	pdTRUE,/* �˳�ʱ����¼�λ */
		                                    pdTRUE,   /* �������Ȥ�������¼� */   
		                                    portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
		
	if((xReturn==pdPASS)&&(bit & DATETIME_EVENT)){
	if(RTC_ReadBackupRegister(RTC_BKP_DRX)!=RTC_BKP_DATA)
	{
		BSP_RTC_Config();
	}
	else 
	{
		/* ����Ƿ��Դ��λ */
		if(RCC_GetFlagStatus(RCC_FLAG_PORRST)!=RESET)
		{
			 printf("\r\n ������Դ��λ....\r\n");
		}
		/* ����Ƿ��ⲿ��λ */
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST)!=RESET)
		{
			BSP_RTC_Config();
			printf("\r\n �����ⲿ��λ....\r\n");
		}
		printf("\r\n ����Ҫ��������RTC....\r\n");
		/* ʹ�� PWR ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
    PWR_BackupAccessCmd(ENABLE);
    /* �ȴ� RTC APB �Ĵ���ͬ�� */
    RTC_WaitForSynchro(); 
	}
	/* ��ʾʱ������� */
	BSP_RTC_SHOW_DateTime();
	
//	portYIELD();
 }
}
}
int i=0;

void Init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	LCDD_Init();	   //Һ������ʼ��
	TP_Init();
	
//		/*FATFS+SDIOϵͳ��ʼ��	*/
//	FileSystem_Init();
	
		GUI_Init();/* ��ʼ��GUI */
	GUI_SetBkColor(GUI_WHITE);
  GUI_CURSOR_Show();/* ������� */
	GUI_UC_SetEncodeUTF8();/* ��ʾ�Զ��庺�� */
  GUI_Clear();
	WM_EnableMemdev(1);
  WM_MULTIBUF_Enable(1);/* ���������� */
	GUI_Exec();
}

static void BSP_LCD_Init_Task(void)
{
//	BaseType_t xReturn=pdPASS;
//BaseType_t 	xReturn=xSemaphoreTake(BinarySem_LCD_Handle,/* ��ֵ�ź������ */
//                              portMAX_DELAY); /* �ȴ�ʱ�� */
//	if(xReturn==pdPASS){
		
		
//	vTaskDelay(100);
		
		if(flag_login==0)
	  {
//			LV.Login=CreateLogin();
			/*���Խ�����*/
			/*3.6�޸ĵ������������ͼ����ʾ��������PPTһ��   */
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
		EventBits_t bit=xEventGroupWaitBits(EventGroup_next_Handle, /* �¼������� */
		                                    LOGIN_EVENT|KEYBOARD_EVENT|MENU_EVENT|MANAGE_SELECT_EVENT|
																				SYSTEM_SET_EVENT|MENU_CHILD_EVENT|MONITOR_EVENT|DISPOSITION_EVENT|
																				CONNECT_EVENT|CANLENDER_EVENT|LOG_EVENT|LOG_CHILD_EVENT|
																				CALIBRATE_EVENT|DISPOSITION_CHILD_EVENT,/* �����̸߳���Ȥ���¼� */
	                                     	pdTRUE,/* �˳�ʱ����¼�λ */
		                                    pdFALSE,   /* �������Ȥ�������¼� */   
		                                    portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
		
		
//		if(bit&KEYBOARD_EVENT)
//		{
//			printf("�л��� Keyboard ҳ��...\n");
//			WM_DeleteWindow(LV.Login);//�滻Ϊɾ��
////			WM_HideWindow(LV.Login);
//			KV.Keyboard=Createnumber();//�滻Ϊ����
////			WM_ShowWindow(KV.Keyboard);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			
//			 
//		}
//		if(bit&LOGIN_EVENT)
//		{
//			printf("�л��� Login ҳ��...\n");
//			WM_DeleteWindow(KV.Keyboard);//�滻Ϊɾ��
////			WM_HideWindow(KV.Keyboard);
//			 LV.Login=CreateLogin();//�滻Ϊ����
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
//			printf("�л��� Menu ҳ��...\n");
//			WM_DeleteWindow(LV.Login);
////			WM_HideWindow(LV.Login);//�滻Ϊɾ��
//			MV.Menu=CreateMenu_Win();
////			WM_ShowWindow(MV.Menu);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Menu Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		if(bit&MANAGE_SELECT_EVENT)
//		{
//			printf("�л��� Manage Select ҳ��...\n");
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
//			printf("�л��� System Set ҳ��...\n");
//			WM_DeleteWindow(MSV.Manage_Sel);
//			SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(MV.Menu);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Manage Select Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//		}
//		
//		if(bit & MENU_CHILD_EVENT) {
//				printf("MENU �л��� MENU_CHILD ҳ��...\n");
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
//			printf("�л��� Monitor ҳ��...\n");
//			WM_DeleteWindow(MV.Menu);//�滻Ϊɾ��
////			WM_HideWindow(LV.Login);
//			MONV.Monitor=CreateMonitor_Win();//�滻Ϊ����
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
			printf("Login �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(LV.Login);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(DISPV.Disposition)
			{
				printf("Disposition �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(DISPV.Disposition);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Keyboard Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(CONTV.Connect)
			{
				printf("Connect �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(CONTV.Connect);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(CANLV.Canlender)
			{
				printf("Canlender �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(CANLV.Canlender);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			 
			break; 
			}
			if(LOGV.Log)
			{
				printf("Log �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(LOGV.Log);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(LOGV.Log_Child)
			{
				printf("Log Child �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(LOGV.Log_Child);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Log Child Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(CALIV.Calibrate)
			{
				printf("Calibrate �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(CALIV.Calibrate);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
			if(DISPV.Disposition_Child)
			{
					printf("Disposition Child �л��� Keyboard ҳ��...\n");
			WM_DeleteWindow(DISPV.Disposition_Child);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			KV.Keyboard=Createnumber();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Calibrate Task FreeBytes %ld\n",sssss);
			GUI_Exec();
				break;
			}
		}
		case LOGIN_EVENT:
		{
			printf("�л��� Login ҳ��...\n");
			WM_DeleteWindow(KV.Keyboard);//�滻Ϊɾ��
//			WM_HideWindow(KV.Keyboard);
			 LV.Login=CreateLogin();//�滻Ϊ����
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
			printf("�л��� Menu ҳ��...\n");
			WM_DeleteWindow(LV.Login);
//			WM_HideWindow(LV.Login);//�滻Ϊɾ��
			MV.Menu=CreateMenu_Win();
//			WM_ShowWindow(MV.Menu);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Menu Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
		}
		case MANAGE_SELECT_EVENT:
		{
			printf("�л��� Manage Select ҳ��...\n");
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
			printf("�л��� System Set ҳ��...\n");
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
				printf("MENU �л��� MENU_CHILD ҳ��...\n");
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
			printf("�л��� Monitor ҳ��...\n");
			WM_DeleteWindow(MV.Menu);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			MONV.Monitor=CreateMonitor_Win();//�滻Ϊ����
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
			printf("�л��� Disposition ҳ��...\n");
			WM_DeleteWindow(MV.Menu);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			DISPV.Disposition=CreateWin_Disposition();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Disposition Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			break;
			}
			
//			if(KV.Keyboard)
//			{
//							printf("�л��� Disposition ҳ��...\n");
//			WM_DeleteWindow(MV.Menu);//�滻Ϊɾ��
////			WM_HideWindow(LV.Login);
//			DISPV.Disposition=CreateWin_Disposition();//�滻Ϊ����
////			WM_ShowWindow(KV.Keyboard);
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Init Disposition Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			break;
//			}
		}
		case CONNECT_EVENT:
		{
			printf("�л��� Connect ҳ��...\n");
			WM_DeleteWindow(SSV.System_Set);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			CONTV.Connect=CreateConnect_Win();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Connect Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case CANLENDER_EVENT:
		{
				printf("�л��� Canlender ҳ��...\n");
			WM_DeleteWindow(SSV.System_Set);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			CANLV.Canlender=CreateCanlender_Window();//�滻Ϊ����
//			WM_ShowWindow(KV.Keyboard);
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case CALIBRATE_EVENT:
		{
				printf("�л��� Canlender ҳ��...\n");
			WM_DeleteWindow(SSV.System_Set);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
//			CALIV.Calibrate=crea();//�滻Ϊ����
			CALIV.Calibrate=CreateCalibrate_Window();
			
			sssss=GUI_ALLOC_GetNumFreeBytes ();
			printf("Init Canlender Task FreeBytes %ld\n",sssss);
			GUI_Exec();
			
			break;
		}
		case LOG_EVENT:
		{
			printf("�л��� Log ҳ��...\n");
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
			printf("�л��� Log Child ҳ��...\n");
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
				printf("�л��� Disposition Child ҳ��...\n");
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
		
//	TP_Init();//����У׼���
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
	
            // ǿ�ƴ�����¼����

while(1){
		EventBits_t bit2=xEventGroupWaitBits(EventGroup_prev_Handle, /* �¼������� */
		                                    LOGIN_EVENT|MENU_EVENT|MANAGE_SELECT_EVENT|SYSTEM_SET_EVENT
																				|MENU_CHILD_EVENT|MONITOR_EVENT|DISPOSITION_EVENT|
																				CONNECT_EVENT|CANLENDER_EVENT|LOG_EVENT|LOG_CHILD_EVENT|
																				CALIBRATE_EVENT|DISPOSITION_CHILD_EVENT	,/* �����̸߳���Ȥ���¼� */
	                                     	pdTRUE,/* �˳�ʱ����¼�λ */
		                                    pdFALSE,   /* �������Ȥ�������¼� */   
		                                    portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */

//	   if(bit2 & LOGIN_EVENT) {
//				printf("�л��� Login ҳ��...\n");
//			WM_DeleteWindow(MV.Menu);
////		SSV.System_Set=CreateSystem_Set_Win();
//			 LV.Login=CreateLogin();//�滻Ϊ����
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
//				printf("Manage Select�л��� Menu ҳ��...\n");
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
//				printf("System Set�л��� Menu ҳ��...\n");
//			WM_DeleteWindow(SSV.System_Set);
////		SSV.System_Set=CreateSystem_Set_Win();
////			WM_ShowWindow(LV.Login);
//			MV.Menu=CreateMenu_Win();
//			sssss=GUI_ALLOC_GetNumFreeBytes ();
//			printf("Return Menu Task FreeBytes %ld\n",sssss);
//			GUI_Exec();
//			}	
//			if(MONV.Monitor) {
//				printf("Monitor�����л��� MENU ҳ��...\n");
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
//				printf("�л��� Manage Select ҳ��...\n");
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
				printf("�л��� Login ҳ��...\n");
			WM_DeleteWindow(MV.Menu);
//		SSV.System_Set=CreateSystem_Set_Win();
			 LV.Login=CreateLogin();//�滻Ϊ����
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
				printf("Manage Select�л��� Menu ҳ��...\n");
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
				printf("System Set�л��� Menu ҳ��...\n");
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
				printf("Monitor�����л��� MENU ҳ��...\n");
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
				printf("Menu_Child �����л��� MENU ҳ��...\n");
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
				printf("Connect �����л��� MENU ҳ��...\n");
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
				printf("Canlender �����л��� MENU ҳ��...\n");
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
				printf("Log �����л��� MENU ҳ��...\n");
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
				printf("Calibrate �����л��� MENU ҳ��...\n");
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
				printf("Disposition �����л��� MENU ҳ��...\n");
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
				printf("System Set �л��� Manage Select ҳ��...\n");
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
					printf("Log �л��� Manage Select ҳ��...\n");
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
					printf("Canlender �л��� System Set ҳ��...\n");
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
						printf("Canlender �л��� System Set ҳ��...\n");
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
						printf("Calibrate �л��� System Set ҳ��...\n");
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
				printf("Keyboard �л��� Dispositon ҳ��...\n");
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
			printf("�л��� Disposition ҳ��...\n");
			WM_DeleteWindow(DISPV.Disposition_Child);//�滻Ϊɾ��
//			WM_HideWindow(LV.Login);
			DISPV.Disposition=CreateWin_Disposition();//�滻Ϊ����
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
				printf("Keyboard �л��� Connect ҳ��...\n");
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
				printf("Keyboard �л��� Canlender ҳ��...\n");
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
				printf("Keyboard �л��� Log ҳ��...\n");
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
				printf("Log Child �л��� Log ҳ��...\n");
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
				printf("Keyboard �л��� Log Child ҳ��...\n");
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
				printf("Keyboard �л��� Calibrate ҳ��...\n");
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
				printf("Keyboard �л��� Dispositon Child ҳ��...\n");
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
//				printf("�л��� System Set ҳ��...\n");
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
	  
            // ǿ�ƴ�����¼����
while(1){	
		EventBits_t bit3=xEventGroupWaitBits(EventGroup_branch_Handle, /* �¼������� */
		                                    MENU_CHILD_EVENT,/* �����̸߳���Ȥ���¼� */
	                                     	pdTRUE,/* �˳�ʱ����¼�λ */
		                                    pdFALSE,   /* �������Ȥ�������¼� */   
		                                    portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
	
	if(bit3 & MENU_CHILD_EVENT) {
				printf("�ӽ����л��� MENU ҳ��...\n");
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
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	/* ���ڳ�ʼ��	*/
	BSP_USART_GPIO_Config();
	
	/* UASRT1��DMA��ʼ��	*/
	BSP_USART1_DMA_Config();
	
//	/* I2C2��EEPROM��ʼ��	*/
  BSP_I2C_EE_Init();
	
	/* SPI��FLASH��ʼ��	*/
	BSP_SPI_FLASH_Config();
	
	/* ADC1+DMA��оƬ�¶ȼ���ʼ��	*/
	ADC_Cmd(ADC1, DISABLE);
	BSP_ADC1_MODE_Config();
	
	/*RTC���ڼ�ʱʱ�ӳ�ʼ��	*/
	BSP_RTC_CLK_Config();
	
	/*PWRֹͣģʽ�������ѳ�ʼ��	*/
	BSP_PWR_EXTI_Key_Config();
	
	/*SPI+LCD��ʾ��У׼��ʼ��	*/
	LCDD_Init();	   //Һ������ʼ��
	TP_Init();
	
	/*FATFS+SDIOϵͳ��ʼ��	*/
	FileSystem_Init();
	
	
//	NVIC_SetPriority(PendSV_IRQn, 15);  // ������ȼ�
//  NVIC_SetPriority(SysTick_IRQn, 15);
//	my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
//	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
//	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
    
	
  /*STemWIN��ʾ����ͷ�ļ���ʼ��	*/
	GUI_Init();/* ��ʼ��GUI */
	GUI_SetBkColor(GUI_WHITE);
  GUI_CURSOR_Show();/* ������� */
	GUI_UC_SetEncodeUTF8();/* ��ʾ�Զ��庺�� */
  GUI_Clear();
	WM_EnableMemdev(1);
//	GUI_EnableAlpha(1);
  WM_MULTIBUF_Enable(1);/* ���������� */
	GUI_Exec();
	
	XYBlock_Init();
	
}

/********************************END OF FILE****************************/
