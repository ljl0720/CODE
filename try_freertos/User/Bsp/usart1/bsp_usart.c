#include "./Bsp/usart1/bsp_usart.h"


/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

uint8_t USART1_SendBuffer[BSP_USART1_BufferSize];
uint8_t USART1_RecvBuffer[BSP_USART1_BufferSize];
uint16_t USART_RX_STA=0;

/* �ź���������� */
extern SemaphoreHandle_t BinarySem_Handle ;
extern SemaphoreHandle_t BinarySem_LCD_DEL_Handle;
/*
***  ��ʼ��USART1��Ϊ���Դ���
***  Ĭ��ΪUSART1���ڣ����Ŷ�Ӧ TX RX λ PA9 PA10
***  ����NVIC���ȼ����ж�
*/
void BSP_USART_GPIO_Config(void)
{
	
	GPIO_InitTypeDef 		GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 		NVIC_InitStruct;
	
	//ʹ��USART1�����ӦGPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(BSP_USART1_GPIO_PORT_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(BSP_USART1_CLK,ENABLE);
	
	//�����Ÿ��õ���Ӧ����������
	GPIO_PinAFConfig(BSP_USART1_TX_GPIO,BSP_USART1_TX_SOURCE,BSP_USART1_TX_AF);
	GPIO_PinAFConfig(BSP_USART1_RX_GPIO,BSP_USART1_RX_SOURCE,BSP_USART1_RX_AF);	
	
	//��������Ϊ��������ģʽ ��������ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;          //����ģʽ
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;				 //����ģʽ
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;          //����ģʽ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;     //����Ƶ��Ϊ50MHz
	GPIO_InitStruct.GPIO_Pin=BSP_USART1_TX_PIN;      //TX����
	GPIO_Init(BSP_USART1_TX_GPIO,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;          //����ģʽ
	GPIO_InitStruct.GPIO_Pin=BSP_USART1_RX_PIN;      //RX����
	GPIO_Init(BSP_USART1_RX_GPIO,&GPIO_InitStruct);
	
	//��USART1�������ò�����/�ֳ�/ֹͣλ/У��λ/����ģʽ/Ӳ��������
	USART_InitStruct.USART_BaudRate=BSP_USART1_BaudRate;            						//115200������
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;          						//8λ�ֳ�
	USART_InitStruct.USART_StopBits=USART_StopBits_1;               						//ֹͣλռ1λ
	USART_InitStruct.USART_Parity=USART_Parity_No;                  						//��У��λ
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;        						//�շ�����
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //��ʹ��Ӳ��������
	USART_Init(BSP_USART1,&USART_InitStruct);
	
	//����USART1��Ӧ��NVIC
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);                             //�����жϷ���Ϊ4
	NVIC_InitStruct.NVIC_IRQChannel=BSP_USART1_IRQChannel;                      //�����ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=4;                        //������ռ���ȼ� 0��15 ԽС���ȼ�Խ��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;                               //���������ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;                                  //ʹ��NVIC
	NVIC_Init(&NVIC_InitStruct);
	
	       
	USART_ITConfig(BSP_USART1,USART_IT_IDLE,ENABLE);                           //ʹ�ܽ����ж�                                                  //ʹ�ܿ����ж�
	
	USART_DMACmd(BSP_USART1,USART_DMAReq_Rx,ENABLE);
	//USART_DMACmd(BSP_USART1,USART_DMAReq_Tx,ENABLE);
	
	USART_Cmd(BSP_USART1,ENABLE);
	
}
void BSP_USART1_DMA_Config(void)
{
	
	DMA_InitTypeDef  DMA_InitStruct;
	//����DMA2ʱ��
	RCC_AHB1PeriphClockCmd(BSP_USART1_DMA_CLK,ENABLE);
	//��λ��ʼ��DMA2������7
	DMA_DeInit(BSP_USART1_TX_DMA_STREAM);
	//ȷ��DMA2������7�ĸ�λ�ɹ�
//	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=DISABLE){}
	//ѡ��ͨ��4
	DMA_InitStruct.DMA_Channel=BSP_USART1_DMA_CHANNEL;
	//ѡ��Դ���ݵ�ַΪUSART1�����ݼĴ�����ַ
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)BSP_USART1_DR;
	//Ŀ���ַΪ�ڴ��ַ
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)USART1_RecvBuffer;
	//���䷽��Ϊ�洢��������
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralToMemory;
	//�������ݴ�С
	DMA_InitStruct.DMA_BufferSize=BSP_USART1_BufferSize;
	//�����ַ������
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//�ڴ��ַ������
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//�������ݵ�λ 32λ
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//�ڴ����ݵ�λ 32λ
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	//һ��ѭ��
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
	//�е����ȼ�
	DMA_InitStruct.DMA_Priority=DMA_Priority_VeryHigh;
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
	DMA_ClearFlag(BSP_USART1_TX_DMA_STREAM,DMA_FLAG_TCIF2);
	//DMA_ITConfig(BSP_USART1_TX_DMA_STREAM, DMA_IT_TE, ENABLE);
	//ʹ��DMA2
	DMA_Cmd(BSP_USART1_TX_DMA_STREAM,ENABLE);
	//�ȴ�DMA2��������Ч
	while(DMA_GetCmdStatus(BSP_USART1_TX_DMA_STREAM)!=ENABLE){}
	
}


