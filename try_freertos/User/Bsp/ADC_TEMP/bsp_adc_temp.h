#ifndef __BSP_ADC_TEMP_H
#define __BSP_ADC_TEMP_H

#include "stm32f4xx.h"

//����12λ��ADC��3.3V��ADCֵΪ0xfff,�¶�Ϊ25��ʱ��Ӧ�ĵ�ѹֵΪ0.76V��0x3AF
#define V25  0x3AF

//б�� ÿ���϶�2.5mV ��Ӧÿ���϶�0x03

#define AVG_SLOPE 0x03 

void BSP_ADC1_MODE_Config(void);


#endif