#include "./Bsp/ADC_TEMP/bsp_adc_temp.h"

//ADC1_BASE��ַ��0x4000 0000+0x2400 ��ADC1�Ļ���ַ�������Ĺ������ݼĴ�����ƫ�Ƶ�ַ�ǣ�0x4c  ((u32)ADC1_BASE+0x4c)
#define ADC1_DR_Address  ((u32)ADC1_BASE+0x4c)


__IO u16 ADC_ConvertedValue_temp;

/*�������¶�ֵ*/
 u16 Current_Temp;
 
 
 /*
 * BSP_ADC1_MODE_Config
 * ����  ��ʹ��ADC1��DMA2��ʱ��
 * ����  :��
 * ���  ����
 * ����  ���ڲ�����
 */
void BSP_ADC1_MODE_Config(void)
{
	
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//Ϊ���ת������Ӧ�ٶȼ���DMA��Ϊ����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	//����ADC1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	// -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStruct);
	//ADC �ֱ���
	//����12λ����
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
	//���õ�ͨ������
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	//�����Զ�ת��
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;
	//��ʹ�ñ��ش���
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	//��ʹ��������� ��������
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
	//��ȡ�Ҷ��뷽ʽ ��Ϊ��λ����
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	;//ת��ͨ����ĿΪ1
	ADC_InitStruct.ADC_NbrOfConversion=1;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	 /* ADC1 regular channel16 configuration */ 
  //���ò���ͨ��ΪADC1 �� IN16,����˳��, ���ò���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2,ADC_SampleTime_144Cycles);
	//ʹ���¶ȴ��������ڲ��ο���ѹ 
	ADC_TempSensorVrefintCmd(ENABLE);
	
	// -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	// ʱ��Ϊfpclk x��Ƶ
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;
	// ��ֹDMAֱ�ӷ���ģʽ
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	// ����ʱ����	
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	// -------------------ADC DMA �ṹ�� ���� ��ʼ��------------------------
	// �������ַ ADC1�Ĺ������ݼĴ���
	DMA_InitStruct.DMA_PeripheralBaseAddr=ADC1_DR_Address;
	// ADת��ֵ����ŵ��ڴ����ַ	�����Ǹ�����ַ��
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)&ADC_ConvertedValue_temp;
	// ������Ϊ���ݴ������Դ	
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralToMemory;
	// ����ָ��DMAͨ�� DMA����Ĵ�С
	DMA_InitStruct.DMA_BufferSize=1;
	// �����ַ�Ĵ���������
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	// �ڴ��ַ�Ĵ���������
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Disable;
	// �������ݿ��Ϊ16λ
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	// �ڴ����ݿ��Ϊ16λ
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	//������ѭ��ģʽ�� �����ֶ�����
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
	//�����ȼ�
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;
	// ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;
	// FIFO ��С��FIFOģʽ��ֹʱ�������������
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	//û������Ϊ�ڴ浽�ڴ�Ĵ���
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	// ѡ�� DMA ͨ����ͨ������������
	DMA_InitStruct.DMA_Channel=DMA_Channel_0;
	//��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(DMA2_Stream0,&DMA_InitStruct);
	// ʹ��DMA��
	DMA_Cmd(DMA2_Stream0,ENABLE);
  // ʹ��DMA���� after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	/* ʹ�� ADC1��DMAͨ�� */
	ADC_DMACmd(ADC1,ENABLE);
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	//��ʼadcת�����������
	ADC_SoftwareStartConv(ADC1);
	
}
  /*
 * BSP_ADC_DMA_TEMP
 * ����  ��ʹ��ADC1��DMA2��ʱ��
 * ����  :��
 * ���  ����
 * ����  ���ڲ�����
 */