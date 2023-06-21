#ifndef __ADC_H
#define __ADC_H    
#include "sys.h"

#define ADC_DATA_LENGTH 70
void Adc_Init(void);
void MYDMA_Init(u32 cpar,u32 cmar,u16 cndtr);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times);
extern u16 ADC_Value[ADC_DATA_LENGTH];
#endif 
