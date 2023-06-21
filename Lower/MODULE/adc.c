 #include "adc.h"
 #include "delay.h"
 #include "parameters.h"
 
 
u16 ADC_Value[ADC_DATA_LENGTH];
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void  Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE ); //使能ADC1通道时钟
 
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    //PA1 作为模拟通道输入引脚                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1); //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 7; //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
    
    ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1
    
    ADC_ResetCalibration(ADC1); //使能复位校准  
     
    while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
    
    ADC_StartCalibration(ADC1); //开启AD校准
 
    while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
    
    MYDMA_Init((u32)(&ADC1->DR), (u32)ADC_Value, ADC_DATA_LENGTH);
}



void MYDMA_Init(u32 cpar,u32 cmar,u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    //使能DMA传输
    
    DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    ADC_DMACmd(ADC1, ENABLE);
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的ADC1的软件转换启动功能
}



//获得ADC值
//ch:通道值 0~3
//u16 Get_Adc(u8 ch)   
//{
//      //设置指定ADC的规则组通道，一个序列，采样时间
//    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5); //ADC1,ADC通道,采样时间为239.5周期
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的ADC1的软件转换启动功能    
//     
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束

//    return ADC_GetConversionValue(ADC1); //返回最近一次ADC1规则组的转换结果
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//    u32 temp_val=0;
//    u8 t;
//    for(t=0;t<times;t++)
//    {
//        temp_val+=Get_Adc(ch);
//        delay_ms(3);
//    }
//    return temp_val/times;
//}      

 
 
 
 
 



