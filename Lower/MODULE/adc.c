 #include "adc.h"
 #include "delay.h"
 #include "parameters.h"
 
 
u16 ADC_Value[ADC_DATA_LENGTH];
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3
void  Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE ); //ʹ��ADC1ͨ��ʱ��
 
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    //PA1 ��Ϊģ��ͨ����������                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1); //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 7; //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
    
    ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1
    
    ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼  
     
    while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����
    
    ADC_StartCalibration(ADC1); //����ADУ׼
 
    while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
    
    MYDMA_Init((u32)(&ADC1->DR), (u32)ADC_Value, ADC_DATA_LENGTH);
}



void MYDMA_Init(u32 cpar,u32 cmar,u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    //ʹ��DMA����
    
    DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
    DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    ADC_DMACmd(ADC1, ENABLE);
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ָ����ADC1�����ת����������
}



//���ADCֵ
//ch:ͨ��ֵ 0~3
//u16 Get_Adc(u8 ch)   
//{
//      //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ָ����ADC1�����ת����������    
//     
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//�ȴ�ת������

//    return ADC_GetConversionValue(ADC1); //�������һ��ADC1�������ת�����
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

 
 
 
 
 



