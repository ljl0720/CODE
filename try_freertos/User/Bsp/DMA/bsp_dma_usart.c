#include "bsp_dma_usart.h"
#include "bsp_usart.h"


/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern uint8_t USART_SendBuffer[];

/*配置USART1的DMA2通道和工作模式*/
void BSP_USART1_DMA_Config(void)
{
	
	DMA_InitTypeDef  DMA_InitStruct;
	//开启DMA2时钟
	RCC_APB1PeriphClockCmd(BSP_USART1_DMA_CLK,ENABLE);
	//复位初始化DMA2数据流7
	DMA_DeInit(BSP_USART1_TX_DMA_STREAM);
	//确定DMA2数据流7的复位成功
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=DISABLE);
	//选择通道4
	DMA_InitStruct.DMA_Channel=BSP_USART1_DMA_CHANNEL;
	//选择源数据地址为USART1的数据寄存器地址
	DMA_InitStruct.DMA_PeripheralBaseAddr=BSP_USART1_DR;
	//目标地址为内存地址
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)USART_SendBuffer;
	//传输方向为存储器到外设
	DMA_InitStruct.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	//传输数据大小
	DMA_InitStruct.DMA_BufferSize=(uint32_t)BSP_USART1_BufferSize;
	//外设地址不自增
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//内存地址不自增
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Disable;
	//外设数据单位 32位
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	//内存数据单位 32位
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	//一次循环
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	//中等优先级
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
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
	DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF7);
	DMA_ITConfig(BSP_USART1_TX_DMA_STREAM, DMA_IT_TE, ENABLE);
	//使能DMA2
	DMA_Cmd(BSP_USART1_TX_DMA_STREAM,ENABLE);
	//等待DMA2数据流有效
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=ENABLE){}
	
}


void Uart_DMA_Rx_Data(void)
{
  BaseType_t	pxHigherPriorityTaskWoken;
  // 关闭DMA ，防止干扰
  DMA_Cmd(BSP_USART1_TX_DMA_STREAM, DISABLE);      
  // 清DMA标志位
  DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF2);         
  //  重新赋值计数值，必须大于等于最大可能接收到的数据帧数目
  DMA_SetCurrDataCounter(BSP_USART1_TX_DMA_STREAM,BSP_USART1_BufferSize);     
  DMA_Cmd(BSP_USART1_TX_DMA_STREAM, ENABLE);       
  /* 
  xSemaphoreGiveFromISR(SemaphoreHandle_t xSemaphore,
                      BaseType_t *pxHigherPriorityTaskWoken);
  */

  //给出二值信号量 ，发送接收到新数据标志，供前台程序查询
  //xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken);	//释放二值信号量
  //如果需要的话进行一次任务切换，系统会判断是否需要进行切换
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

}