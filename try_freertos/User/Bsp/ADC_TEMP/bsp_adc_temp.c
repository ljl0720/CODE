#include "./Bsp/ADC_TEMP/bsp_adc_temp.h"

//ADC1_BASE地址：0x4000 0000+0x2400 即ADC1的基地址，而他的规则数据寄存器的偏移地址是：0x4c  ((u32)ADC1_BASE+0x4c)
#define ADC1_DR_Address  ((u32)ADC1_BASE+0x4c)


__IO u16 ADC_ConvertedValue_temp;

/*计算后的温度值*/
 u16 Current_Temp;
 
 
 /*
 * BSP_ADC1_MODE_Config
 * 描述  ：使能ADC1和DMA2的时钟
 * 输入  :无
 * 输出  ：无
 * 调用  ：内部调用
 */
void BSP_ADC1_MODE_Config(void)
{
	
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	//为提高转换和响应速度计入DMA作为辅助
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	//开启ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	// -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStruct);
	//ADC 分辨率
	//采用12位采样
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
	//采用单通道采样
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	//启用自动转换
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;
	//不使用边沿触发
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	//因使用软件触发 无需配置
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
	//采取右对齐方式 即为低位对齐
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	;//转换通道数目为1
	ADC_InitStruct.ADC_NbrOfConversion=1;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	 /* ADC1 regular channel16 configuration */ 
  //设置采样通道为ADC1 的 IN16,采样顺序, 设置采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2,ADC_SampleTime_144Cycles);
	//使能温度传感器和内部参考电压 
	ADC_TempSensorVrefintCmd(ENABLE);
	
	// -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	// 时钟为fpclk x分频
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;
	// 禁止DMA直接访问模式
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	// 采样时间间隔	
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	// -------------------ADC DMA 结构体 参数 初始化------------------------
	// 外设基地址 ADC1的规则数据寄存器
	DMA_InitStruct.DMA_PeripheralBaseAddr=ADC1_DR_Address;
	// AD转换值所存放的内存基地址	（就是给个地址）
	DMA_InitStruct.DMA_Memory0BaseAddr=(uint32_t)&ADC_ConvertedValue_temp;
	// 外设作为数据传输的来源	
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralToMemory;
	// 定义指定DMA通道 DMA缓存的大小
	DMA_InitStruct.DMA_BufferSize=1;
	// 外设地址寄存器不自增
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	// 内存地址寄存器不自增
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Disable;
	// 外设数据宽度为16位
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	// 内存数据宽度为16位
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	//工作在循环模式下 无需手动重启
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
	//高优先级
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;
	// 禁止DMA FIFO	，使用直连模式
	DMA_InitStruct.DMA_FIFOMode=DMA_FIFOMode_Disable;
	// FIFO 大小，FIFO模式禁止时，这个不用配置
	DMA_InitStruct.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	//没有设置为内存到内存的传输
	DMA_InitStruct.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	// 选择 DMA 通道，通道存在于流中
	DMA_InitStruct.DMA_Channel=DMA_Channel_0;
	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(DMA2_Stream0,&DMA_InitStruct);
	// 使能DMA流
	DMA_Cmd(DMA2_Stream0,ENABLE);
  // 使能DMA请求 after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	/* 使能 ADC1的DMA通道 */
	ADC_DMACmd(ADC1,ENABLE);
	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	//开始adc转换，软件触发
	ADC_SoftwareStartConv(ADC1);
	
}
  /*
 * BSP_ADC_DMA_TEMP
 * 描述  ：使能ADC1和DMA2的时钟
 * 输入  :无
 * 输出  ：无
 * 调用  ：内部调用
 */