#ifndef __BSP_ADC_TEMP_H
#define __BSP_ADC_TEMP_H

#include "stm32f4xx.h"

//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为0.76V即0x3AF
#define V25  0x3AF

//斜率 每摄氏度2.5mV 对应每摄氏度0x03

#define AVG_SLOPE 0x03 

void BSP_ADC1_MODE_Config(void);


#endif