void BSP_USART1_DMA_Rx_Data(void)
{
  BaseType_t	pxHigherPriorityTaskWoken;
  // �ر�DMA ����ֹ����
  DMA_Cmd(BSP_USART1_RX_DMA_STREAM, DISABLE);      
  // ��DMA��־λ
  DMA_ClearFlag(BSP_USART1_RX_DMA_STREAM,DMA_FLAG_TCIF2);         
  //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ
  DMA_SetCurrDataCounter(BSP_USART1_RX_DMA_STREAM,BSP_USART1_BufferSize);     
  DMA_Cmd(BSP_USART1_RX_DMA_STREAM, ENABLE);       
  /* 
  xSemaphoreGiveFromISR(SemaphoreHandle_t xSemaphore,
                      BaseType_t *pxHigherPriorityTaskWoken);
  */

  //������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
  xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
	printf("�ź���\n");
	
  //�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�	
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

}

//�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(BSP_USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(BSP_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

//�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(BSP_USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(BSP_USART1);
}


/*���͵����ַ�*/
void BSP_USART1_SendByte(USART_TypeDef *pUSARTx,uint8_t ch)
{

	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
	/* �ȴ��������ݼĴ���Ϊ�� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	
}

/*�����ַ���*/
void BSP_USART1_SendString(USART_TypeDef *pUSARTx, char *str)
{
	
	unsigned int len=0;
	do
  {  	
		//�������ַ���Ϊ����ַÿ��ƫ��һ���ַ�������
		BSP_USART1_SendByte(pUSARTx,*(str+len));
		len++;
		
	}while(*(str+len)!='\0');
	
	 /* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	
}

/*���յ����ַ�*/
uint16_t BSP_USART1_RecvByte(USART_TypeDef *pUSARTx)
{
	/* ��USART��ȡһ���ֽ����� */
	uint16_t USART1_RecvData=USART_ReceiveData(pUSARTx);
	/* �ȴ��������ݼĴ����ǿ� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_RXNE)!=RESET);
	
	return USART1_RecvData;
	
}

/*�����ַ���*/
char* BSP_USART1_RecvString(USART_TypeDef *pUSARTx,char *str)
{
	
	unsigned int len=0;
	do
	{
		
		BSP_USART1_RecvByte(pUSARTx);
		len++;
		
	}while(*(str+len)!='\0');
		 /* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_RXNE)==RESET);
	return  str;
}

//void USART1_IRQHandler(void)
//{	
//	
//	uint8_t Res;
//	if(USART_GetFlagStatus(BSP_USART1,USART_FLAG_RXNE)!=RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res=BSP_USART1_RecvByte(BSP_USART1);	//��ȡ���յ�������
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)
//				{
//					USART_RX_STA=0;      //���մ���,���¿�ʼ
//				}else{
//					USART_RX_STA|=0x8000;//��������� 
//				}	
//			}else{							//��û�յ�0X0D ��δ���ַ�����β
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
//						USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
//					}
//				}
//			}
//		}
//		
//	}
//	
//}
