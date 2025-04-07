#include "./Bsp/usart1/bsp_usart.h"


/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

uint8_t USART1_SendBuffer[BSP_USART1_BufferSize];
uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];
uint16_t USART_RX_STA=0;

/* 信号量句柄引用 */
extern SemaphoreHandle_t BinarySem_Handle ;
extern SemaphoreHandle_t BinarySem_LCD_DEL_Handle;
/*
***  初始化USART1作为调试串口
***  默认为USART1串口，引脚对应 TX RX 位 PA9 PA10
***  配置NVIC优先级和中断
*/
void BSP_USART_GPIO_Config(void)
{
	
	GPIO_InitTypeDef 		GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 		NVIC_InitStruct;
	
	//使能USART1和其对应GPIOA的时钟
	RCC_AHB1PeriphClockCmd(BSP_USART1_GPIO_PORT_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(BSP_USART1_CLK,ENABLE);
	
	//将引脚复用到对应的外设引脚
	GPIO_PinAFConfig(BSP_USART1_TX_GPIO,BSP_USART1_TX_SOURCE,BSP_USART1_TX_AF);
	GPIO_PinAFConfig(BSP_USART1_RX_GPIO,BSP_USART1_RX_SOURCE,BSP_USART1_RX_AF);	
	
	//定义引脚为复用推挽模式 处于上拉模式
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;          //复用模式
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;				 //推挽模式
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;          //上拉模式
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;     //工作频率为50MHz
	GPIO_InitStruct.GPIO_Pin=BSP_USART1_TX_PIN;      //TX引脚
	GPIO_Init(BSP_USART1_TX_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;          //复用模式
	GPIO_InitStruct.GPIO_Pin=BSP_USART1_RX_PIN;      //RX引脚
	GPIO_Init(BSP_USART1_RX_GPIO,&GPIO_InitStruct);
	
	//对USART1串口配置波特率/字长/停止位/校验位/工作模式/硬件流控制
	USART_InitStruct.USART_BaudRate=BSP_USART1_BaudRate;            						//115200波特率
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;          						//8位字长
	USART_InitStruct.USART_StopBits=USART_StopBits_1;               						//停止位占1位
	USART_InitStruct.USART_Parity=USART_Parity_No;                  						//无校验位
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;        						//收发均可
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //不使用硬件控制流
	USART_Init(BSP_USART1,&USART_InitStruct);
	
	//配置USART1对应的NVIC
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);                             //设置中断分组为4
	NVIC_InitStruct.NVIC_IRQChannel=BSP_USART1_IRQChannel;                      //设置中断源
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=4;                        //设置抢占优先级 0—15 越小优先级越高
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;                               //设置子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;                                  //使能NVIC
	NVIC_Init(&NVIC_InitStruct);
	
	       
	USART_ITConfig(BSP_USART1,USART_IT_IDLE,ENABLE);                           //使能接收中断                                                  //使能空闲中断
	
	USART_DMACmd(BSP_USART1,USART_DMAReq_Rx,ENABLE);
	//USART_DMACmd(BSP_USART1,USART_DMAReq_Tx,ENABLE);
	
	USART_Cmd(BSP_USART1,ENABLE);
	
}
void BSP_USART1_DMA_Config(void)
{
	
	DMA_InitTypeDef  DMA_InitStruct;
	//开启DMA2时钟
	RCC_AHB1PeriphClockCmd(BSP_USART1_DMA_CLK,ENABLE);
	//复位初始化DMA2数据流7
	DMA_DeInit(BSP_USART1_TX_DMA_STREAM);
	//确定DMA2数据流7的复位成功
//	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=DISABLE){}
	//选择通道4
	DMA_InitStruct.DMA_Channel=BSP_USART1_DMA_CHANNEL;
	//选择源数据地址为USART1的数据寄存器地址
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)BSP_USART1_DR;
	//目标地址为内存地址
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)USART1_RecvBuffer;
	//传输方向为存储器到外设
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralToMemory;
	//传输数据大小
	DMA_InitStruct.DMA_BufferSize=BSP_USART1_BufferSize;
	//外设地址不自增
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//内存地址不自增
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//外设数据单位 32位
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//内存数据单位 32位
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	//一次循环
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
	//中等优先级
	DMA_InitStruct.DMA_Priority=DMA_Priority_VeryHigh;
	//禁用FIFO
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;	
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	//突发传输
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	//突发传输
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	//配置DMA2的数据流7
	DMA_Init(BSP_USART1_TX_DMA_STREAM,&DMA_InitStruct);
	//清除事件标志位 DMA_FLAG_TCIFx对应数据流x
	DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF2);
	//DMA_ITConfig(BSP_USART1_TX_DMA_STREAM, DMA_IT_TE, ENABLE);
	//使能DMA2
	DMA_Cmd(BSP_USART1_TX_DMA_STREAM,ENABLE);
	//等待DMA2数据流有效
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=ENABLE){}
	
}


