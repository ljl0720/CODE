#include "bsp_dma_usart.h"
#include "bsp_usart.h"


/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern uint8_t USART_SendBuffer[];

/*����USART1��DMA2ͨ���͹���ģʽ*/
void BSP_USART1_DMA_Config(void)
{
	
	DMA_InitTypeDef  DMA_InitStruct;
	//����DMA2ʱ��
	RCC_APB1PeriphClockCmd(BSP_USART1_DMA_CLK,ENABLE);
	//��λ��ʼ��DMA2������7
	DMA_DeInit(BSP_USART1_TX_DMA_STREAM);
	//ȷ��DMA2������7�ĸ�λ�ɹ�
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=DISABLE);
	//ѡ��ͨ��4
	DMA_InitStruct.DMA_Channel=BSP_USART1_DMA_CHANNEL;
	//ѡ��Դ���ݵ�ַΪUSART1�����ݼĴ�����ַ
	DMA_InitStruct.DMA_PeripheralBaseAddr=BSP_USART1_DR;
	//Ŀ���ַΪ�ڴ��ַ
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)USART_SendBuffer;
	//���䷽��Ϊ�洢��������
	DMA_InitStruct.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	//�������ݴ�С
	DMA_InitStruct.DMA_BufferSize=(uint32_t)BSP_USART1_BufferSize;
	//�����ַ������
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//�ڴ��ַ������
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Disable;
	//�������ݵ�λ 32λ
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	//�ڴ����ݵ�λ 32λ
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	//һ��ѭ��
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	//�е����ȼ�
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
	//����FIFO
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;	
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	//ͻ������
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	//ͻ������
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	//����DMA2��������7
	DMA_Init(BSP_USART1_TX_DMA_STREAM,&DMA_InitStruct);
	//����¼���־λ DMA_FLAG_TCIFx��Ӧ������x
	DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF7);
	DMA_ITConfig(BSP_USART1_TX_DMA_STREAM, DMA_IT_TE, ENABLE);
	//ʹ��DMA2
	DMA_Cmd(BSP_USART1_TX_DMA_STREAM,ENABLE);
	//�ȴ�DMA2��������Ч
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=ENABLE){}
	
}


void Uart_DMA_Rx_Data(void)
{
  BaseType_t	pxHigherPriorityTaskWoken;
  // �ر�DMA ����ֹ����
  DMA_Cmd(BSP_USART1_TX_DMA_STREAM, DISABLE);      
  // ��DMA��־λ
  DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF2);         
  //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ
  DMA_SetCurrDataCounter(BSP_USART1_TX_DMA_STREAM,BSP_USART1_BufferSize);     
  DMA_Cmd(BSP_USART1_TX_DMA_STREAM, ENABLE);       
  /* 
  xSemaphoreGiveFromISR(SemaphoreHandle_t xSemaphore,
                      BaseType_t *pxHigherPriorityTaskWoken);
  */

  //������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
  //xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
  //�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

}