void BSP_USART1_DMA_Rx_Data(void)
{
  BaseType_t	pxHigherPriorityTaskWoken;
  // 关闭DMA ，防止干扰
  DMA_Cmd(BSP_USART1_RX_DMA_STREAM, DISABLE);      
  // 清DMA标志位
  DMA_ClearFlag(BSP_USART1_RX_DMA_STREAM,DMA_FLAG_TCIF2);         
  //  重新赋值计数值，必须大于等于最大可能接收到的数据帧数目
  DMA_SetCurrDataCounter(BSP_USART1_RX_DMA_STREAM,BSP_USART1_BufferSize);     
  DMA_Cmd(BSP_USART1_RX_DMA_STREAM, ENABLE);       
  /* 
  xSemaphoreGiveFromISR(SemaphoreHandle_t xSemaphore,
                      BaseType_t *pxHigherPriorityTaskWoken);
  */

  //给出二值信号量 ，发送接收到新数据标志，供前台程序查询
  xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken);	//释放二值信号量
	printf("信号量\n");
	
  //如果需要的话进行一次任务切换，系统会判断是否需要进行切换	
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

}

//重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(BSP_USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(BSP_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(BSP_USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(BSP_USART1);
}


/*发送单个字符*/
void BSP_USART1_SendByte(USART_TypeDef *pUSARTx,uint8_t ch)
{

	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
	/* 等待发送数据寄存器为空 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	
}

/*发送字符串*/
void BSP_USART1_SendString(USART_TypeDef *pUSARTx, char *str)
{
	
	unsigned int len=0;
	do
  {  	
		//发送以字符串为基地址每次偏移一个字符的数据
		BSP_USART1_SendByte(pUSARTx,*(str+len));
		len++;
		
	}while(*(str+len)!='\0');
	
	 /* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	
}

/*接收单个字符*/
uint16_t BSP_USART1_RecvByte(USART_TypeDef *pUSARTx)
{
	/* 从USART读取一个字节数据 */
	uint16_t USART1_RecvData=USART_ReceiveData(pUSARTx);
	/* 等待接收数据寄存器非空 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_RXNE)!=RESET);
	
	return USART1_RecvData;
	
}

/*接收字符串*/
char* BSP_USART1_RecvString(USART_TypeDef *pUSARTx,char *str)
{
	
	unsigned int len=0;
	do
	{
		
		BSP_USART1_RecvByte(pUSARTx);
		len++;
		
	}while(*(str+len)!='\0');
		 /* 等待接收完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_RXNE)==RESET);
	return  str;
}

//void USART1_IRQHandler(void)
//{	
//	
//	uint8_t Res;
//	if(USART_GetFlagStatus(BSP_USART1,USART_FLAG_RXNE)!=RESET)//接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res=BSP_USART1_RecvByte(BSP_USART1);	//读取接收到的数据
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)
//				{
//					USART_RX_STA=0;      //接收错误,重新开始
//				}else{
//					USART_RX_STA|=0x8000;//接收完成了 
//				}	
//			}else{							//还没收到0X0D 并未到字符串结尾
//				if(Res==0x0d){
//					
//					USART_RX_STA|=0x4000;
//					
//				}else{
//					
//					USART1_RecvBuffer[USART_RX_STA&0x3FFF]=Res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(BSP_USART1_RECV_LEN-1))
//					{
//						USART_RX_STA=0;//接收数据错误,重新开始接收
//					}
//				}
//			}
//		}
//		
//	}
//	
//}
